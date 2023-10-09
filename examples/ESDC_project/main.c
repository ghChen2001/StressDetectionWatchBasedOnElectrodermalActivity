// #include "shell.h"
#include <FreeRTOS.h>
#include "semphr.h"
#include "task.h"
#include "board.h"

#include "bluetooth.h"
#include "conn.h"
#include "conn_internal.h"
#if defined(BL702) || defined(BL602)
#include "ble_lib_api.h"
#elif defined(BL616)
#include "btble_lib_api.h"
#endif
#include "hci_driver.h"
#include "bl616_glb.h"
#include "ble_tp_svc.h"
#include "rfparam_adapter.h"
#include "hci_core.h"
// #include "hrs.h"
// #include "bas.h"
#include "mysvc.h"

/* WIFI */
#include <lwip/tcpip.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <lwipopts_user.h>

#include "bl_fw_api.h"
#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
#include "onenet/onenet_post.h"

#include "bflb_mtimer.h"
#include "bflb_spi.h"
#include "bflb_dma.h"
#include "bflb_i2c.h"
#include "bflb_l1c.h"
#include "bflb_rtc.h"

/* LVGL */
#include "lv_conf.h"
#include "lvgl.h"
// #include "demos/lv_demos.h"
#include "myproject/ui.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lcd.h"

#include "AD5940Port.h"
#include "AD5940.h"
#include "ElectrodermalActivity.h"
// #include "Algorithm/sparsEDA/sparsEDA.h"
// #include "cvxEDA/cvxEDA.h"
// #include "cvxEDA/cvxEDA_emxAPI.h"
// #include "cvxEDA/cvxEDA_terminate.h"
// #include "cvxEDA/cvxEDA_types.h"
// #include "cvxEDA/rt_nonfinite.h"

#include "lis2dh12_reg.h"
#include "embedded_pedometer/count_steps.h"
// #include "M601.h"
// #include "M117/m117_drv.h"
#include "TMP117/tmp117_drv.h"

#include "afe4404/AFE4404.h"
#include "afe4404/Calibration_AFE4404.h"
#include "afe4404/hrm.h"
#include "afe4404/AFE4404_PORT.h"

#include "csi_math.h"
#include "tinymaix.h"
#include "model/esdc_model.h"
#include "model/esdc_model_bin.h"
#include "Algorithm/CControl/Headers/Functions.h"

// Findpeaks
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#include "my_findpeaks.h"
#include "my_findpeaks_emxAPI.h"
#include "my_findpeaks_terminate.h"
#include "my_findpeaks_types.h"
#include "rt_nonfinite.h"

// #include "MAX30101/max30101_algo.h"
// #include "MAX30101/max30101_drv.h"
// Time
#include <lwip/apps/sntp.h>
#include "bflb_timestamp.h"

#define DBG_TAG "MAIN"
#include "log.h"

#define BUFFERLEN          32
#define APPBUFF_SIZE       512
#define ACC_FIFO_WATERMARK 5

/* WIFI */
#define WIFI_STACK_SIZE    (1536)
#define TASK_PRIORITY_FW   configMAX_PRIORITIES - 2

#define GMTp8              8 * 60 * 60
#define GMTm7              -7 * 60 * 60

void *__dso_handle = NULL;

static TaskHandle_t EDA_handle;
static TaskHandle_t ACCE_handle;
static TaskHandle_t TEMP_handle;
static TaskHandle_t HR_handle;
static TaskHandle_t LVGL_handle;
// static TaskHandle_t WIRELESS_handle;
static TaskHandle_t CLOCK_handle;
static TaskHandle_t WIFI_handle;
static TaskHandle_t Algo_handle;
static TaskHandle_t wifi_fw_task;
static TaskHandle_t BLE_handle;
static wifi_conf_t conf = {
    .country_code = "CN",
};
static bool wifi_connected = false;
static uint8_t wifiInitDone = 0;

static struct bflb_device_s *uart0;
struct bflb_device_s *gpio;
struct bflb_device_s *spi0;        // For AD5940
static struct bflb_device_s *i2c0; // For LIS2DH12, TMP117, AFE4404
struct bflb_device_s *rtc;
// struct bflb_device_s *dma0_ch0;
// struct bflb_device_s *dma0_ch1;

// AD5940
uint8_t ucInterrupted;
float LFOSCFreq; /* Measured LFOSC frequency */
uint32_t ResistorForBaseline = 0;
uint32_t AppBuff[APPBUFF_SIZE];
// fImpCar_Type EDABase = {
//     .Real = 17637.3361f,
//     .Image = -111117.5284f,
// };
fImpCar_Type EDABase = {
    .Real = 53718.496f,
    .Image = -88349.547f,
};
uint32_t err_code_ad5940;

// LIS2DH12
static uint8_t whoamI;
static bl616_ctx_t dev_ctx;
static int16_t data_raw_acceleration[3];
static float acceleration_mg[3];
static float acceWindow[300] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
static uint16_t acceWindowPtr = 0;
static uint32_t stepCnt = 0;

/* AFE4404 */
uint8_t afe4404_adcrdy = 0;
// HRM algo
extern unsigned char HeartRate;
uint8_t offsetDACcalibFlag = 0;
signed long AFE44xx_SPO2_Data_buf[6] = { 0 };
volatile uint8_t bCDCDataReceived_event = 0; //Indicates data has been received without an open rcv operation
uint8_t CALIBRATION_ENABLED = 0;
uint32_t hr_transfer = 0;
uint32_t hr_onenet = 0;
extern unsigned int interArrayWindow[111];
extern unsigned int iA_pointer;

enum CAP_MODE { FINITE,
                CONTINUOUS };
char captureMode = FINITE;
char captureInProgressFlag = 0;

char sendDataFlag = 0;

unsigned char AFE44xxAddr;
unsigned long AFE44xxRegVal;
unsigned long totalCount;
unsigned long sampleCount;

// There should be a global variable called g_OneSecondFlag which should be set to '1' each time you receive 100 samples (PRF number of samples, PRF =100)
// the calibration code will reset it to 0
// volatile int g_OneSecondFlag; should be present in the project as a global variable
/*Global variables declared in the source code - Can be used in other files in the project*/
volatile int g_OneSecondFlag = 0;
uint16_t prfcount = 0;
extern char LED_Sel;
//(for 1, read from reg 44 and reg 45, for 2, read from reg 42 and reg 43)
extern int Calibration; // Global variable which is controlled by the library. Keep checking the status of the variable
// 0 means Calibration is over (no need to call the CalibrateAFE4404()
// 1 means Calibration is running (need to call the CalibrateAFE4404()
extern long CTR;
extern unsigned long AFE44xx_Current_Register_Settings[5];

/* END AFE4404 */

/* MAX30101 */
/* END MAX30101 */
static float Tempurature = 0;

/* freeRTOS */
SemaphoreHandle_t xMutex;
SemaphoreHandle_t xMutex_printf;
SemaphoreHandle_t xMutex_lvgl;
SemaphoreHandle_t xMutex_interArrayWindow; // For BVP window update in HR task
SemaphoreHandle_t xMutex_DSP;
/* END freeRTOS */

/* Algo */
float EDAwindow_o[4 * 30] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
float EDAwindow_f[4 * 30];
// float SCR_nonezero[25] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
// uint32_t SCR_nonezero_timepoint[25] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
uint32_t eda_cycle_cnt = 0;
uint8_t scr_transfer_pointer = 0;
uint8_t eda_ready_flag = 0;
uint16_t EDAcnt = 0;
float TEMPwindow[4 * 30] = { 0 };
uint8_t tw_pointer = 0;
int output_class = -1;
/* END Algo*/

long long time_base = 0;

// static const uint8_t m_length = BUFFERLEN;                           /**< Transfer length. */
// static ATTR_NOCACHE_NOINIT_RAM_SECTION uint8_t tx_buffer[BUFFERLEN]; /**< TX buffer. */
// static ATTR_NOCACHE_NOINIT_RAM_SECTION uint8_t rx_buffer[BUFFERLEN]; /**< RX buffer. */

// struct bflb_dma_channel_lli_pool_s tx_llipool[1];
// struct bflb_dma_channel_lli_transfer_s tx_transfers[1];
// struct bflb_dma_channel_lli_pool_s rx_llipool[1];
// struct bflb_dma_channel_lli_transfer_s rx_transfers[1];

volatile bool spi0_tc = false; /**< Flag used to indicate that SPI instance completed the transfer. */

// extern void shell_init_with_task(struct bflb_device_s *shell);

/* Private Function Defination */
/*Function Definations For LIS2DH12 */
static int32_t lis2dh12_platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
                                       uint16_t len);
static int32_t lis2dh12_platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                                      uint16_t len);
// static void tx_com(uint8_t *tx_buffer, uint16_t len);
// static void platform_delay(uint32_t ms);
// static void platform_init(void);
/*END Function Definations For LIS2DH12 */

/* lvgl log cb */
void lv_log_print_g_cb(const char *buf)
{
    printf("[LVGL] %s", buf);
}

static int btblecontroller_em_config(void)
{
    extern uint8_t __LD_CONFIG_EM_SEL;
    volatile uint32_t em_size;

    em_size = (uint32_t)&__LD_CONFIG_EM_SEL;

    if (em_size == 0) {
        GLB_Set_EM_Sel(GLB_WRAM160KB_EM0KB);
    } else if (em_size == 32 * 1024) {
        GLB_Set_EM_Sel(GLB_WRAM128KB_EM32KB);
    } else if (em_size == 64 * 1024) {
        GLB_Set_EM_Sel(GLB_WRAM96KB_EM64KB);
    } else {
        GLB_Set_EM_Sel(GLB_WRAM96KB_EM64KB);
    }

    return 0;
}

static void ble_connected(struct bt_conn *conn, u8_t err)
{
    if (err || conn->type != BT_CONN_TYPE_LE) {
        return;
    }
    printf("%s", __func__);

    xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
    ui_setBleImg(1);
    xSemaphoreGive(xMutex_lvgl);
}

static void ble_disconnected(struct bt_conn *conn, u8_t reason)
{
    int ret;

    if (conn->type != BT_CONN_TYPE_LE) {
        return;
    }

    printf("%s", __func__);
    xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
    ui_setBleImg(0);
    xSemaphoreGive(xMutex_lvgl);

    // enable adv
    ret = set_adv_enable(true);
    if (ret) {
        printf("Restart adv fail. \r\n");
    }
}

static struct bt_conn_cb ble_conn_callbacks = {
    .connected = ble_connected,
    .disconnected = ble_disconnected,
};

static void ble_start_adv(void)
{
    struct bt_le_adv_param param;
    int err = -1;
    struct bt_data adv_data[] = {
        BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR | BT_LE_AD_GENERAL),
        //BT_DATA_BYTES(BT_DATA_NAME_COMPLETE, "SmartWatch_1234")
    };
    struct bt_data adv_rsp[1] = {
        BT_DATA_BYTES(BT_DATA_MANUFACTURER_DATA, "BL616")
    };

    memset(&param, 0, sizeof(param));
    // Set advertise interval
    param.interval_min = BT_GAP_ADV_FAST_INT_MIN_2;
    param.interval_max = BT_GAP_ADV_FAST_INT_MAX_2;
    /*Get adv type, 0:adv_ind,  1:adv_scan_ind, 2:adv_nonconn_ind 3: adv_direct_ind*/
    param.options = (BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_USE_NAME | BT_LE_ADV_OPT_ONE_TIME);

    err = bt_le_adv_start(&param, adv_data, ARRAY_SIZE(adv_data), adv_rsp, ARRAY_SIZE(adv_rsp));
    if (err) {
        printf("Failed to start advertising (err %d) \r\n", err);
    }
    printf("Start advertising success.\r\n");
}

void bt_enable_cb(int err)
{
    if (!err) {
        bt_addr_le_t bt_addr;
        bt_get_local_public_address(&bt_addr);
        printf("BD_ADDR:(MSB)%02x:%02x:%02x:%02x:%02x:%02x(LSB) \r\n",
               bt_addr.a.val[5], bt_addr.a.val[4], bt_addr.a.val[3], bt_addr.a.val[2], bt_addr.a.val[1], bt_addr.a.val[0]);

        bt_conn_cb_register(&ble_conn_callbacks);
        ble_tp_init();

        // start advertising
        ble_start_adv();
    }
}

/****************************************************************************
 * Functions
 ****************************************************************************/

int wifi_start_firmware_task(void)
{
    LOG_I("Starting wifi ...\r\n");

    /* enable wifi clock */

    GLB_PER_Clock_UnGate(GLB_AHB_CLOCK_IP_WIFI_PHY | GLB_AHB_CLOCK_IP_WIFI_MAC_PHY | GLB_AHB_CLOCK_IP_WIFI_PLATFORM);
    GLB_AHB_MCU_Software_Reset(GLB_AHB_MCU_SW_WIFI);

    /* set ble controller EM Size */

    GLB_Set_EM_Sel(GLB_WRAM160KB_EM0KB);

    if (0 != rfparam_init(0, NULL, 0)) {
        LOG_I("PHY RF init failed!\r\n");
        return 0;
    }

    LOG_I("PHY RF init success!\r\n");

    /* Enable wifi irq */

    extern void interrupt0_handler(void);
    bflb_irq_attach(WIFI_IRQn, (irq_callback)interrupt0_handler, NULL);
    // bflb_irq_set_priority(WIFI_IRQn, 7, 0);
    bflb_irq_enable(WIFI_IRQn);

    xTaskCreate(wifi_main, (char *)"fw", WIFI_STACK_SIZE, NULL, TASK_PRIORITY_FW, &wifi_fw_task);

    return 0;
}

void wifi_event_handler(uint32_t code)
{
    switch (code) {
        case CODE_WIFI_ON_INIT_DONE: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_INIT_DONE\r\n", __func__);
            wifi_mgmr_init(&conf);
        } break;
        case CODE_WIFI_ON_MGMR_DONE: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_MGMR_DONE\r\n", __func__);
            wifiInitDone = 1;
        } break;
        case CODE_WIFI_ON_SCAN_DONE: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_SCAN_DONE\r\n", __func__);
            wifi_mgmr_sta_scanlist();
        } break;
        case CODE_WIFI_ON_CONNECTED: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_CONNECTED\r\n", __func__);
            wifi_connected = true;

            sntp_setoperatingmode(SNTP_OPMODE_POLL);
            sntp_init();
            sntp_setservername(0, "pool.ntp.org");
            sntp_setservername(1, "ntp1.aliyun.com");
            sntp_setservername(2, "158.69.48.97");

            void mm_sec_keydump();
            mm_sec_keydump();
        } break;
        case CODE_WIFI_ON_GOT_IP: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_GOT_IP\r\n", __func__);
        } break;
        case CODE_WIFI_ON_DISCONNECT: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_DISCONNECT\r\n", __func__);
            wifi_connected = false;
            sntp_stop();
        } break;
        case CODE_WIFI_ON_AP_STARTED: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_AP_STARTED\r\n", __func__);
        } break;
        case CODE_WIFI_ON_AP_STOPPED: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_AP_STOPPED\r\n", __func__);
        } break;
        case CODE_WIFI_ON_AP_STA_ADD: {
            LOG_I("[APP] [EVT] [AP] [ADD] %lld\r\n", xTaskGetTickCount());
        } break;
        case CODE_WIFI_ON_AP_STA_DEL: {
            LOG_I("[APP] [EVT] [AP] [DEL] %lld\r\n", xTaskGetTickCount());
        } break;
        default: {
            LOG_I("[APP] [EVT] Unknown code %u \r\n", code);
        }
    }
}

void spi_isr(int irq, void *arg)
{
    uint32_t intstatus = bflb_spi_get_intstatus(spi0);
    // printf("tc done1\r\n");
    if (intstatus & SPI_INTSTS_TC) {
        bflb_spi_int_clear(spi0, SPI_INTCLR_TC);
        spi0_tc = true;
        // printf("tc done\r\n");
    } else if (intstatus & SPI_INTSTS_TX_FIFO) {
        // printf("tx fifo\r\n");
    } else if (intstatus & SPI_INTSTS_RX_FIFO) {
        // printf("rx fifo\r\n");
    }
}

/* MCU related external line interrupt service routine */
void GPIO_Interrupt(int irq, void *arg)
{
    if (bflb_gpio_get_intstatus(gpio, AFE_ADC_DRDY)) {
        afe4404_adcrdy = 1;
        bflb_gpio_int_clear(gpio, AFE_ADC_DRDY);
    }

    if (bflb_gpio_get_intstatus(gpio, KEY0)) {
        ucInterrupted = 1;
        // printf("AD5940 INT: %d\r\n", AD5940_GetMCUIntFlag());
        bflb_gpio_int_clear(gpio, KEY0);
    }
}

static void peripheral_init(void)
{
    /* spi0 Init */
    // For AD5940/5941

    /* spi clk */
    bflb_gpio_init(gpio, GPIO_PIN_25, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    /* spi miso */
    bflb_gpio_init(gpio, GPIO_PIN_18, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    /* spi mosi */
    bflb_gpio_init(gpio, GPIO_PIN_19, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    /* CS controlled by software */

    /* Set the SPI parameters */
    struct bflb_spi_config_s spi_cfg = {
#if (SPI_CASE_SELECT == SPI_MASTER_CASE)
        .freq = 16 * 1000 * 1000,
        .role = SPI_ROLE_MASTER,
#else
        .freq = 32 * 1000 * 1000,
        .role = SPI_ROLE_SLAVE,
#endif
        .mode = SPI_MODE0,
        .data_width = SPI_DATA_WIDTH_8BIT,
        .bit_order = SPI_BIT_MSB,
        .byte_order = SPI_BYTE_LSB,
        .tx_fifo_threshold = 0,
        .rx_fifo_threshold = 0,
    };

    // struct bflb_dma_channel_config_s tx_config = {
    //     .direction = DMA_MEMORY_TO_PERIPH,
    //     .src_req = DMA_REQUEST_NONE,
    //     .dst_req = DMA_REQUEST_SPI0_TX,
    //     .src_addr_inc = DMA_ADDR_INCREMENT_ENABLE,
    //     .dst_addr_inc = DMA_ADDR_INCREMENT_DISABLE,
    //     .src_burst_count = DMA_BURST_INCR1,
    //     .dst_burst_count = DMA_BURST_INCR1,
    //     .src_width = DMA_DATA_WIDTH_8BIT,
    //     .dst_width = DMA_DATA_WIDTH_8BIT,
    // };

    // struct bflb_dma_channel_config_s rx_config = {
    //     .direction = DMA_PERIPH_TO_MEMORY,
    //     .src_req = DMA_REQUEST_SPI0_RX,
    //     .dst_req = DMA_REQUEST_NONE,
    //     .src_addr_inc = DMA_ADDR_INCREMENT_DISABLE,
    //     .dst_addr_inc = DMA_ADDR_INCREMENT_ENABLE,
    //     .src_burst_count = DMA_BURST_INCR1,
    //     .dst_burst_count = DMA_BURST_INCR1,
    //     .src_width = DMA_DATA_WIDTH_8BIT,
    //     .dst_width = DMA_DATA_WIDTH_8BIT,
    // };
    
    /* Now do not use DMA */
    bflb_spi_init(spi0, &spi_cfg);
    // bflb_spi_txint_mask(spi0, false);
    // bflb_spi_rxint_mask(spi0, false);
    bflb_spi_tcint_mask(spi0, false);              // Disable Interrupt
    bflb_irq_attach(spi0->irq_num, spi_isr, NULL); // IRQ
    bflb_irq_enable(spi0->irq_num);
    // bflb_spi_feature_control(spi0, SPI_CMD_SET_CS_INTERVAL, 0);

    /* AD5941 Pin Init */
    //Interrupt Key
    bflb_gpio_init(gpio, KEY0, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_INT_TRIG_MODE_ASYNC_FALLING_EDGE);
    bflb_gpio_int_mask(gpio, KEY0, false);
    //RST pin
    bflb_gpio_init(gpio, AD5940_RST_PIN, GPIO_OUTPUT);
    //Soft CS pin
    bflb_gpio_init(gpio, AD5940_CS_PIN, GPIO_OUTPUT);

    AD5940_CsSet();
    AD5940_RstSet();

    // bflb_spi_link_txdma(spi0, true);
    // bflb_spi_link_rxdma(spi0, true);

    // dma0_ch0 = bflb_device_get_by_name("dma0_ch0");
    // dma0_ch1 = bflb_device_get_by_name("dma0_ch1");

    // bflb_dma_channel_init(dma0_ch0, &tx_config);
    // bflb_dma_channel_init(dma0_ch1, &rx_config);

    // bflb_dma_channel_irq_attach(dma0_ch0, dma0_ch0_isr, NULL);
    // bflb_dma_channel_irq_attach(dma0_ch1, dma0_ch1_isr, NULL);

    /* END spi0 Init */

    /* i2c0 Init */
    // For LIS2DH12, M117, AFE4404
    /* I2C0_SDA */
    bflb_gpio_init(gpio, GPIO_PIN_27, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    /* I2C0_SCL */
    bflb_gpio_init(gpio, GPIO_PIN_26, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0);
    bflb_i2c_init(i2c0, 400000);
    /* END i2c0 init*/

    /* Attach IRQ */
    bflb_irq_attach(gpio->irq_num, GPIO_Interrupt, gpio);

    /* AFE4404 Pin Init */
    bflb_gpio_init(gpio, AFE_ADC_DRDY, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_INT_TRIG_MODE_ASYNC_RISING_EDGE);
    bflb_gpio_int_mask(gpio, AFE_ADC_DRDY, true); // Now Disable Pin 28 Interrupt For AFE4404

    // RST
    bflb_gpio_init(gpio, AFE_RESETZ, GPIO_OUTPUT | GPIO_FLOAT | GPIO_SMT_EN);
    bflb_gpio_set(gpio, AFE_RESETZ);

    /* Enable IRQ */
    // bflb_irq_set_priority(gpio->irq_num, 1, 0);
    bflb_irq_enable(gpio->irq_num);
}

/* Initialize AD5940 basic blocks like clock */
static int32_t AD5940PlatformCfg(void)
{
    CLKCfg_Type clk_cfg;
    FIFOCfg_Type fifo_cfg;
    SEQCfg_Type seq_cfg;
    AGPIOCfg_Type gpio_cfg;
    LFOSCMeasure_Type LfoscMeasure;

    //  /* Use hardware reset */
    //  AD5940_HWReset();
    //  /* Platform configuration */
    //  AD5940_Initialize();

    /* Step1. Configure clock */
    clk_cfg.ADCClkDiv = ADCCLKDIV_1;
    clk_cfg.ADCCLkSrc = ADCCLKSRC_HFOSC;
    clk_cfg.SysClkDiv = SYSCLKDIV_1;
    clk_cfg.SysClkSrc = SYSCLKSRC_HFOSC;
    clk_cfg.HfOSC32MHzMode = bFALSE;
    clk_cfg.HFOSCEn = bTRUE;
    clk_cfg.HFXTALEn = bFALSE;
    clk_cfg.LFOSCEn = bTRUE;
    AD5940_CLKCfg(&clk_cfg);
    /* Step2. Configure FIFO and Sequencer*/
    fifo_cfg.FIFOEn = bFALSE;
    fifo_cfg.FIFOMode = FIFOMODE_FIFO;
    fifo_cfg.FIFOSize = FIFOSIZE_4KB; /* 4kB for FIFO, The reset 2kB for sequencer */
    fifo_cfg.FIFOSrc = FIFOSRC_DFT;
    fifo_cfg.FIFOThresh = 4;   //AppBIACfg.FifoThresh;        /* DFT result. One pair for RCAL, another for Rz. One DFT result have real part and imaginary part */
    AD5940_FIFOCfg(&fifo_cfg); /* Disable to reset FIFO. */
    fifo_cfg.FIFOEn = bTRUE;
    AD5940_FIFOCfg(&fifo_cfg); /* Enable FIFO here */
    /* Configure sequencer and stop it */
    seq_cfg.SeqMemSize = SEQMEMSIZE_2KB;
    seq_cfg.SeqBreakEn = bFALSE;
    seq_cfg.SeqIgnoreEn = bFALSE;
    seq_cfg.SeqCntCRCClr = bTRUE;
    seq_cfg.SeqEnable = bFALSE;
    seq_cfg.SeqWrTimer = 0;
    AD5940_SEQCfg(&seq_cfg);

    /* Step3. Interrupt controller */
    AD5940_INTCCfg(AFEINTC_1, AFEINTSRC_ALLINT, bTRUE);         /* Enable all interrupt in Interrupt Controller 1, so we can check INTC flags */
    AD5940_INTCCfg(AFEINTC_0, AFEINTSRC_DATAFIFOTHRESH, bTRUE); /* Interrupt Controller 0 will control GP0 to generate interrupt to MCU */
    AD5940_INTCClrFlag(AFEINTSRC_ALLINT);
    /* Step4: Reconfigure GPIO */
    gpio_cfg.FuncSet = GP6_SYNC | GP5_SYNC | GP4_SYNC | GP2_EXTCLK | GP1_SYNC | GP0_INT;
    gpio_cfg.InputEnSet = AGPIO_Pin0;
    gpio_cfg.OutputEnSet = AGPIO_Pin0 | AGPIO_Pin1 | AGPIO_Pin4 | AGPIO_Pin5 | AGPIO_Pin6;
    gpio_cfg.OutVal = 0;
    gpio_cfg.PullEnSet = 0;
    AD5940_AGPIOCfg(&gpio_cfg);

    AD5940_SleepKeyCtrlS(SLPKEY_UNLOCK); /* Enable AFE to enter sleep mode. */

    /* Measure LFOSC frequency */
    LfoscMeasure.CalDuration = 1000.0; /* 1000ms used for calibration. */
    LfoscMeasure.CalSeqAddr = 0;
    LfoscMeasure.SystemClkFreq = 16000000.0f; /* 16MHz in this firmware. */
    AD5940_LFOSCMeasure(&LfoscMeasure, &LFOSCFreq);
    printf("Freq:%.3f\r\n", LFOSCFreq);

    return 0;
}

void AD5940_ReadWriteNBytes(unsigned char *pSendBuffer, unsigned char *pRecvBuff, unsigned long length)
{
    spi0_tc = false;
    // printf("Enter AD5940_ReadWriteNBytes");

    // tx_transfers[0].src_addr = (uint32_t)tx_buffer;
    // tx_transfers[0].dst_addr = (uint32_t)DMA_ADDR_SPI0_TDR;
    // tx_transfers[0].nbytes = length;

    // rx_transfers[0].src_addr = (uint32_t)DMA_ADDR_SPI0_RDR;
    // rx_transfers[0].dst_addr = (uint32_t)rx_buffer;
    // rx_transfers[0].nbytes = length;

    // bflb_dma_channel_lli_reload(dma0_ch0, tx_llipool, 1, tx_transfers, 1);
    // bflb_dma_channel_lli_reload(dma0_ch1, rx_llipool, 1, rx_transfers, 1);
    // printf("bflb_dma_channel_lli_reload");
    // bflb_dma_channel_start(dma0_ch0);
    // printf("bflb_dma_channel_start(dma0_ch0)");
    // bflb_dma_channel_start(dma0_ch1);
    // printf("bflb_dma_channel_start");

    taskENTER_CRITICAL();
    // xSemaphoreTake(xMutex,
    //                portMAX_DELAY);
    bflb_spi_poll_exchange(spi0, pSendBuffer, pRecvBuff, length);
    // xSemaphoreGive(xMutex);
    taskEXIT_CRITICAL();
}

void AD5940EDAStructInit(void)
{
    AppEDACfg_Type *pCfg;

    AppEDAGetCfg(&pCfg);
    pCfg->MaxSeqLen = 512;

    pCfg->LfoscClkFreq = LFOSCFreq;    /* Don't do LFOSC calibration now. We assume the default LFOSC is trimmed. */
    pCfg->RtiaAutoScaleEnable = bTRUE; /* We manually select resistor value. */
    pCfg->LptiaRtiaSel = LPTIARTIA_120K;
    pCfg->SinAmplitude = 1100 * 3 / 4; /* Set excitation voltage to 0.75 times of full range. */
    pCfg->SinFreq = 100.0f;
    pCfg->SampleFreq = 400.0f; /* Do not change sample frequency unless you know how it works. */
    pCfg->EDAODR = 4.0f;       /* ODR decides how frequently to start the engine to measure impedance. */
    pCfg->FifoThresh = 8;      /* The minimum threshold value is 4, and should always be 4*N, where N is 1,2,3... */
    pCfg->bParaChanged = bTRUE;
    //pCfg->NumOfData = -1;						/* Never stop until you stop it manually by AppBIACtrl() function */
    pCfg->RcalVal = 10000.0;
    pCfg->DftNum = DFTNUM_16;
}

/* print EDA result to uart */
AD5940Err EDAShowResult(void *pData, uint32_t DataCount)
{
    float RtiaMag;
    float S;
    /*Process data*/
    fImpCar_Type *pImp = (fImpCar_Type *)pData;
    float mag, phase;
    fImpCar_Type res;
    AppEDACtrl(EDACTRL_GETRTIAMAG, &RtiaMag);

    /*Process data*/
    for (int i = 0; i < DataCount; i++) {
        res = pImp[i];
        res.Real += ResistorForBaseline; /* Show the real result of impedance under test(between F+/S+) */
        mag = AD5940_ComplexMag(&res);
        phase = AD5940_ComplexPhase(&res) * 180 / MATH_PI;
        S = 1000000 / mag;
        printf("Rtia:%.3f; Real:%.3f; Image:%.3f; Mag:%.3f; Conductance: %.3f uS; Phase:%.3f \r\n", RtiaMag, res.Real, res.Image, mag, S, phase);
        printf("Conductance=%.3f\r\n", S);
        // eda_transfer_cnt++;
        if (isnanf(S) || (S > 100.0f)) {
            EDAcnt = 0;
        } else {
            EDAwindow_o[EDAcnt] = S;
            EDAcnt++;
        }
    }
    return 0;
}

uint8_t BL618_I2C_TRANSMIT(struct bflb_i2c_msg_s *msg, uint16_t msglen)
{
    xSemaphoreTake(xMutex,
                   portMAX_DELAY);
    uint8_t ret_code = bflb_i2c_transfer(i2c0, msg, msglen);
    xSemaphoreGive(xMutex);

    return ret_code;
}

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  handle    customizable argument. 
 * @param  reg       register to write
 * @param  bufp      pointer to data to write in register reg
 * @param  len       number of consecutive register to write
 *
 */
static int32_t lis2dh12_platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
                                       uint16_t len)
{
    /* Write */
    reg |= 0x80;
    struct bflb_i2c_msg_s msgs[1];
    uint8_t subaddr[1 + len];

    subaddr[0] = reg;
    for (int8_t i = 0; i < len; i++) {
        subaddr[i + 1] = bufp[i];
    }

    msgs[0].addr = 0x30 >> 1;
    msgs[0].flags = I2C_M_NOSTART;
    msgs[0].buffer = subaddr;
    msgs[0].length = len + 1;

    return BL618_I2C_TRANSMIT(msgs, 1);
}

/*
 * @brief  Read generic device register (platform dependent)
 *
 * @param  handle    customizable argument. 
 * @param  reg       register to read
 * @param  bufp      pointer to buffer that store the data read
 * @param  len       number of consecutive register to read
 *
 */
static int32_t lis2dh12_platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                                      uint16_t len)
{
    /* Read */
    reg |= 0x80;
    struct bflb_i2c_msg_s msgs[2];
    uint8_t subaddr[1] = { reg };

    msgs[0].addr = 0x30 >> 1;
    msgs[0].flags = I2C_M_NOSTART;
    msgs[0].buffer = subaddr;
    msgs[0].length = 1;

    msgs[1].addr = 0x30 >> 1;
    msgs[1].flags = I2C_M_READ;
    msgs[1].buffer = bufp;
    msgs[1].length = len;

    return BL618_I2C_TRANSMIT(msgs, 2);
}

int8_t tmp117_i2c_write(uint8_t *txbuf, uint16_t len)
{
    struct bflb_i2c_msg_s msgs[1];

    msgs[0].addr = TMP117_DeviceID1;
    msgs[0].flags = I2C_M_NOSTART;
    msgs[0].buffer = txbuf;
    msgs[0].length = len;

    return BL618_I2C_TRANSMIT(msgs, 1);
}

int8_t tmp117_i2c_read(uint8_t addr, uint8_t *rxbuf, uint16_t len)
{
    struct bflb_i2c_msg_s msgs[2];
    uint8_t subaddr[1] = { addr };

    msgs[0].addr = TMP117_DeviceID1;
    msgs[0].flags = I2C_M_NOSTART;
    msgs[0].buffer = subaddr;
    msgs[0].length = 1;

    msgs[1].addr = TMP117_DeviceID1;
    msgs[1].flags = I2C_M_READ;
    msgs[1].buffer = rxbuf;
    msgs[1].length = len;

    return BL618_I2C_TRANSMIT(msgs, 2);
}

/**********************************************************************************************************/
/*	        AFE4404_Trigger_HWReset         				                                              */
/**********************************************************************************************************/
void AFE4404_Trigger_HWReset(void)
{
    bflb_gpio_reset(gpio, AFE_RESETZ);
    bflb_mtimer_delay_us(40); // ~30us delay
    bflb_gpio_set(gpio, AFE_RESETZ);
    bflb_mtimer_delay_ms(10); // ~10ms delay with 16MHz clock
}

/**********************************************************************************************************/
/*	        AFE4404_Enable_HWPDN         				                                          		  */
/**********************************************************************************************************/
void AFE4404_Enable_HWPDN(void)
{
    bflb_gpio_reset(gpio, AFE_RESETZ);
    bflb_mtimer_delay_ms(10); // ~10ms delay with 16MHz clock
}

/**********************************************************************************************************/
/*	        AFE4404_Disable_HWPDN         				                                          		  */
/**********************************************************************************************************/
void AFE4404_Disable_HWPDN(void)
{
    bflb_gpio_set(gpio, AFE_RESETZ);
    bflb_mtimer_delay_ms(10); // ~10ms delay with 16MHz clock
}

/**********************************************************************************************************/
/* AFE4404_ADCRDY_Interrupt_Enable												                          */
/**********************************************************************************************************/
void AFE4404_ADCRDY_Interrupt_Enable(void)
{
    bflb_gpio_int_clear(gpio, AFE_ADC_DRDY);
    bflb_gpio_int_mask(gpio, AFE_ADC_DRDY, false); // Now Enable Pin 28 Interrupt For AFE4404
}

/**********************************************************************************************************/
/* AFE4404_ADCRDY_Interrupt_Disable												                          */
/**********************************************************************************************************/
void AFE4404_ADCRDY_Interrupt_Disable(void)
{
    bflb_gpio_int_clear(gpio, AFE_ADC_DRDY);
    bflb_gpio_int_mask(gpio, AFE_ADC_DRDY, true); // Now Disable Pin 28 Interrupt For AFE4404
}

/*********************************************************************************************************/
/* AFE4404_Reg_Write																	                 */
/*********************************************************************************************************/
void AFE4404_Reg_Write(unsigned char reg_address, unsigned long data)
{
    unsigned char configData[4];
    configData[0] = reg_address;
    configData[1] = (unsigned char)(data >> 16);
    configData[2] = (unsigned char)(((data & 0x00FFFF) >> 8));
    configData[3] = (unsigned char)(((data & 0x0000FF)));

    struct bflb_i2c_msg_s msgs[1];

    msgs[0].addr = AFE4404_I2C_DEFAULT_ADDRESS;
    msgs[0].flags = I2C_M_NOSTART;
    msgs[0].buffer = configData;
    msgs[0].length = 4;

    // bflb_i2c_transfer(i2c0, msgs, 1);
    BL618_I2C_TRANSMIT(msgs, 1);
}

/**********************************************************************************************************/
/* AFE4404_Reg_Read           					  			  											  */
/**********************************************************************************************************/
signed long AFE4404_Reg_Read(unsigned char Reg_address)
{
    unsigned char configData[3];
    signed long retVal;
    //    I2C_read(AFE4404_I2C_DEFAULT_ADDRESS, Reg_address, configData, 3);

    struct bflb_i2c_msg_s msgs[2];

    msgs[0].addr = AFE4404_I2C_DEFAULT_ADDRESS;
    msgs[0].flags = I2C_M_NOSTART;
    msgs[0].buffer = &Reg_address;
    msgs[0].length = 1;

    msgs[1].addr = AFE4404_I2C_DEFAULT_ADDRESS;
    msgs[1].flags = I2C_M_READ;
    msgs[1].buffer = configData;
    msgs[1].length = 3;

    // bflb_i2c_transfer(i2c0, msgs, 2);
    BL618_I2C_TRANSMIT(msgs, 2);

    retVal = configData[0];
    retVal = (retVal << 8) | configData[1];
    retVal = (retVal << 8) | configData[2];
    if (Reg_address >= 0x2A && Reg_address <= 0x2F) {
        if (retVal & 0x00200000) // check if the ADC value is positive or negative
        {
            retVal &= 0x003FFFFF; // convert it to a 22 bit value
            return (retVal ^ 0xFFC00000);
        }
    }
    return retVal;
}

/**********************************************************************************************************/
/*	        AFE4404_Enable_Read        	                  					  							  */
/**********************************************************************************************************/
void AFE4404_Enable_Read(void)
{
    unsigned char configData[4];
    configData[0] = CONTROL0;
    configData[1] = 0x00;
    configData[2] = 0x00;
    configData[3] = 0x01;

    struct bflb_i2c_msg_s msgs[1];

    msgs[0].addr = AFE4404_I2C_DEFAULT_ADDRESS;
    msgs[0].flags = I2C_M_NOSTART;
    msgs[0].buffer = configData;
    msgs[0].length = 4;

    BL618_I2C_TRANSMIT(msgs, 1);
    // bflb_i2c_transfer(i2c0, msgs, 1);
    // I2C_write(AFE4404_I2C_DEFAULT_ADDRESS, CONTROL0, configData, 3);
}

/**********************************************************************************************************/
/*	        AFE4404_Disable_Read       	                  					  							  */
/**********************************************************************************************************/
void AFE4404_Disable_Read(void)
{
    unsigned char configData[4];
    configData[0] = CONTROL0;
    configData[1] = 0x00;
    configData[2] = 0x00;
    configData[3] = 0x00;

    struct bflb_i2c_msg_s msgs[1];

    msgs[0].addr = AFE4404_I2C_DEFAULT_ADDRESS;
    msgs[0].flags = I2C_M_NOSTART;
    msgs[0].buffer = configData;
    msgs[0].length = 4;

    // bflb_i2c_transfer(i2c0, msgs, 1);
    BL618_I2C_TRANSMIT(msgs, 1);

    //    I2C_write(AFE4404_I2C_DEFAULT_ADDRESS, CONTROL0, configData, 3);
}

// /*
//  * @brief  platform init For M601
//  *           Now use M117 instead
//  */
// void M601_platformInit(void)
// {
// bflb_gpio_init(gpio, M601_OUT, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_DIS | GPIO_DRV_1);
// bflb_gpio_init(gpio, M601_IN, GPIO_INPUT | GPIO_FLOAT | GPIO_SMT_EN);
// }
//
// void M601_DQ_reset(void)
// {
// bflb_gpio_reset(gpio, M601_OUT);
// }
//
// void M601_DQ_set(void)
// {
// bflb_gpio_set(gpio, M601_OUT);;
// }
//
// bool M601_DQ_read(void)
// {
// return bflb_gpio_read(gpio, M601_IN);
// }

static tm_err_t layer_cb(tm_mdl_t *mdl, tml_head_t *lh)
{ //dump middle result
    int h = lh->out_dims[1];
    int w = lh->out_dims[2];
    int ch = lh->out_dims[3];
    mtype_t *output = TML_GET_OUTPUT(mdl, lh);
    return TM_OK;
    TM_PRINTF("Layer %d callback ========\n", mdl->layer_i);
#if 1
    for (int y = 0; y < h; y++) {
        TM_PRINTF("[");
        for (int x = 0; x < w; x++) {
            TM_PRINTF("[");
            for (int c = 0; c < ch; c++) {
#if TM_MDL_TYPE == TM_MDL_FP32
                TM_PRINTF("%.3f,", output[(y * w + x) * ch + c]);
#else
                TM_PRINTF("%.3f,", TML_DEQUANT(lh, output[(y * w + x) * ch + c]));
#endif
            }
            TM_PRINTF("],");
        }
        TM_PRINTF("],\n");
    }
    TM_PRINTF("\n");
#endif
    return TM_OK;
}

static void parse_output(tm_mat_t *outs, uint8_t dim)
{
    tm_mat_t out = outs[0];
    float *data = out.dataf;
    float maxp = 0;
    float sump = 0;
    int maxi = -1;
    for (int i = 0; i < dim; i++) {
        // printf("%d: %.3f\n", i, data[i]);
        sump += data[i];
        if (data[i] > maxp) {
            maxi = i;
            maxp = data[i];
        }
    }
    for (int i = 0; i < dim; i++) {
        printf("%d: %.3f\n", i, data[i] / sump);
    }
    if (dim == 2) {
        xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
        ui_UpdateMentalLabel(maxi, data[maxi] / sump);
        ui_UpdateMdl2(data[0] / sump, data[1] / sump);
        xSemaphoreGive(xMutex_lvgl);
        output_class = maxi;
    } else if (dim == 3) {
        xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
        ui_UpdateMdl1(data[0] / sump, data[1] / sump, data[2] / sump);
        xSemaphoreGive(xMutex_lvgl);
    }
    TM_PRINTF("### Predict output is: Number %d, prob %.3f\n", maxi, maxp / sump);
    return;
}

void sntp_set_time(uint32_t sntp_time)
{
    bflb_rtc_disable(rtc);
    time_base = (long long)sntp_time;
    bflb_rtc_set_time(rtc, BFLB_RTC_SEC2TIME(1));
    printf("TIME SET\r\n");
}

static void EDA_task(void *pvParameters)
{
    uint32_t temp;
    // float epsilon = 1;
    // float dMin = 1.25 * 8;
    // float thMin = 0.025;
    // float SCL[320] = { 0 };
    // float SCR[320] = { 0 };
    // float MSE[320] = { 0 };
    uint8_t first_loop = 1;
    csi_fir_instance_f32 S_EDA;
    uint16_t numTaps = 31;
    uint16_t i;
    float32_t coef_fir[33] = {
        -0.035227631774027,
        -0.011403290059329,
        -0.011278657115407,
        -0.009526620943612,
        -0.005938073254900,
        -3.522182366573807e-04,
        0.007169927421241,
        0.016457560822460,
        0.027192182443781,
        0.038888032983133,
        0.050978558586626,
        0.062780427590412,
        0.073636288745708,
        0.082865718623332,
        0.089924801523552,
        0.094338035120237,
        0.095851561929052,
        0.094338035120237,
        0.089924801523552,
        0.082865718623332,
        0.073636288745708,
        0.062780427590412,
        0.050978558586626,
        0.038888032983133,
        0.027192182443781,
        0.016457560822460,
        0.007169927421241,
        -3.522182366573807e-04,
        -0.005938073254900,
        -0.009526620943612,
        -0.011278657115407,
        -0.011403290059329,
        -0.035227631774027,
    };
    float32_t state_fir[33 + 120 - 1];
    printf("EDA task enter \r\n");
    // csi_fir_decimate_init_f32(&S_EDA, 33, 2, coef_fir, state_fir, 240); // NumTaps = 33; BolckSize = 40;
    csi_fir_init_f32(&S_EDA, 33, coef_fir, state_fir, 120); // NumTaps = 33; BolckSize = 40;
    printf("EDA task start \r\n");

    /* AD5940 INIT */
    AD5940_HWReset();
    vTaskDelay(1000);
    printf("AD5940_HWReset\r\n");

    // taskENTER_CRITICAL();
    AD5940_Initialize();
    // taskEXIT_CRITICAL();
    vTaskDelay(10);
    // int32_t i = AD5940_ReadReg(REG_AFECON_CHIPID);
    // printf("CHIPID read:0x%04x.\r\n", i);
    printf("AD5940_Initialize\r\n");
    AD5940PlatformCfg();
    vTaskDelay(10);
    printf("AD5940PlatformCfg\r\n");
    AD5940EDAStructInit();
    vTaskDelay(10);
    ResistorForBaseline = 50400;
    /* END AD5940 INIT */

    xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
    lv_obj_clear_flag(ui_AD5940, LV_OBJ_FLAG_HIDDEN);
    xSemaphoreGive(xMutex_lvgl);
    err_code_ad5940 = AppEDAInit(AppBuff, APPBUFF_SIZE); /* Initialize BIA(EDA) application. Provide a buffer, which is used to store sequencer commands */
    xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
    lv_obj_add_flag(ui_AD5940, LV_OBJ_FLAG_HIDDEN);
    xSemaphoreGive(xMutex_lvgl);
    printf("AppEDAInit:%d\r\n", err_code_ad5940);

    err_code_ad5940 = AppEDACtrl(APPCTRL_START, 0); /* Control BIA(EDA) measurement to start. Second parameter has no meaning with this command. */
    printf("APPCTRL_START:%d\r\n", err_code_ad5940);

    err_code_ad5940 = AppEDACtrl(EDACTRL_SETBASE, &EDABase);
    printf("EDACTRL_SETBASE:%d\r\n", err_code_ad5940);

    printf("EDA Started, begin to loop %s\r\n", __func__);

    while (1) {
        // printf("Enter EDA_task while\r\n");
        /* Check if interrupt flag which will be set when interrupt occurred. */
        if (AD5940_GetMCUIntFlag()) {
            AD5940_ClrMCUIntFlag(); /* Clear this flag */

            temp = APPBUFF_SIZE;

            // taskENTER_CRITICAL();
            err_code_ad5940 = AppEDAISR(AppBuff, &temp); /* Deal with it and provide a buffer to store data we got */
            printf("AppEDAISR:%d\r\n", err_code_ad5940);
            // taskEXIT_CRITICAL();
            EDAShowResult(AppBuff, temp); /* Show the results to UART */
            if (EDAcnt >= 30 * 4) {
                // if (first_loop) {
                //     for (i = 0; i < 2; i++) {
                //         csi_fir_f32(&S_EDA, EDAwindow + (i * 40), EDAwindow + (i * 40), 40);
                //     }
                // }
                // for (i = 2; i < 8; i++) {
                //     csi_fir_f32(&S_EDA, EDAwindow + (i * 40), EDAwindow + (i * 40), 40);
                // }
                // // for (i=0;i<320;i++){c
                // //     EDAwindow[i] = EDAwindow[i+16];
                // // }
                // if (first_loop) {
                //     for (i = 0; i < 80; i++) {
                //         printf("{FilteredConductance}%.3f\r\n", EDAwindow[i]);
                //     }
                //     first_loop = 0;
                // }
                // for (i = 80; i < 320; i++) {
                //     printf("{FilteredConductance}%.3f\r\n", EDAwindow[i]);
                // }

                // for (i = 0; i < 2; i++) {
                //     csi_fir_f32(&S_EDA, EDAwindow_o + (i * 40), EDAwindow_f + (i * 40), 40);
                // }

                // for (i = 0; i < 120; i++) {
                //     EDAwindow_f[i] = EDAwindow_o[i];
                // }
                xSemaphoreTake(xMutex_DSP, portMAX_DELAY);
                csi_fir_f32(&S_EDA, EDAwindow_o, EDAwindow_f, 120);
                xSemaphoreGive(xMutex_DSP);

                for (i = 0; i < 120; i++) {
                    printf("{FilteredConductance}%.3f\r\n", EDAwindow_f[i]);
                }

                // sparsEDA(EDAwindow, 8, 320, epsilon, 40, dMin, thMin, SCR, SCL, MSE);
                // EDAcnt = 10 * 8;
                EDAcnt = 0;
                // scr_transfer_pointer = 0;
                // for (i = 0; i < 30 * 8; i++) {
                //     printf("{SCR}%.3f\r\n", SCR[i]);
                //     // if (SCR[i] != 0.0) {
                //     //     SCR_nonezero[scr_transfer_pointer] = SCR[i];
                //     //     SCR_nonezero_timepoint[scr_transfer_pointer] =
                //     // }
                // }
                eda_ready_flag = 1;
                // for (i = 0; i < 10 * 8; i++) {
                //     EDAwindow[i] = EDAwindow[30 * 8 + i];
                // }
                // printf("\r\n");
            }
            // printf("EDA NUMBER:%d\r\n", number);
        }
        vTaskDelay(1);
    }

    vTaskDelete(NULL);
}

static void ACCE_task(void *pvParameters)
{
    printf("ACCE task enter \r\n");
    vTaskDelay(10);

    /* LIS2DH12 INIT*/
    /* Initialize mems driver interface */
    dev_ctx.write_reg = lis2dh12_platform_write;
    dev_ctx.read_reg = lis2dh12_platform_read;
    /* Wait sensor boot time */
    vTaskDelay(50);
    /* Check device ID */
    lis2dh12_device_id_get(&dev_ctx, &whoamI);
    printf("LIS2DH12 Device ID: 0x%x\r\n", whoamI);
    /* Set FIFO watermark to FIFO_WATERMARK */
    lis2dh12_fifo_watermark_set(&dev_ctx, ACC_FIFO_WATERMARK - 1);
    /* Set FIFO mode to Stream mode (aka Continuous Mode) */
    lis2dh12_fifo_mode_set(&dev_ctx, LIS2DH12_DYNAMIC_STREAM_MODE);
    /* Enable FIFO */
    lis2dh12_fifo_set(&dev_ctx, PROPERTY_ENABLE);
    /* Enable Block Data Update. */
    lis2dh12_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
    /* Set Output Data Rate to 1Hz. */
    lis2dh12_data_rate_set(&dev_ctx, LIS2DH12_ODR_25Hz);
    /* Set full scale to 2g. */
    lis2dh12_full_scale_set(&dev_ctx, LIS2DH12_4g);
    /* Set device in continuous mode with 8 bit resol. */
    lis2dh12_operating_mode_set(&dev_ctx, LIS2DH12_LP_8bit);
    /* END LIS2DH12 INIT*/

    printf("ACCE task start \r\n");
    printf("begin to loop %s\r\n", __func__);

    uint32_t number = 0;
    static uint8_t dummy;
    int32_t ret_code;
    acceWindowPtr = 0;
    stepCnt = 0;

    while (1) {
        /* Check if FIFO level over threshold */
        ret_code = lis2dh12_fifo_fth_flag_get(&dev_ctx, &dummy);
        // printf("dummy: %d , ret code: %d\r\n", dummy, ret_code);
        // printf("d %d", dummy);

        if (dummy) {
            number++;

            /* Read number of sample in FIFO */
            lis2dh12_fifo_data_level_get(&dev_ctx, &dummy);

            while (dummy > 0) {
                /* Read XL samples */
                lis2dh12_acceleration_raw_get(&dev_ctx, data_raw_acceleration);
                acceleration_mg[0] =
                    lis2dh12_from_fs2_hr_to_mg(data_raw_acceleration[0]);
                acceleration_mg[1] =
                    lis2dh12_from_fs2_hr_to_mg(data_raw_acceleration[1]);
                acceleration_mg[2] =
                    lis2dh12_from_fs2_hr_to_mg(data_raw_acceleration[2]);
                // printf("%d - Acceleration [mg]:%4.2f\t%4.2f\t%4.2f\r\n",
                //        ACC_FIFO_WATERMARK - dummy,
                //        acceleration_mg[0], acceleration_mg[1], acceleration_mg[2]);

                acceWindow[acceWindowPtr] = acceleration_mg[0];
                acceWindow[acceWindowPtr + 1] = acceleration_mg[1];
                acceWindow[acceWindowPtr + 2] = acceleration_mg[2];
                acceWindowPtr += 3;

                xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
                ui_UpdateAccelerationChart(acceleration_mg[0] * 0.016, acceleration_mg[1] * 0.016, acceleration_mg[2] * 0.016);
                xSemaphoreGive(xMutex_lvgl);

                dummy--;
            }
            if (acceWindowPtr >= 300) {
                printf("ACCE Step Begin\r\n");
                stepCnt += count_steps(acceWindow);
                acceWindowPtr = 0;
                // printf("ACCE NUMBER: %d\r\n", stepCnt);
                xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
                ui_UpdateStepLabel(stepCnt);
                xSemaphoreGive(xMutex_lvgl);
            }
        }

        // printf("ACCE\r\n");
        vTaskDelay(20);
    }

    vTaskDelete(NULL);
}

static void TEMP_task(void *pvParameters)
{
    printf("TEMP task enter \r\n");
    vTaskDelay(10);
    printf("TEMP task start \r\n");
    printf("begin to loop %s\r\n", __func__);

    uint32_t number = 0;
    tmp117_Initialization();
    uint16_t chipid;
    tmp117_get_chipid(&chipid);
    printf("TMP117 CHIPID: 0x%x", chipid);
    tmp117_StartConv();

    while (1) {
        // printf("tmp117 data ready %d\r\n", tmp117_DataReady());
        if (tmp117_DataReady()) {
            tmp117_get_tempurature(&Tempurature);
            // printf("Tempurature: %.3f, Number: %d\r\n", Tempurature, number);
            xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
            ui_UpdateTempuratureChart(Tempurature);
            ui_UpdateTempLabel(Tempurature);
            xSemaphoreGive(xMutex_lvgl);
            TEMPwindow[tw_pointer] = Tempurature;
            tw_pointer++;
            if (tw_pointer >= 120) {
                for (int i = 0; i < 119; i++) {
                    TEMPwindow[i] = TEMPwindow[i + 1];
                }
                tw_pointer--;
            }
            // if (wifi_connected && !(number % 8)) {
            //     // printf("1");
            //     onenet_transfer(Tempurature);
            // }
            tmp117_StartConv();
            // tmp117_StartConv();
            number++;
        }
        // printf("TEMP\r\n");
        vTaskDelay(200); // 4Hz TMP117单次采样的时间为125ms （8点平均）
    }

    vTaskDelete(NULL);
}

static void HR_task(void *pvParameters)
{
    printf("HR task enter \r\n");
    vTaskDelay(10);
    printf("HR task start \r\n");
    printf("begin to loop %s\r\n", __func__);

    uint32_t NUMBER = 0;
    uint8_t uiUpdateCntHr = 0;

    /* AFE4404 INIT */
    initStatHRM();
    // AFE4404_RESETZ_Init(); // Included in AFE4404_PinInit
    AFE4404_Trigger_HWReset();
    AFE4404_Init();
    /* END AFE4404 INIT */

    /* AFE4404 */
    offsetDACcalibFlag = 1;
    afe4404_adcrdy = 0;
    AFE4404_ADCRDY_Interrupt_Enable();

    CALIBRATION_ENABLED = 1;
    AFE4404_Reg_Write(AFE_CONTROL0, 0x000000); // write mode
    AFE4404_Reg_Init();
    // AFE4404_Reg_Write(AFE_CONTROL0,0x000000); //write mode
    // AFE4404_Reg_Write(AFE_EXT_CLK_DIV_REG,0x04);      // CLKDIV_EXTMODE
    AFE4404_Reg_Write(AFE_CONTROL0, 0x000001); // read mode
    initCalibrationRoutine();

    while (1) {
        if (offsetDACcalibFlag == 0) {
            // printf("1\r\n");
            if (afe4404_adcrdy) {
                // printf("2\r\n");
                afe4404_adcrdy = 0;
                offsetDACcalibFlag = OFFSET_DAC_Code_Est(AFE4404_Reg_Read(45));
                // printf("offsetDACcalibFlag %d\r\n", offsetDACcalibFlag);
                if (offsetDACcalibFlag)
                    AFE4404_ADCRDY_Interrupt_Disable(); // Disable DRDY interrupt
            }
        } else {
            offsetDACcalibFlag = 1;
            if (afe4404_adcrdy) { // 读取数据
                afe4404_adcrdy = 0;
                AFE44xx_SPO2_Data_buf[0] = AFE4404_Reg_Read(42); //read LED2 Data
                AFE44xx_SPO2_Data_buf[1] = AFE4404_Reg_Read(43); //read LED3 data
                AFE44xx_SPO2_Data_buf[2] = AFE4404_Reg_Read(44); //read LED1 Data
                AFE44xx_SPO2_Data_buf[3] = AFE4404_Reg_Read(45); //read Ambient Data
                AFE44xx_SPO2_Data_buf[4] = AFE4404_Reg_Read(46); //read LED2 - LED3 Data
                AFE44xx_SPO2_Data_buf[5] = AFE4404_Reg_Read(47); //read LED1 - Ambient Data

                // printf("READ DATA\r\n");

                if (CALIBRATION_ENABLED == 1) { // 如果开启校准，进入校准过程
                    if (Calibration == 1) {     // 如果校准没有结束
                        if (LED_Sel == 2)
                            CalibrateAFE4404(AFE44xx_SPO2_Data_buf[0], AFE44xx_SPO2_Data_buf[3]);
                        else if (LED_Sel == 3)
                            CalibrateAFE4404(AFE44xx_SPO2_Data_buf[1], AFE44xx_SPO2_Data_buf[3]);
                        else // Default LED_Sel = 1
                            CalibrateAFE4404(AFE44xx_SPO2_Data_buf[2], AFE44xx_SPO2_Data_buf[3]);
                    }

                    prfcount++;
                    if (prfcount == 100) {
                        g_OneSecondFlag = 1;
                        prfcount = 0;
                    }
                }
                if (captureMode == FINITE) { // 有限采样模式
                    sampleCount++;
                    if (sampleCount == totalCount) {
                        sampleCount = 0;
                        totalCount = 1;
                        AFE4404_ADCRDY_Interrupt_Disable(); // 采到有限的点后停止采样
                        captureMode = FINITE;
                        captureInProgressFlag = 0;
                        // P5OUT &= ~BIT0; //Turn off LED P5.0 (Green)
                    }
                }
                sendDataFlag = 1; // 数据采集完成发送数据
            }
        }

        if (sendDataFlag) {
            // printf("Start Send Data\r\n");
            xSemaphoreTake(xMutex_interArrayWindow, portMAX_DELAY);
            statHRMAlgo(AFE44xx_SPO2_Data_buf[5]);
            xSemaphoreGive(xMutex_interArrayWindow);
            // Overwrite LED2-AMB2 with Heart rate
            hr_transfer += HeartRate;

            sendDataFlag = 0;

            // printf("{LED2Data} %d\n", AFE44xx_SPO2_Data_buf[0]);
            // printf("{LED3Data} %d\n", AFE44xx_SPO2_Data_buf[1]);
            // printf("{LED1Data} %d\n", AFE44xx_SPO2_Data_buf[2]);
            // printf("{AmbientData} %d\n", AFE44xx_SPO2_Data_buf[3]);

            // // if (CALIBRATION_ENABLED) {
            //     printf("CALIBRATION_ENABLED\r\n");
            // } else {
            // printf("{HRData}%d\n", AFE44xx_SPO2_Data_buf[4]);
            if (NUMBER >= 200) {
                hr_transfer /= 200;
                hr_onenet = hr_transfer;
                xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
                ui_UpdateHRLabel(hr_transfer);
                xSemaphoreGive(xMutex_lvgl);
                NUMBER = 0;
                hr_transfer = 0;
            }
            if (uiUpdateCntHr >= 5) {
                xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
                ui_UpdatePPGChart(AFE44xx_SPO2_Data_buf[5], 0, 0);
                xSemaphoreGive(xMutex_lvgl);
                uiUpdateCntHr = 0;
            }
            NUMBER++;
            uiUpdateCntHr++;

            // printf("{LED1AMBData}%d\n", AFE44xx_SPO2_Data_buf[5]);

            // bflb_mtimer_delay_ms(250);
        }

        if (captureInProgressFlag == 0) { // Start process again
            captureInProgressFlag = 1;
            totalCount = 1;

            sampleCount = 0;

            if (sampleCount) {
                captureMode = FINITE;
                totalCount = sampleCount;
            } else
                captureMode = CONTINUOUS;
            sampleCount = 0;
            AFE4404_ADCRDY_Interrupt_Enable(); // Enable DRDY interrupt

            afe4404_adcrdy = 0;
            sendDataFlag = 0;
            AFE44xx_SPO2_Data_buf[0] = 0;
            AFE44xx_SPO2_Data_buf[1] = 0;
            AFE44xx_SPO2_Data_buf[2] = 0;
            AFE44xx_SPO2_Data_buf[3] = 0;
            AFE44xx_SPO2_Data_buf[4] = 0;
            AFE44xx_SPO2_Data_buf[5] = 0;
        }
        vTaskDelay(1);
    }

    vTaskDelete(NULL);
}

static void LVGL_task(void *pvParameters)
{
    printf("LVGL task enter \r\n");
    vTaskDelay(10);
    printf("LVGL task start \r\n");
    printf("begin to loop %s\r\n", __func__);

    /* lvgl init */
    lv_log_register_print_cb(lv_log_print_g_cb);
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    ui_init();

    while (1) {
        xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
        if (wifi_connected) {
            ui_setWifiImg(1);
        } else {
            ui_setWifiImg(0);
        }
        lv_task_handler();
        xSemaphoreGive(xMutex_lvgl);
        vTaskDelay(2);
    }
    vTaskDelete(NULL);
}

static void CLOCK_task(void *pvParameters)
{
    printf("CLOCK task enter \r\n");
    vTaskDelay(10);
    bflb_rtc_set_time(rtc, BFLB_RTC_SEC2TIME(1));
    long long rtc_sec = 0;
    uint8_t rtc_sec0 = 0;
    uint8_t rtc_min = 0;
    uint8_t rtc_hour = 0;
    uint8_t rtc_mon = 0;
    uint8_t rtc_date = 0;
    uint8_t rtc_week = 7;
    uint16_t rtc_year = 0;
    // bflb_timestamp_t info;
    struct tm *info;
    

    printf("CLOCK task start \r\n");
    while (1) {
        // printf("time:%lld\r\n", BFLB_RTC_TIME2SEC(bflb_rtc_get_time(rtc)));
        rtc_sec = BFLB_RTC_TIME2SEC(bflb_rtc_get_time(rtc)) + time_base + GMTp8;
        info = localtime(&rtc_sec);
        // bflb_timestamp_utc2time(rtc_sec, &info);
        // printf("%s\r\n", asctime(info));

        if (info->tm_sec != rtc_sec0) {
            rtc_sec0 = info->tm_sec;
            xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
            ui_UpdateSecLabel(info->tm_sec);
            xSemaphoreGive(xMutex_lvgl);
        }

        if (info->tm_min != rtc_min) {
            rtc_min = info->tm_min;
            xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
            ui_UpdateMinLabel(rtc_min);
            xSemaphoreGive(xMutex_lvgl);
        }

        if (info->tm_hour != rtc_hour) {
            rtc_hour = info->tm_hour;
            xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
            ui_UpdateHourLabel(rtc_hour);
            xSemaphoreGive(xMutex_lvgl);
        }

        if (info->tm_mday != rtc_date) {
            rtc_date = info->tm_mday;
            xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
            ui_UpdateDateLabel(rtc_date);
            xSemaphoreGive(xMutex_lvgl);
        }

        if (info->tm_mon != rtc_mon) {
            rtc_mon = info->tm_mon;
            xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
            ui_UpdateMonLabel(rtc_mon + 1);
            xSemaphoreGive(xMutex_lvgl);
        }

        if (info->tm_year != rtc_year) {
            rtc_year = info->tm_year;
            xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
            ui_UpdateYearLabel(rtc_year + 1900);
            xSemaphoreGive(xMutex_lvgl);
        }

        if (info->tm_wday != rtc_week) {
            rtc_week = info->tm_wday;
            xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
            ui_UpdateWeekLabel(rtc_week);
            xSemaphoreGive(xMutex_lvgl);
        }

        vTaskDelay(250);
    }
    vTaskDelete(NULL);
}

static void WIFI_task(void *pvParameters)
{
    uint8_t weatherCounter = 6;
    char weatherCityName[10] = { 0 };
    char weatherText[10] = { 0 };
    uint8_t weatherCode = 100;
    char weatherTemp[10] = { 0 };

    printf("WIFI task start \r\n");

    wifi_mgmr_sta_quickconnect("cgh1", "asdfghjk", 2400, 5000);

    while (1) {
        if (!wifi_connected) {
            wifi_mgmr_sta_quickconnect("cgh1", "asdfghjk", 2400, 5000);
            vTaskDelay(10000);
        } else {
            onenet_transfer(Tempurature, hr_onenet, output_class);
            // if(eda_transfer_flag){
            //     eda_transfer_flag = 0;
            //     onenet_transfer_GSR(EDAwindow_transfer_filtered, 240);
            // }

            if (weatherCounter >= 6) {
                memset(weatherCityName, 0, 10);
                memset(weatherText, 0, 10);
                memset(weatherTemp, 0, 10);
                weather_get("nanjing", weatherCityName, weatherText, &weatherCode, weatherTemp);
                // weather_get("ip", weatherCityName, weatherText, &weatherCode, weatherTemp);
                xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
                ui_updateWeather(weatherCityName, weatherText, weatherCode, weatherTemp);
                xSemaphoreGive(xMutex_lvgl);
                weatherCounter = 0;
            }
            vTaskDelay(10000);
            weatherCounter++;
        }
    }
    vTaskDelete(NULL);
}

static void Algo_task(void *pvParameters)
{
    printf("Algo task enter \r\n");
    vTaskDelay(20);

    printf("Algo task start \r\n");
    vTaskDelay(5000);
    // float e[120];
    // float p[120];
    // float r[120];
    // float t[120];
    // float obj;
    // float EDA_STD = 0;
    // float EDA_mean = 0;
    // f_sparse l;
    // g_sparse d;

    /* EDA Feature */
    float EDA_mean = 0;
    float EDA_std = 0;
    float EDA_min = 0;

    emxArray_real32_T *z;

    float distance_tmp = 4.0f * 4;
    int EDA_peakNum = 0;
    float EDA_peakHeightAvg = 0.0f;
    float EDA_peakHeightMin = 1e6;
    float EDA_peakHeightMax = 1e-6;
    float EDA_peakHeightStd = 0.0f;
    float temp = 0;

    /* HR Feature */
    float HRM_max = 0;
    float HRM_min = 0;
    float HRM_std = 0;
    float HRM_mean = 0;
    uint index = 0;

    /* TEMP Feature */
    float TEMP_max = 0;
    float TEMP_min = 0;
    float TEMP_avg = 0;

    float input_data[15] = { 1.6, 1.0, 1.0, 1.0, 1.0,
                             1.7, 1.9, 1.2, 1.0, 1.0,
                             1.1, 0.5, 1.0, 1.0, 1.0 };

    // float input_data[15] = { EDA_mean, EDA_std, EDA_min, EDA_peakNum, EDA_peakHeightAvg,
    //                          EDA_peakHeightMin, EDA_peakHeightMax, EDA_peakHeightStd, HRM_max, HRM_min,
    //                          HRM_std, HeartRate, TEMP_avg, TEMP_max, TEMP_min };

    TM_DBGT_INIT();
    TM_PRINTF("tinymaix start\n");
    tm_mdl_t mdl;
    tm_err_t res;
    tm_mdl_t mdl_bin;
    // tm_mat_t in = { 1, 1, 1, 15, { NULL } };
    tm_mat_t outs[1];
    tm_mat_t outs_bin[1];
    tm_mat_t in = { 1, 1, 1, 15, { NULL } };
    tm_mat_t in_bin = { 1, 1, 1, 13, { NULL } };

    tm_stat((tm_mdlbin_t *)mdl_data);
    tm_stat((tm_mdlbin_t *)mdl_data_bin);

    res = tm_load(&mdl, mdl_data, NULL, layer_cb, &in);
    if (res != TM_OK) {
        TM_PRINTF("tm model load err %d\n", res);
    }
    res = tm_load(&mdl_bin, mdl_data_bin, NULL, layer_cb, &in_bin);
    if (res != TM_OK) {
        TM_PRINTF("tm bin model load err %d\n", res);
    }

    emxInitArray_real32_T(&z, 1);
    uint16_t temp_min = 0;
    uint16_t temp_max = 0;
    while (1) {
        if (eda_ready_flag) {
            //     // float b_dv[120];
            //     emxInit_sparse(&l);
            //     emxInit_sparse1(&d);
            //     printf("Enter cvxEDA\r\n");
            //     /* Initialize function 'cvxEDA' input arguments. */
            //     /* Initialize function input argument 'y'. */
            //     /* Call the entry-point 'cvxEDA'. */
            //     EDA_STD = 0;
            //     EDA_mean = 0;
            //     for (int i = 0; i < 120; i++) {
            //         EDA_mean += EDAwindow_f[i];
            //     }
            //     EDA_mean = EDA_mean / 120;
            //     printf("EDA_mean %.3f\r\n", EDA_mean);
            //     for (int i = 0; i < 120; i++) {
            //         EDA_STD += (EDAwindow_f[i] - EDA_mean) * (EDAwindow_f[i] - EDA_mean);
            //     }
            //     EDA_STD = sqrtf(EDA_STD/120);
            //     printf("EDA_STD %.3f\r\n", EDA_STD);
            //     for (int i = 0; i < 120; i++) {
            //         printf("{EDAwindow_f_o} %.3f\r\n", EDAwindow_f[i]);
            //         EDAwindow_f[i] = (EDAwindow_f[i] - EDA_mean) / EDA_STD;
            //         printf("{EDAwindow_f} %.3f\r\n", EDAwindow_f[i]);
            //     }
            //     cvxEDA(EDAwindow_f, 0.25, r, p, t, &l, &d, e, &obj);
            //     emxDestroy_sparse1(d);
            //     emxDestroy_sparse(l);
            //     for (int i = 0; i < 120; i++) {
            //         printf("{cvxEDA_p} %.3f\r\n", p[i]);
            //     }
            //     for (int i = 0; i < 120; i++) {
            //         printf("{cvxEDA_r} %.3f\r\n", r[i]);
            //     }
            //     for (int i = 0; i < 120; i++) {
            //         printf("{cvxEDA_t} %.3f\r\n", t[i]);
            //     }
            //     printf("Exit cvxEDA\r\n");
            //     // cvxEDA_terminate();
            //     eda_ready_flag = 0;

            EDA_min = 1e6;
            EDA_mean = 0;
            for (uint8_t i = 0; i < 120; i++) {
                if (EDAwindow_f[i] < EDA_min) {
                    EDA_min = EDAwindow_f[i];
                }
                EDA_mean += EDAwindow_f[i];
            }
            EDA_mean /= 120;
            EDA_std = 0;
            for (uint8_t i = 0; i < 120; i++) {
                EDA_std += (EDAwindow_f[i] - EDA_mean) * (EDAwindow_f[i] - EDA_mean);
            }
            EDA_std = sqrtf(EDA_std / 120);

            // csi_mean_f32(EDAwindow_f, 120, &EDA_mean);
            // csi_std_f32(EDAwindow_f, 120, &EDA_std);
            // csi_std_f32(EDAwindow_f, 120, &EDA_min);
            printf("EDA_min %.3f\r\n", EDA_min);

            my_findpeaks(EDAwindow_f, distance_tmp, 120, z);
            EDA_peakHeightAvg = 0.0f;
            EDA_peakHeightMin = 1e6;
            EDA_peakHeightMax = 1e-6;
            EDA_peakHeightStd = 0.0f;
            temp = 0;

            for (uint8_t i = 0; i < 120; i++) {
                temp = z->data[i];
                if (temp > 1e-3) {
                    EDA_peakNum++;
                    EDA_peakHeightAvg += temp;
                    if (temp > EDA_peakHeightMax) {
                        EDA_peakHeightMax = temp;
                    }
                    if (temp < EDA_peakHeightMin) {
                        EDA_peakHeightMin = temp;
                    }
                }
            }
            EDA_peakHeightAvg /= EDA_peakNum;
            for (uint8_t i = 0; i < 120; i++) {
                temp = z->data[i];
                if (temp > 1e-3) {
                    EDA_peakHeightStd += (temp - EDA_peakHeightAvg) * (temp - EDA_peakHeightAvg);
                }
            }
            EDA_peakHeightStd = sqrtf(EDA_peakHeightStd / EDA_peakNum);
            // emxDestroyArray_real32_T(z);
            printf("EDA_max %.3f\r\n", EDA_peakHeightMax);

            temp_min = (uint16_t)floorf(EDA_min * 1000);
            temp_max = (uint16_t)ceilf(EDA_peakHeightMax * 1000);
            // printf("(uint16_t)ceilf(EDA_peakHeightMax * 100) %d\r\n", (uint16_t)ceilf(EDA_peakHeightMax * 1000));
            // printf("(uint16_t)floorf(EDA_min * 100) %d\r\n", (uint16_t)floorf(EDA_min * 1000));

            HRM_min = 1e6;
            HRM_max = 1e-6;
            HRM_std = 0.0f;
            HRM_mean = 0;

            xSemaphoreTake(xMutex_interArrayWindow, portMAX_DELAY);

            for (uint8_t i = 0; i < iA_pointer; i++) {
                if ((float)interArrayWindow[i] < HRM_min) {
                    HRM_min = (float)interArrayWindow[i];
                }
                if ((float)interArrayWindow[i] > HRM_max) {
                    HRM_max = (float)interArrayWindow[i];
                }
                HRM_mean += (float)interArrayWindow[i];
            }
            HRM_mean /= iA_pointer;

            for (uint8_t i = 0; i < iA_pointer; i++) {
                HRM_std += ((float)interArrayWindow[i] - HRM_mean) * ((float)interArrayWindow[i] - HRM_mean);
            }
            HRM_std = sqrtf(HRM_std / iA_pointer);

            xSemaphoreGive(xMutex_interArrayWindow);
            printf("interArrayWindow\r\n");

            // csi_max_f32(interArrayWindow, iA_pointer, &HRM_max, &index);
            // csi_min_f32(interArrayWindow, iA_pointer, &HRM_min, &index);
            // csi_std_f32(interArrayWindow, iA_pointer, &HRM_std);

            // csi_max_f32(TEMPwindow, 120, &TEMP_max, &index);
            // csi_min_f32(TEMPwindow, 120, &TEMP_min, &index);
            // csi_mean_f32(TEMPwindow, 120, &TEMP_avg);

            TEMP_min = 1e6;
            TEMP_max = 1e-6;
            TEMP_avg = 0;
            for (uint8_t i = 0; i < 120; i++) {
                if (TEMPwindow[i] < TEMP_min) {
                    TEMP_min = TEMPwindow[i];
                }
                if (TEMPwindow[i] > TEMP_max) {
                    TEMP_max = TEMPwindow[i];
                }
                TEMP_avg += TEMPwindow[i];
            }
            TEMP_avg /= 120;

            printf("TEMPwindow\r\n");

            input_data[0] = (EDA_mean - 1.9635107565854033f) / 2.653874198543756f;
            input_data[1] = (EDA_std - 0.04519523431581202f) / 0.0796896165882909f;
            input_data[2] = (EDA_min - 1.868224049678013f) / 2.51363460423691f;
            input_data[3] = (EDA_peakNum - 4.0f) / 1.5f;
            input_data[4] = (EDA_peakHeightAvg - 1.9726016026110904f) / 2.6678769175713786f;
            input_data[5] = (EDA_peakHeightMin - 1.9058191315547377f) / 2.5812927451430987f;
            input_data[6] = (EDA_peakHeightMax - 2.060425913523459f) / 2.7624762331501787f;
            input_data[7] = (EDA_peakHeightStd - 0.04304164967248956f) / 0.07752566272303067f;
            input_data[8] = (HRM_max - 88.47838086476541f / 64 * 100) / 20.74675891492638f / 64 * 100;
            input_data[9] = (HRM_min - 23.879484820607175f / 64 * 100) / 6.484658316275455f / 64 * 100;
            input_data[10] = (HRM_std - 14.116718579677409f) / 4.923849396461889f;
            input_data[11] = (HeartRate - 81.20699172033119f) / 12.742457294618461f;
            input_data[12] = (TEMP_avg - 33.80643054277829f) / 0.017385464484055594f;
            input_data[13] = (TEMP_max - 33.80643054277829f) / 0.017385464484055594f;
            input_data[14] = (TEMP_min - 33.80643054277829f) / 0.017385464484055594f;
            printf("Feature Done\r\n");

            tm_mat_t in_float;
            in_float.dims = 1;
            in_float.h = 1;
            in_float.w = 1;
            in_float.c = 15;
            in_float.dataf = input_data;
            tm_mat_t in_float_bin;
            in_float_bin.dims = 1;
            in_float_bin.h = 1;
            in_float_bin.w = 1;
            in_float_bin.c = 15;
            in_float_bin.dataf = input_data;

            res = tm_preprocess(&mdl, TMPP_FP2INT, &in_float, &in);
            res = tm_preprocess(&mdl_bin, TMPP_FP2INT, &in_float_bin, &in_bin);

            TM_DBGT_START();
            res = tm_run(&mdl_bin, &in_bin, outs_bin);
            TM_DBGT("tm_run_bin");
            if (res == TM_OK)
                parse_output(outs_bin, 2);
            else
                TM_PRINTF("tm bin run error: %d\n", res);

            TM_DBGT_START();
            res = tm_run(&mdl, &in, outs);
            TM_DBGT("tm_run");
            if (res == TM_OK)
                parse_output(outs, 3);
            else
                TM_PRINTF("tm run error: %d\n", res);

            eda_ready_flag = 0;

            xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
            ui_UpdateEDAChartRange(temp_max, temp_min);
            ui_UpdateEDAChart(EDAwindow_f, 120);
            ui_UpdateEda1(EDA_min, EDA_mean, EDA_std);
            ui_UpdateEda2(EDA_peakNum, EDA_peakHeightAvg);
            ui_UpdateEda3(EDA_peakHeightMin, EDA_peakHeightMax, EDA_peakHeightStd);
            ui_UpdateBvp(HRM_min, HRM_max, HRM_mean, HeartRate);
            ui_UpdateTemp(TEMP_avg);
            xSemaphoreGive(xMutex_lvgl);

            printf("EdaChartUpdate\r\n");
        }
        vTaskDelay(250);
    }
    vTaskDelete(NULL);
}

static void BLE_task(void *pvParameters)
{
    char tempArray[5] = { 0 };
    // hrs_init();
    // bas_init();
    mysvc_init();

    while (1) {
        // bt_gatt_bas_set_battery_level(5);
        // bt_hrs_notify(HeartRate);
        sprintf(tempArray, "%.1f", Tempurature);
        bt_gatt_mysvc_update(hr_onenet, stepCnt, tempArray, output_class);
        vTaskDelay(250);
    }
    vTaskDelete(NULL);
}

int main(void)
{
    board_init();
    printf("board_init\r\n");

    /* Get all the device handle*/
    uart0 = bflb_device_get_by_name("uart0");
    spi0 = bflb_device_get_by_name("spi0");
    i2c0 = bflb_device_get_by_name("i2c0");
    gpio = bflb_device_get_by_name("gpio");
    rtc = bflb_device_get_by_name("rtc");

    configASSERT((configMAX_PRIORITIES > 4));

    /* Shell init */
    // shell_init_with_task(uart0);
    // printf("shell_init_with_task\r\n");

    tcpip_init(NULL, NULL);
    printf("tcpip_init\r\n");
    if (1) {
        wifi_start_firmware_task();
        printf("wifi_start\r\n");
    }

    /* set ble controller EM Size */
    btblecontroller_em_config();
    printf("btblecontroller_em_config\r\n");

    // /* Init rf */
    // if (0 != rfparam_init(0, NULL, 0)) {
    //     printf("PHY RF init failed!\r\n");
    //     return 0;
    // }

// Initialize BLE controller
#if defined(BL702) || defined(BL602)
    ble_controller_init(configMAX_PRIORITIES - 1);
#else
    btble_controller_init(configMAX_PRIORITIES - 1);
#endif
    printf("btble_controller_init\r\n");
    // Initialize BLE Host stack
    hci_driver_init();
    printf("hci_driver_init\r\n");

    if (1) {
        bt_enable(bt_enable_cb);
        printf("bt_enable\r\n");
    }

    /* Peripheral Init (spi0, i2c0)*/
    peripheral_init();

    /* TMP117 INIT */
    // m117_soft_reset();
    // bflb_mtimer_delay_ms(500);
    // m117_wakeup();
    // m117_config();
    /* END TMP117 INIT */

    xMutex = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex);

    xMutex_printf = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex_printf);

    xMutex_lvgl = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex_lvgl);

    xMutex_interArrayWindow = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex_interArrayWindow);

    xMutex_DSP = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex_DSP);

    if (xMutex == NULL | xMutex_printf == NULL | xMutex_lvgl == NULL | xMutex_interArrayWindow == NULL | xMutex_DSP == NULL) {
        printf("Create sem fail\r\n");
        while (1) {
        }
    }

    xTaskCreate(EDA_task, (char *)"EDA_task", 1024, NULL, configMAX_PRIORITIES - 4, &EDA_handle);
    xTaskCreate(Algo_task, (char *)"Algo_task", 2 * 1024, NULL, configMAX_PRIORITIES - 4, &Algo_handle);
    xTaskCreate(ACCE_task, (char *)"ACCE_task", 2 * 1024, NULL, configMAX_PRIORITIES - 4, &ACCE_handle);
    xTaskCreate(TEMP_task, (char *)"TEMP_task", 512, NULL, configMAX_PRIORITIES - 4, &TEMP_handle);
    xTaskCreate(HR_task, (char *)"HR_task", 1024, NULL, configMAX_PRIORITIES - 4, &HR_handle);
    xTaskCreate(LVGL_task, (char *)"LVGL_task", 4 * 1024, NULL, configMAX_PRIORITIES - 3, &LVGL_handle);
    xTaskCreate(CLOCK_task, (char *)"Clock_task", 512, NULL, configMAX_PRIORITIES - 4, &CLOCK_handle);
    xTaskCreate(WIFI_task, (char *)"Wifi_task", 2 * 1024, NULL, configMAX_PRIORITIES - 4, &WIFI_handle);
    xTaskCreate(BLE_task, (char *)"Ble_task", 1024, NULL, configMAX_PRIORITIES - 4, &BLE_handle);

    vTaskStartScheduler();
    while (1) {
    }
}
