// #include "shell.h"
#include <FreeRTOS.h>
#include "semphr.h"
#include "task.h"
#include "board.h"
#include "mem.h"

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
// #include "ble_tp_svc.h"
#include "rfparam_adapter.h"
#include "hci_core.h"
// #include "hrs.h"
// #include "bas.h"
#include "gatt.h"
#include "mysvc.h"
#include "plotbvp.h"
#include "ploteda.h"
#include "plotacc.h"
#include "plottemp.h"
#include "cts.h"

#include "bflb_mtd.h"
#include "easyflash.h"

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
#include "bflb_uart.h"

#include "main_functions.h"

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

#include "lsm6dso_reg.h"
#include "MLX90632/mlx90632.h"

#include "afe4404/AFE4404.h"
#include "afe4404/Calibration_AFE4404.h"
#include "afe4404/hrm.h"
#include "afe4404/AFE4404_PORT.h"

#include "NAND_SPI.h"
#include "fatfs_diskio_register.h"
#include "ff.h"
#include "usbd_core.h"
#include "usbd_msc.h"

#include "csi_math.h"
// #include "tinymaix.h"
// #include "model/esdc_model.h"
// #include "model/esdc_model_bin.h"
// #include "Algorithm/CControl/Headers/Functions.h"

// Findpeaks
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>
#include "my_findpeaks.h"
#include "my_findpeaks_emxAPI.h"
#include "my_findpeaks_terminate.h"
#include "my_findpeaks_types.h"
#include "rt_nonfinite.h"

// Time
#include <lwip/apps/sntp.h>
#include "bflb_timestamp.h"

#include "time.h"

#include "bflb_pwm_v2.h"
#include "bflb_clock.h"

#include "INS5699S/ins5699s.h"
#include "max17048.h"

#include "touch_conf_user.h"

#include "main_head.h"

#include "model_test.h"

#define DBG_TAG "MAIN"
#include "log.h"

#define BUFFERLEN          32
#define APPBUFF_SIZE       512
#define ACC_FIFO_WATERMARK 5

/* WIFI */
#define WIFI_STACK_SIZE    (1536)
#define TASK_PRIORITY_FW   configMAX_PRIORITIES - 2

#define LSM6DSO_BOOT_TIME  10

void *__dso_handle = NULL;

static TaskHandle_t EDA_handle = NULL;
static TaskHandle_t ACCE_handle = NULL;
static TaskHandle_t TEMP_handle = NULL;
static TaskHandle_t HR_handle = NULL;
static TaskHandle_t LVGL_handle = NULL;
// static TaskHandle_t WIRELESS_handle;
static TaskHandle_t CLOCK_handle = NULL;
static TaskHandle_t WIFI_handle = NULL;
static TaskHandle_t Algo_handle = NULL;
static TaskHandle_t wifi_fw_task = NULL;
static TaskHandle_t BLE_handle = NULL;
static TaskHandle_t GPS_handle = NULL;
static TaskHandle_t MOTOR_handle = NULL;
static TaskHandle_t BAT_handle = NULL;
static TaskHandle_t FILE_handle = NULL;
static TaskHandle_t BLE_trans_handle = NULL;
static wifi_conf_t conf = {
    .country_code = "CN",
};
static bool wifi_connected = false;
static uint8_t wifiInitDone = 0;

static struct bflb_device_s *uart0;
static struct bflb_device_s *uart1;
struct bflb_device_s *gpio;
struct bflb_device_s *spi0; // For AD5940
struct bflb_device_s *i2c0; // For LIS2DH12, TMP117, AFE4404
struct bflb_device_s *rtc;
struct bflb_device_s *pwm;
struct bflb_device_s *i2c1 = NULL;
struct bflb_device_s *dma0_ch0;
struct bflb_device_s *dma0_ch1;

// AD5940
uint8_t ucInterrupted;
bool AD5940WarmUp = false;
float LFOSCFreq; /* Measured LFOSC frequency */
uint32_t ResistorForBaseline = 0;
uint32_t AppBuff[APPBUFF_SIZE];
// fImpCar_Type EDABase = {
//     .Real = 0.0f,
//     .Image = -0.0f,
// }; // Zero
// fImpCar_Type EDABase = {
//     .Real = 103808.81f,
//     .Image = -120586.1855f,
// }; // device 1
// fImpCar_Type EDABase = {
//     .Real = 106158.5718f,
//     .Image = -117342.1136f,
// }; // device 2
fImpCar_Type EDABase = {
    .Real = 106252.1935f,
    .Image = -114552.1958f,
}; // device 0
int err_code_ad5940;
static bool isAD5940Init = false;
float lastS = 0;

// LSM6DSO
static uint8_t whoamI, rst;
static stmdev_ctx_t dev_ctx;
static int16_t data_raw_acceleration[3];
static float acceleration_mg[3];
static uint32_t stepCnt = 0;
static int16_t data_raw_angular_rate[3];
static float angular_rate_mdps[3];

/* AFE4404 */
uint8_t afe4404_adcrdy = 0;
float afe4404_adcrdy_freq = 0;
// HRM algo
extern unsigned char HeartRate;
uint8_t offsetDACcalibFlag = 0;
int32_t AFE44xx_SPO2_Data_buf[6] = { 0 };
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
static float TempuratureAmbient = 0;

/* freeRTOS */
SemaphoreHandle_t xMutex_IIC0;
SemaphoreHandle_t xMutex_IIC1;
SemaphoreHandle_t xMutex_SPI;
SemaphoreHandle_t xMutex_printf;
SemaphoreHandle_t xMutex_lvgl;
SemaphoreHandle_t xMutex_interArrayWindow; // For BVP window update in HR task
SemaphoreHandle_t xMutex_DSP;
SemaphoreHandle_t xMutex_BLE;
SemaphoreHandle_t xMutex_RTC;
/* binary */
SemaphoreHandle_t xSem_HR = NULL;
SemaphoreHandle_t xSem_EDA = NULL;
// SemaphoreHandle_t xSem_usb = NULL;
SemaphoreHandle_t xSem_MOTOR = NULL;
SemaphoreHandle_t xSem_EDAEN = NULL;
SemaphoreHandle_t xSem_BVPEN = NULL;
SemaphoreHandle_t xSem_ACCEN = NULL;
SemaphoreHandle_t xSem_TEMPEN = NULL;
SemaphoreHandle_t xSem_ALGOEN = NULL;
SemaphoreHandle_t xSem_BLEDISCONN = NULL;
SemaphoreHandle_t xSem_BTNINT = NULL;
SemaphoreHandle_t xSem_spirx = NULL;
SemaphoreHandle_t xSem_spitx = NULL;
SemaphoreHandle_t xSem_spitc = NULL;
// SemaphoreHandle_t xSem_UpdateHRLabel = NULL;
// SemaphoreHandle_t xSem_UpdatePPGChart = NULL;
/* Queues */
QueueHandle_t Queue_HRBLE;
QueueHandle_t Queue_AccBLE_XL;
QueueHandle_t Queue_AccBLE_GY;
QueueHandle_t Queue_TempBLE;
QueueHandle_t Queue_EdaBLE;
// Fifo for file system.
QueueHandle_t Queue_EdaConFile;
QueueHandle_t Queue_EdaPhaFile;
QueueHandle_t Queue_EdaRealFile;
QueueHandle_t Queue_EdaImageFile;
QueueHandle_t Queue_BvpFile;
QueueHandle_t Queue_HRFile;
QueueHandle_t Queue_TempFile_To;
QueueHandle_t Queue_TempPlot_To;
QueueHandle_t Queue_TempFile_Ta;
QueueHandle_t Queue_AccFile_XL_X;
QueueHandle_t Queue_AccFile_GY_X;
QueueHandle_t Queue_AccFile_XL_Y;
QueueHandle_t Queue_AccFile_GY_Y;
QueueHandle_t Queue_AccFile_XL_Z;
QueueHandle_t Queue_AccFile_GY_Z;
QueueHandle_t Queue_AccPlot_XL_X;
QueueHandle_t Queue_AccPlot_XL_Y;
QueueHandle_t Queue_AccPlot_XL_Z;
QueueHandle_t Queue_AlgoFile;
QueueHandle_t Queue_AlgoPlot;
QueueHandle_t Queue_HRLabelUpdate;
QueueHandle_t Queue_PPGChartUpdate;
QueueHandle_t Queue_BvpAlgo;
QueueHandle_t Queue_EdaAlgo;
/* END freeRTOS */

/* Algo */
// float EDAwindow_o[4 * 30] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
// float EDAwindow_f[4 * 30];
// float SCR_nonezero[25] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
// uint32_t SCR_nonezero_timepoint[25] ATTR_NOINIT_PSRAM_SECTION __attribute__((aligned(32)));
float EDA_Window_ori[4 * 40] __attribute__((section(".psram_noinit"), aligned(32)));
float EDA_Window_scale[4 * 40] __attribute__((section(".psram_noinit"), aligned(32)));
int32_t BVP_Window_ori[64 * 40] __attribute__((section(".psram_noinit"), aligned(32)));
int32_t BVP_Window_scale[64 * 40] __attribute__((section(".psram_noinit"), aligned(32)));
float BVP_Window_scale2[64 * 40] __attribute__((section(".psram_noinit"), aligned(32)));
uint32_t eda_cycle_cnt = 0;
uint8_t scr_transfer_pointer = 0;
uint8_t eda_ready_flag = 0;
uint16_t EDAcnt = 0;
float TEMPwindow[4 * 30] = { 0 };
uint8_t tw_pointer = 0;
int output_class = -1;
bool isAlgoOn = false;
/* END Algo*/

long long time_base = 0;
bool timeCorrected = false;

// static const uint8_t m_length = BUFFERLEN;                           /**< Transfer length. */
// static ATTR_NOCACHE_NOINIT_RAM_SECTION uint8_t tx_buffer[BUFFERLEN]; /**< TX buffer. */
// static ATTR_NOCACHE_NOINIT_RAM_SECTION uint8_t rx_buffer[BUFFERLEN]; /**< RX buffer. */

struct bflb_dma_channel_lli_pool_s tx_llipool[1];
struct bflb_dma_channel_lli_pool_s rx_llipool[1];

volatile bool spi0_tc = false; /**< Flag used to indicate that SPI instance completed the transfer. */

uint8_t CHG_Interrupted = 0;
bool isBleConnected = false;
bool isPlotedaInit = false;
bool isPlotBvpInit = false;
bool isPlotTempInit = false;
bool isPlotAccInit = false;
struct bt_conn *my_conn;
struct bt_gatt_exchange_params exchg_mtu;
int tx_mtu_size = 20;
// extern void shell_init_with_task(struct bflb_device_s *shell);

ATTR_NOCACHE_NOINIT_RAM_SECTION __attribute((aligned(32))) FATFS fs;
__attribute((aligned(32))) static ATTR_NOCACHE_NOINIT_RAM_SECTION uint32_t workbuf[4096];

MKFS_PARM fs_para = {
    .fmt = FM_FAT32,     /* Format option (FM_FAT, FM_FAT32, FM_EXFAT and FM_SFD) */
    .n_fat = 1,          /* Number of FATs */
    .align = 0,          /* Data area alignment (sector) */
    .n_root = 1,         /* Number of root directory entries */
    .au_size = 512 * 32, /* Cluster size (byte) */
};

ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) FIL f_eda;
ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) FIL f_temp;
ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) FIL f_acc;
ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) FIL f_bvp;
ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) FIL f_algo;

/* Private Function Defination */
/*Function Definations For lsm6dso */
static int32_t lsm6dso_platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
                                      uint16_t len);
static int32_t lsm6dso_platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                                     uint16_t len);
// static void tx_com(uint8_t *tx_buffer, uint16_t len);
static void lsm6dso_platform_delay(uint32_t ms);
// static void platform_init(void);
/*END Function Definations For lsm6dso */

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

static void ble_tp_tx_mtu_size(struct bt_conn *conn, u8_t err,
                               struct bt_gatt_exchange_params *params)
{
    if (!err) {
        tx_mtu_size = bt_gatt_get_mtu(conn);
        printf("ble tp echange mtu size success, mtu size: %d", tx_mtu_size);
    } else {
        printf("ble tp echange mtu size failure, err: %d", err);
    }
}

static void ble_connected(struct bt_conn *conn, u8_t err)
{
    if (err || conn->type != BT_CONN_TYPE_LE) {
        return;
    }
    int ret = -1;
    int tx_octets = 0x00fb;
    int tx_time = 0x0848;

    printf("%s", __func__);

    xQueueReset(Queue_HRBLE);
    xQueueReset(Queue_AccBLE_GY);
    xQueueReset(Queue_AccBLE_XL);
    xQueueReset(Queue_TempBLE);

    isBleConnected = true;
    if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
        ui_setBleImg(1);
        xSemaphoreGive(xMutex_lvgl);
    }

    //set data length after connected.
    ret = bt_le_set_data_len(conn, tx_octets, tx_time);
    if (!ret) {
        printf("ble tp set data length success.");
    } else {
        printf("ble tp set data length failure, err: %d\n", ret);
    }

    //exchange mtu size after connected.
    exchg_mtu.func = ble_tp_tx_mtu_size;
    ret = bt_gatt_exchange_mtu(conn, &exchg_mtu);
    if (!ret) {
        printf("ble tp exchange mtu size pending.");
    } else {
        printf("ble tp exchange mtu size failure, err: %d", ret);
    }

    my_conn = conn;
}

static void ble_disconnected(struct bt_conn *conn, u8_t reason)
{
    int ret;

    if (conn->type != BT_CONN_TYPE_LE) {
        return;
    }

    printf("%s", __func__);
    isBleConnected = false;
    if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
        ui_setBleImg(0);
        xSemaphoreGive(xMutex_lvgl);
    }

    if (xSemaphoreTake(xSem_BLEDISCONN, 0) == pdTRUE) {
        bt_le_adv_stop();

        ret = bt_disable();

        if (ret == 0) {
            printf("bt_disable\r\n");
        } else {
            printf("bt_disable Error. %d\r\n", ret);
        }
    } else {
        // enable adv
        ret = set_adv_enable(true);
        if (ret) {
            printf("Restart adv fail. \r\n");
        }
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
        // ble_tp_init();

        // start advertising
        ble_start_adv();
    } else {
        printf("bt_enable error: %d\r\n", err);
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

    // /* set ble controller EM Size */

    // GLB_Set_EM_Sel(GLB_WRAM160KB_EM0KB);

    // if (0 != rfparam_init(0, NULL, 0)) {
    //     LOG_I("PHY RF init failed!\r\n");
    //     return 0;
    // }

    // LOG_I("PHY RF init success!\r\n");

    /* Enable wifi irq */

    extern void interrupt0_handler(void);
    bflb_irq_attach(WIFI_IRQn, (irq_callback)interrupt0_handler, NULL);
    // bflb_irq_set_priority(WIFI_IRQn, 7, 0);
    bflb_irq_enable(WIFI_IRQn);

    xTaskCreate(wifi_main, (char *)"fw", WIFI_STACK_SIZE, NULL, TASK_PRIORITY_FW, &wifi_fw_task);

    return 0;
}

int wifi_stop_firmware_task(void)
{
    // LOG_I("Stoping wifi ...\r\n");

    /* disable wifi clock */

    // GLB_PER_Clock_Gate(GLB_AHB_CLOCK_IP_WIFI_PHY | GLB_AHB_CLOCK_IP_WIFI_MAC_PHY | GLB_AHB_CLOCK_IP_WIFI_PLATFORM);
    // GLB_AHB_MCU_Software_Reset(GLB_AHB_MCU_SW_WIFI);

    // /* set ble controller EM Size */

    // GLB_Set_EM_Sel(GLB_WRAM160KB_EM0KB);

    // if (0 != rfparam_init(0, NULL, 0)) {
    //     LOG_I("PHY RF init failed!\r\n");
    //     return 0;
    // }

    // LOG_I("PHY RF init success!\r\n");

    /* Enable wifi irq */

    // extern void interrupt0_handler(void);
    // bflb_irq_attach(WIFI_IRQn, (irq_callback)interrupt0_handler, NULL);
    // bflb_irq_set_priority(WIFI_IRQn, 7, 0);
    // bflb_irq_disable(WIFI_IRQn);

    // if (wifi_fw_task != NULL) {
    //     vTaskDelete(wifi_fw_task);
    // }
    // xTaskCreate(wifi_main, (char *)"fw", WIFI_STACK_SIZE, NULL, TASK_PRIORITY_FW, &wifi_fw_task);

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

            void mm_sec_keydump();
            mm_sec_keydump();
        } break;
        case CODE_WIFI_ON_GOT_IP: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_GOT_IP\r\n", __func__);
        } break;
        case CODE_WIFI_ON_DISCONNECT: {
            LOG_I("[APP] [EVT] %s, CODE_WIFI_ON_DISCONNECT\r\n", __func__);
            wifi_connected = false;
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

void dma0_ch0_isr(void *arg)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // printf("tx done\r\n");
    xSemaphoreGiveFromISR(xSem_spitx, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void dma0_ch1_isr(void *arg)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // printf("rx done\r\n");
    xSemaphoreGiveFromISR(xSem_spirx, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void spi_isr(int irq, void *arg)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t intstatus = bflb_spi_get_intstatus(spi0);
    // printf("tc done1\r\n");
    if (intstatus & SPI_INTSTS_TC) {
        bflb_spi_int_clear(spi0, SPI_INTCLR_TC);
        // spi0_tc = true;
        // printf("tc done\r\n");
        xSemaphoreGiveFromISR(xSem_spitc, &xHigherPriorityTaskWoken);
        
    } else if (intstatus & SPI_INTSTS_TX_FIFO) {
        // printf("tx fifo\r\n");
    } else if (intstatus & SPI_INTSTS_RX_FIFO) {
        // printf("rx fifo\r\n");
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/* MCU related external line interrupt service routine */
void GPIO_Interrupt(int irq, void *arg)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // vTaskSuspend(FILE_handle);
    // printf("GPIO INT\r\n");
    if (bflb_gpio_get_intstatus(gpio, AFE_ADC_DRDY)) {
        // afe4404_adcrdy = 1;
        bflb_gpio_int_clear(gpio, AFE_ADC_DRDY);
        xSemaphoreGiveFromISR(xSem_HR, &xHigherPriorityTaskWoken);
        // printf("GPIO AFE4404 INT\r\n");
    }

    xHigherPriorityTaskWoken = pdFALSE;
    if (bflb_gpio_get_intstatus(gpio, KEY0)) {
        // ucInterrupted = 1;
        // printf("AD5940 INT: %d\r\n", AD5940_GetMCUIntFlag());
        bflb_gpio_int_clear(gpio, KEY0);
        xSemaphoreGiveFromISR(xSem_EDA, &xHigherPriorityTaskWoken);
    }

    if (bflb_gpio_get_intstatus(gpio, GPIO_PIN_33)) {
        if (bflb_gpio_read(gpio, GPIO_PIN_33)) { // 1:2 Not Charging
            CHG_Interrupted = 2;
        } else { // 0:1 Charging
            CHG_Interrupted = 1;
        }

        // printf("CHG_Interrupted: %d\r\n", CHG_Interrupted);
        bflb_gpio_int_clear(gpio, GPIO_PIN_33);
    }

    if (bflb_gpio_get_intstatus(gpio, GPIO_PIN_2)) {
        printf("BTN_Interrupted.\r\n");
        bflb_gpio_int_clear(gpio, GPIO_PIN_2);
        xSemaphoreGiveFromISR(xSem_BTNINT, &xHigherPriorityTaskWoken);
    }

    // xTaskResumeFromISR(FILE_handle);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void spi0_init(void)
{
    /* spi0 Init New */
    // For AD5941 & NAND

    /* spi clk */
    bflb_gpio_init(gpio, GPIO_PIN_25, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0 | GPIO_DRV_3);
    /* spi miso */
    bflb_gpio_init(gpio, GPIO_PIN_18, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0 | GPIO_DRV_3);
    /* spi mosi */
    bflb_gpio_init(gpio, GPIO_PIN_19, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_0 | GPIO_DRV_3);

    //CS pin For AD5941
    bflb_gpio_init(gpio, AD5940_CS_PIN, GPIO_OUTPUT | GPIO_PULLUP | GPIO_DRV_0 | GPIO_DRV_3);
    bflb_gpio_set(gpio, AD5940_CS_PIN);
    //CS Pin For NAND
    bflb_gpio_init(gpio, GPIO_PIN_22, GPIO_OUTPUT | GPIO_PULLUP | GPIO_DRV_0 | GPIO_DRV_3);
    bflb_gpio_set(gpio, GPIO_PIN_22);

    /* Set the SPI parameters */
    struct bflb_spi_config_s spi_cfg_0 = {
        .freq = 200 * 1000, // 200kHz
        .role = SPI_ROLE_MASTER,
        .mode = SPI_MODE0,
        .data_width = SPI_DATA_WIDTH_8BIT,
        .bit_order = SPI_BIT_MSB,
        .byte_order = SPI_BYTE_LSB,
        .tx_fifo_threshold = 0,
        .rx_fifo_threshold = 0,
    };

    struct bflb_dma_channel_config_s tx_config = {
        .direction = DMA_MEMORY_TO_PERIPH,
        .src_req = DMA_REQUEST_NONE,
        .dst_req = DMA_REQUEST_SPI0_TX,
        .src_addr_inc = DMA_ADDR_INCREMENT_ENABLE,
        .dst_addr_inc = DMA_ADDR_INCREMENT_DISABLE,
        .src_burst_count = DMA_BURST_INCR1,
        .dst_burst_count = DMA_BURST_INCR1,
        .src_width = DMA_DATA_WIDTH_8BIT,
        .dst_width = DMA_DATA_WIDTH_8BIT,
    };
    struct bflb_dma_channel_config_s rx_config = {
        .direction = DMA_PERIPH_TO_MEMORY,
        .src_req = DMA_REQUEST_SPI0_RX,
        .dst_req = DMA_REQUEST_NONE,
        .src_addr_inc = DMA_ADDR_INCREMENT_DISABLE,
        .dst_addr_inc = DMA_ADDR_INCREMENT_ENABLE,
        .src_burst_count = DMA_BURST_INCR1,
        .dst_burst_count = DMA_BURST_INCR1,
        .src_width = DMA_DATA_WIDTH_8BIT,
        .dst_width = DMA_DATA_WIDTH_8BIT,
    };

    bflb_spi_init(spi0, &spi_cfg_0);

    NAND_DeviceInit();

    bflb_spi_feature_control(spi0, SPI_CMD_SET_FREQ, 16 * 1000 * 1000);

    NAND_GetInfo();

    // bflb_spi_link_txdma(spi0, true);
    // bflb_spi_link_rxdma(spi0, true);

    // bflb_spi_tcint_mask(spi0, true);
    // bflb_irq_attach(spi0->irq_num, spi_isr, NULL);
    // bflb_irq_enable(spi0->irq_num);

    dma0_ch0 = bflb_device_get_by_name("dma0_ch0");
    dma0_ch1 = bflb_device_get_by_name("dma0_ch1");

    bflb_dma_channel_init(dma0_ch0, &tx_config);
    bflb_dma_channel_init(dma0_ch1, &rx_config);

    bflb_dma_channel_irq_attach(dma0_ch0, dma0_ch0_isr, NULL);
    bflb_dma_channel_irq_attach(dma0_ch1, dma0_ch1_isr, NULL);
}

static void peripheral_init(void)
{
    //     /* spi0 Init */
    //     // For AD5940/5941

    //     /* spi clk */
    //     bflb_gpio_init(gpio, GPIO_PIN_25, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    //     /* spi miso */
    //     bflb_gpio_init(gpio, GPIO_PIN_18, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    //     /* spi mosi */
    //     bflb_gpio_init(gpio, GPIO_PIN_19, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    //     /* CS controlled by software */

    //     /* Set the SPI parameters */
    //     struct bflb_spi_config_s spi_cfg = {
    // #if (SPI_CASE_SELECT == SPI_MASTER_CASE)
    //         .freq = 12 * 1000 * 1000,
    //         .role = SPI_ROLE_MASTER,
    // #else
    //         .freq = 32 * 1000 * 1000,
    //         .role = SPI_ROLE_SLAVE,
    // #endif
    //         .mode = SPI_MODE0,
    //         .data_width = SPI_DATA_WIDTH_8BIT,
    //         .bit_order = SPI_BIT_MSB,
    //         .byte_order = SPI_BYTE_LSB,
    //         .tx_fifo_threshold = 0,
    //         .rx_fifo_threshold = 0,
    //     };

    //     // struct bflb_dma_channel_config_s tx_config = {
    //     //     .direction = DMA_MEMORY_TO_PERIPH,
    //     //     .src_req = DMA_REQUEST_NONE,
    //     //     .dst_req = DMA_REQUEST_SPI0_TX,
    //     //     .src_addr_inc = DMA_ADDR_INCREMENT_ENABLE,
    //     //     .dst_addr_inc = DMA_ADDR_INCREMENT_DISABLE,
    //     //     .src_burst_count = DMA_BURST_INCR1,
    //     //     .dst_burst_count = DMA_BURST_INCR1,
    //     //     .src_width = DMA_DATA_WIDTH_8BIT,
    //     //     .dst_width = DMA_DATA_WIDTH_8BIT,
    //     // };

    //     // struct bflb_dma_channel_config_s rx_config = {
    //     //     .direction = DMA_PERIPH_TO_MEMORY,
    //     //     .src_req = DMA_REQUEST_SPI0_RX,
    //     //     .dst_req = DMA_REQUEST_NONE,
    //     //     .src_addr_inc = DMA_ADDR_INCREMENT_DISABLE,
    //     //     .dst_addr_inc = DMA_ADDR_INCREMENT_ENABLE,
    //     //     .src_burst_count = DMA_BURST_INCR1,
    //     //     .dst_burst_count = DMA_BURST_INCR1,
    //     //     .src_width = DMA_DATA_WIDTH_8BIT,
    //     //     .dst_width = DMA_DATA_WIDTH_8BIT,
    //     // };

    //     /* Now do not use DMA */
    //     bflb_spi_init(spi0, &spi_cfg);
    //     // bflb_spi_txint_mask(spi0, false);
    //     // bflb_spi_rxint_mask(spi0, false);
    //     bflb_spi_tcint_mask(spi0, false);              // Disable Interrupt
    //     bflb_irq_attach(spi0->irq_num, spi_isr, NULL); // IRQ
    //     bflb_irq_enable(spi0->irq_num);
    //     // bflb_spi_feature_control(spi0, SPI_CMD_SET_CS_INTERVAL, 0);

    // bflb_spi_link_txdma(spi0, true);
    // bflb_spi_link_rxdma(spi0, true);

    // bflb_dma_channel_init(dma0_ch0, &tx_config);
    // bflb_dma_channel_init(dma0_ch1, &rx_config);

    // bflb_dma_channel_irq_attach(dma0_ch0, dma0_ch0_isr, NULL);
    // bflb_dma_channel_irq_attach(dma0_ch1, dma0_ch1_isr, NULL);

    //     /* END spi0 Init */

    spi0_init();

    /* AD5941 Pin Init */
    //Interrupt Key
    bflb_gpio_init(gpio, KEY0, GPIO_INPUT);
    bflb_gpio_int_init(gpio, KEY0, GPIO_INT_TRIG_MODE_SYNC_FALLING_EDGE);
    bflb_gpio_int_mask(gpio, KEY0, false);
    //RST pin
    bflb_gpio_init(gpio, AD5940_RST_PIN, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_3);
    bflb_gpio_set(gpio, AD5940_RST_PIN);

    /* MAX17048 Pin Init */
    bflb_gpio_init(gpio, GPIO_PIN_34, GPIO_INPUT | GPIO_PULLUP);
    // bflb_gpio_int_mask(gpio, GPIO_PIN_34, false);

    /* i2c0 Init */
    // For LIS2DH12, M117, AFE4404
    /* I2C0_SDA */
    bflb_gpio_init(gpio, GPIO_PIN_27, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_3);
    /* I2C0_SCL */
    bflb_gpio_init(gpio, GPIO_PIN_26, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_3);
    bflb_i2c_init(i2c0, 400000);
    /* END i2c0 init*/

    /* Attach IRQ */
    bflb_irq_attach(gpio->irq_num, GPIO_Interrupt, gpio);

    /* AFE4404 Pin Init */
    bflb_gpio_init(gpio, AFE_ADC_DRDY, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_3);
    bflb_gpio_int_init(gpio, AFE_ADC_DRDY, GPIO_INT_TRIG_MODE_ASYNC_RISING_EDGE);
    bflb_gpio_int_mask(gpio, AFE_ADC_DRDY, true); // Now Disable Pin 28 Interrupt For AFE4404

    // RST
    bflb_gpio_init(gpio, AFE_RESETZ, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_3);
    bflb_gpio_set(gpio, AFE_RESETZ);

    /* Enable IRQ */
    bflb_irq_set_priority(gpio->irq_num, 8, 0);
    bflb_irq_enable(gpio->irq_num);

    /* MOTOR PWM INIT */
    bflb_gpio_init(gpio, GPIO_PIN_14, GPIO_FUNC_PWM0 | GPIO_ALTERNATE | GPIO_PULLDOWN | GPIO_SMT_EN | GPIO_DRV_3);
    bflb_gpio_reset(gpio, GPIO_PIN_14);

    /* period = .XCLK / .clk_div / .period = 40MHz( 32MHZ for bl702l) / 40( 32 for bl702l) / 1000 = 1KHz */
    struct bflb_pwm_v2_config_s cfg = {
        .clk_source = BFLB_SYSTEM_PBCLK,
#if defined(BL702L)
        .clk_div = 32,
#else
        .clk_div = 40,
#endif
        .period = 100,
    };
    bflb_pwm_v2_init(pwm, &cfg);

    /* I2C1 INIT (TOUCH, RTC, FUEL) */
    /* I2C0_SCL */
    bflb_gpio_init(gpio, TOUCH_I2C_SCL_PIN, GPIO_FUNC_I2C1 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_3);
    /* I2C0_SDA */
    bflb_gpio_init(gpio, TOUCH_I2C_SDA_PIN, GPIO_FUNC_I2C1 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_3);
    /* TOUCH_RESET */
    bflb_gpio_init(gpio, TOUCH_RESET, GPIO_OUTPUT | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_3);

    bflb_i2c_init(i2c1, 400000);

    /* GPIO33 CHG */
    bflb_gpio_init(gpio, GPIO_PIN_33, GPIO_INPUT);
    bflb_gpio_int_init(gpio, GPIO_PIN_33, GPIO_INT_TRIG_MODE_SYNC_FALLING_RISING_EDGE);
    bflb_gpio_int_mask(gpio, GPIO_PIN_33, false);

    /* GPIO13 MOROT_EN */
    bflb_gpio_init(gpio, GPIO_PIN_13, GPIO_OUTPUT | GPIO_PULLDOWN | GPIO_DRV_3);
    bflb_gpio_reset(gpio, GPIO_PIN_13);

    /* GPIO2 BTN */
    bflb_gpio_init(gpio, GPIO_PIN_2, GPIO_INPUT | GPIO_SMT_EN);
    bflb_gpio_int_init(gpio, GPIO_PIN_2, GPIO_INT_TRIG_MODE_SYNC_FALLING_EDGE);
    bflb_gpio_int_mask(gpio, GPIO_PIN_2, false);
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
    // gpio_cfg.FuncSet = GP6_SYNC | GP5_SYNC | GP4_SYNC | GP2_EXTCLK | GP1_SYNC | GP0_INT;
    gpio_cfg.FuncSet = GP0_INT;
    gpio_cfg.InputEnSet = 0;
    gpio_cfg.OutputEnSet = AGPIO_Pin0;
    gpio_cfg.OutVal = 0;
    gpio_cfg.PullEnSet = 0;
    AD5940_AGPIOCfg(&gpio_cfg);

    AD5940_SleepKeyCtrlS(SLPKEY_UNLOCK); /* Enable AFE to enter sleep mode. */

    /* Measure LFOSC frequency */
    LfoscMeasure.CalDuration = 2000.0; /* 1000ms used for calibration. */
    LfoscMeasure.CalSeqAddr = 0;
    LfoscMeasure.SystemClkFreq = 16000000.0f; /* 16MHz in this firmware. */
    AD5940_LFOSCMeasure(&LfoscMeasure, &LFOSCFreq);
    printf("Freq:%.3f\r\n", LFOSCFreq);

    return 0;
}

void AD5940_ReadWriteNBytes(unsigned char *pSendBuffer, unsigned char *pRecvBuff, unsigned long length)
{
    // spi0_tc = false;
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

    // printf("AD5940_ReadWriteNBytes Enter\r\n");
    bflb_spi_feature_control(spi0, SPI_CMD_SET_FREQ, 8 * 1000 * 1000);

    // struct bflb_dma_channel_lli_transfer_s tx_transfers[1];
    // struct bflb_dma_channel_lli_transfer_s rx_transfers[1];

    // tx_transfers[0].src_addr = (uint32_t)pSendBuffer;
    // tx_transfers[0].dst_addr = (uint32_t)DMA_ADDR_SPI0_TDR;
    // tx_transfers[0].nbytes = length;

    // rx_transfers[0].src_addr = (uint32_t)DMA_ADDR_SPI0_RDR;
    // rx_transfers[0].dst_addr = (uint32_t)pRecvBuff;
    // rx_transfers[0].nbytes = length;

    // bflb_dma_channel_lli_reload(dma0_ch0, tx_llipool, 1, tx_transfers, 1);
    // bflb_dma_channel_lli_reload(dma0_ch1, rx_llipool, 1, rx_transfers, 1);
    // bflb_dma_channel_start(dma0_ch1);
    // bflb_dma_channel_start(dma0_ch0);
    // printf("bflb_dma_channel_start\r\n");

    // if (xSemaphoreTake(xSem_spitx, portMAX_DELAY) == pdFALSE) {
    //     return;
    // }
    // if (xSemaphoreTake(xSem_spirx, portMAX_DELAY) == pdFALSE) {
    //     return;
    // }
    // if (xSemaphoreTake(xSem_spitc, portMAX_DELAY) == pdFALSE) {
    //     return;
    // }

    bflb_spi_poll_exchange(spi0, pSendBuffer, pRecvBuff, length);
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
    pCfg->RcalVal = 20000.0;
    pCfg->DftNum = DFTNUM_16;
    // pCfg->ReDoRtiaCal = bTRUE;
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
    static float dataBuf[16] = { 0 };
    uint32_t *ptr = &S;

    /*Process data*/
    for (int i = 0; i < DataCount; i++) {
        res = pImp[i];
        res.Real += ResistorForBaseline; /* Show the real result of impedance under test(between F+/S+) */
        // printf("{res}%.4f, %.4f\r\n", res.Real, res.Image);
        mag = AD5940_ComplexMag(&res);
        phase = AD5940_ComplexPhase(&res) * 180 / MATH_PI;
        S = 1000000 / mag;
        // printf("Rtia:%.3f; Real:%.3f; Image:%.3f; Mag:%.3f; Conductance: %.3f uS; Phase:%.3f \r\n", RtiaMag, res.Real, res.Image, mag, S, phase);
        // printf("{Conductance}%.8f\r\n", S);
        // printf("{Conductance_hex}%08x\r\n", *ptr);
        // printf("{Phase}%.3f\r\n", phase);
        // eda_transfer_cnt++;
        // if (isnanf(S) || (S > 100.0f)) {
        //     EDAcnt = 0;
        // } else {
        //     EDAwindow_o[EDAcnt] = S;
        //     EDAcnt++;
        // }
        if (isnanf(S) || (S < 0.0f) || (S > 10000.0f)) {
            S = lastS;
        }
        dataBuf[i] = S;
        xQueueSend(Queue_EdaConFile, &S, 0);
        xQueueSend(Queue_EdaPhaFile, &phase, 0);
        xQueueSend(Queue_EdaRealFile, &res.Real, 0);
        xQueueSend(Queue_EdaImageFile, &res.Image, 0);
        if (isAlgoOn) {
            xQueueSend(Queue_EdaAlgo, &S, 0);
        }
        if (isPlotedaInit && isBleConnected) {
            xQueueSend(Queue_EdaBLE, &S, 0);
        }
        lastS = S;
    }

    // if (isPlotedaInit && isBleConnected) {
    //     if (xSemaphoreTake(xMutex_BLE, 0) == pdTRUE) {
    //         bt_gatt_ploteda_update(dataBuf, DataCount);
    //         xSemaphoreGive(xMutex_BLE);
    //     }
    // }
    return 0;
}

uint8_t BL618_I2C_TRANSMIT(struct bflb_i2c_msg_s *msg, uint16_t msglen)
{
    if (xSemaphoreTake(xMutex_IIC0,
                       portMAX_DELAY) == pdTRUE) {
        uint8_t ret_code = bflb_i2c_transfer(i2c0, msg, msglen);
        xSemaphoreGive(xMutex_IIC0);
        return ret_code;
    }
    return -1;
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
static int32_t lsm6dso_platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
                                      uint16_t len)
{
    /* Write */
    struct bflb_i2c_msg_s msgs[1];
    uint8_t subaddr[1 + len];

    subaddr[0] = reg;
    for (int8_t i = 0; i < len; i++) {
        subaddr[i + 1] = bufp[i];
    }

    msgs[0].addr = LSM6DSO_I2C_ADD_L >> 1;
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
static int32_t lsm6dso_platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                                     uint16_t len)
{
    /* Read */
    struct bflb_i2c_msg_s msgs[2];
    uint8_t subaddr[1] = { reg };

    msgs[0].addr = LSM6DSO_I2C_ADD_L >> 1;
    msgs[0].flags = I2C_M_NOSTOP;
    msgs[0].buffer = subaddr;
    msgs[0].length = 1;

    msgs[1].addr = LSM6DSO_I2C_ADD_L >> 1;
    msgs[1].flags = I2C_M_READ;
    msgs[1].buffer = bufp;
    msgs[1].length = len;

    return BL618_I2C_TRANSMIT(msgs, 2);
}

/*
 * @brief  platform specific delay (platform dependent)
 *
 * @param  ms        delay in ms
 *
 */
static void platform_delay(uint32_t ms)
{
    vTaskDelay(ms);
}

int32_t mlx90632_i2c_read(int16_t register_address, uint16_t *value)
{
    struct bflb_i2c_msg_s msgs[2];
    uint8_t data[2] = { 0 };
    uint32_t ret = 0;
    uint8_t subaddr[2] = {
        (register_address & 0xFF00) >> 8,
        register_address & 0x00FF,
    };

    msgs[0].addr = MLX_CHIP_ADDRESS;
    msgs[0].flags = I2C_M_NOSTOP;
    msgs[0].buffer = subaddr;
    msgs[0].length = 2;

    msgs[1].addr = MLX_CHIP_ADDRESS;
    msgs[1].flags = I2C_M_READ;
    msgs[1].buffer = data;
    msgs[1].length = 2;

    ret = BL618_I2C_TRANSMIT(msgs, 2);
    data[0] = data[0] & 0xFF;
    data[1] = data[1] & 0xFF;
    *value = data[1] | (data[0] << 8);
    // printf("READ %X\r\n", *value);
    return ret;
}

int32_t mlx90632_i2c_read32(int16_t register_address, uint32_t *value)
{
    struct bflb_i2c_msg_s msgs[2];
    uint8_t data[4] = { 0 };
    uint8_t subaddr[2] = {
        (register_address & 0xFF00) >> 8,
        register_address & 0x00FF,
    };
    int32_t ret = 0;

    msgs[0].addr = MLX_CHIP_ADDRESS;
    msgs[0].flags = I2C_M_NOSTOP;
    msgs[0].buffer = subaddr;
    msgs[0].length = 2;

    msgs[1].addr = MLX_CHIP_ADDRESS;
    msgs[1].flags = I2C_M_READ;
    msgs[1].buffer = data;
    msgs[1].length = 4;

    ret = BL618_I2C_TRANSMIT(msgs, 2);

    *value = (uint32_t)data[2] << 24 | (uint32_t)data[3] << 16 | (uint32_t)data[0] << 8 | (uint32_t)data[1];
    return ret;
}

int32_t mlx90632_i2c_write(int16_t register_address, uint16_t value)
{
    struct bflb_i2c_msg_s msgs[1];
    uint8_t subaddr[4] = {
        (register_address & 0xFF00) >> 8,
        register_address & 0x00FF,
        (value & 0xFF00) >> 8,
        value & 0x00FF,
    };

    msgs[0].addr = MLX_CHIP_ADDRESS;
    msgs[0].flags = 0;
    msgs[0].buffer = subaddr;
    msgs[0].length = 4;

    return BL618_I2C_TRANSMIT(msgs, 1);
}

/* Implementation of reading all calibration parameters for calucation of Ta and To */
static int mlx90632_read_eeprom(int32_t *PR, int32_t *PG, int32_t *PO, int32_t *PT, int32_t *Ea, int32_t *Eb, int32_t *Fa, int32_t *Fb, int32_t *Ga, int16_t *Gb, int16_t *Ha, int16_t *Hb, int16_t *Ka)
{
    int32_t ret;
    ret = mlx90632_i2c_read32(MLX90632_EE_P_R, (uint32_t *)PR);
    if (ret < 0)
        return ret;
    ret = mlx90632_i2c_read32(MLX90632_EE_P_G, (uint32_t *)PG);
    if (ret < 0)
        return ret;
    ret = mlx90632_i2c_read32(MLX90632_EE_P_O, (uint32_t *)PO);
    if (ret < 0)
        return ret;
    ret = mlx90632_i2c_read32(MLX90632_EE_P_T, (uint32_t *)PT);
    if (ret < 0)
        return ret;
    ret = mlx90632_i2c_read32(MLX90632_EE_Ea, (uint32_t *)Ea);
    if (ret < 0)
        return ret;
    ret = mlx90632_i2c_read32(MLX90632_EE_Eb, (uint32_t *)Eb);
    if (ret < 0)
        return ret;
    ret = mlx90632_i2c_read32(MLX90632_EE_Fa, (uint32_t *)Fa);
    if (ret < 0)
        return ret;
    ret = mlx90632_i2c_read32(MLX90632_EE_Fb, (uint32_t *)Fb);
    if (ret < 0)
        return ret;
    ret = mlx90632_i2c_read32(MLX90632_EE_Ga, (uint32_t *)Ga);
    if (ret < 0)
        return ret;
    ret = mlx90632_i2c_read(MLX90632_EE_Gb, (uint16_t *)Gb);
    if (ret < 0)
        return ret;
    ret = mlx90632_i2c_read(MLX90632_EE_Ha, (uint16_t *)Ha);
    if (ret < 0)
        return ret;
    ret = mlx90632_i2c_read(MLX90632_EE_Hb, (uint16_t *)Hb);
    if (ret < 0)
        return ret;
    ret = mlx90632_i2c_read(MLX90632_EE_Ka, (uint16_t *)Ka);
    if (ret < 0)
        return ret;

    // printf("R %d, G %d, T %d, O %d, Ea %d, Eb %d, Fa %d, Fb %d, Ga %d, Gb %d, Ka %d, Ha %d, Hb %d\n", *PR, *PG, *PT, *PO, *Ea, *Eb, *Fa, *Fb, *Ga, *Gb, *Ka, *Ha, *Hb);

    return 0;
}

void MLX_usleep(int min_range, int max_range)
{
    vTaskDelay(min_range / 1000);
}

void MLX_msleep(int msecs)
{
    vTaskDelay(msecs);
}
/**********************************************************************************************************/
/*	        AFE4404_Trigger_HWReset         				                                              */
/**********************************************************************************************************/
void AFE4404_Trigger_HWReset(void)
{
    bflb_gpio_reset(gpio, AFE_RESETZ);
    vTaskDelay(1); // ~30us delay
    bflb_gpio_set(gpio, AFE_RESETZ);
    vTaskDelay(10); // ~10ms delay with 16MHz clock
}

/**********************************************************************************************************/
/*	        AFE4404_Enable_HWPDN         				                                          		  */
/**********************************************************************************************************/
void AFE4404_Enable_HWPDN(void)
{
    bflb_gpio_reset(gpio, AFE_RESETZ);
    vTaskDelay(10); // ~10ms delay with 16MHz clock
}

/**********************************************************************************************************/
/*	        AFE4404_Disable_HWPDN         				                                          		  */
/**********************************************************************************************************/
void AFE4404_Disable_HWPDN(void)
{
    bflb_gpio_set(gpio, AFE_RESETZ);
    vTaskDelay(10); // ~10ms delay with 16MHz clock
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
int32_t AFE4404_Reg_Read(unsigned char Reg_address)
{
    unsigned char configData[3];
    int32_t retVal;
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

// static tm_err_t layer_cb(tm_mdl_t *mdl, tml_head_t *lh)
// { //dump middle result
//     int h = lh->out_dims[1];
//     int w = lh->out_dims[2];
//     int ch = lh->out_dims[3];
//     mtype_t *output = TML_GET_OUTPUT(mdl, lh);
//     return TM_OK;
//     TM_PRINTF("Layer %d callback ========\n", mdl->layer_i);
// #if 1
//     for (int y = 0; y < h; y++) {
//         TM_PRINTF("[");
//         for (int x = 0; x < w; x++) {
//             TM_PRINTF("[");
//             for (int c = 0; c < ch; c++) {
// #if TM_MDL_TYPE == TM_MDL_FP32
//                 TM_PRINTF("%.3f,", output[(y * w + x) * ch + c]);
// #else
//                 TM_PRINTF("%.3f,", TML_DEQUANT(lh, output[(y * w + x) * ch + c]));
// #endif
//             }
//             TM_PRINTF("],");
//         }
//         TM_PRINTF("],\n");
//     }
//     TM_PRINTF("\n");
// #endif
//     return TM_OK;
// }

// static void parse_output(tm_mat_t *outs, uint8_t dim)
// {
//     tm_mat_t out = outs[0];
//     float *data = out.dataf;
//     float maxp = 0;
//     float sump = 0;
//     int maxi = -1;
//     for (int i = 0; i < dim; i++) {
//         // printf("%d: %.3f\n", i, data[i]);
//         sump += data[i];
//         if (data[i] > maxp) {
//             maxi = i;
//             maxp = data[i];
//         }
//     }
//     for (int i = 0; i < dim; i++) {
//         printf("%d: %.3f\n", i, data[i] / sump);
//     }
//     if (dim == 2) {
//         xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
//         ui_UpdateMentalLabel(maxi, data[maxi] / sump);
//         ui_UpdateMdl2(data[0] / sump, data[1] / sump);
//         xSemaphoreGive(xMutex_lvgl);
//         output_class = maxi;
//     } else if (dim == 3) {
//         xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
//         ui_UpdateMdl1(data[0] / sump, data[1] / sump, data[2] / sump);
//         xSemaphoreGive(xMutex_lvgl);
//     }
//     TM_PRINTF("### Predict output is: Number %d, prob %.3f\n", maxi, maxp / sump);
//     return;
// }

void sntp_set_time(uint32_t sntp_time) // call back configured at "sntp_opts.h"
{
    struct bflb_tm time;
    // bflb_rtc_disable(rtc);
    // time_base = (long long)sntp_time;
    // bflb_rtc_set_time(rtc, BFLB_RTC_SEC2TIME(0));
    printf("TIME SET\r\n");
    if (xSemaphoreTake(xMutex_RTC, portMAX_DELAY) == pdTRUE) {
        time = bflb_rtc_set_utc_time_sec(sntp_time);
        ins5699_set_time(&time);
        xSemaphoreGive(xMutex_RTC);
    }
}

static void EDA_task(void *pvParameters)
{
    // vTaskDelay(2000);
    uint32_t temp;
    uint16_t i;

    printf("EDA task enter \r\n");
    printf("EDA task start \r\n");

    // if (!isAD5940Init) {
    /* AD5940 INIT */
    AD5940_HWReset();
    vTaskDelay(100);
    printf("AD5940_HWReset\r\n");

    AD5940_Initialize();
    vTaskDelay(100);
    // int32_t i = AD5940_ReadReg(REG_AFECON_CHIPID);
    // printf("CHIPID read:0x%04x.\r\n", i);
    printf("AD5940_Initialize\r\n");
    AD5940PlatformCfg();
    vTaskDelay(100);
    printf("AD5940PlatformCfg\r\n");
    AD5940EDAStructInit();
    vTaskDelay(100);
    ResistorForBaseline = 100000;

    if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
        lv_obj_clear_flag(ui_AD5940, LV_OBJ_FLAG_HIDDEN);
        xSemaphoreGive(xMutex_lvgl);
    }
    err_code_ad5940 = AppEDAInit(AppBuff, APPBUFF_SIZE); /* Initialize BIA(EDA) application. Provide a buffer, which is used to store sequencer commands */
    if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
        lv_obj_add_flag(ui_AD5940, LV_OBJ_FLAG_HIDDEN);
        xSemaphoreGive(xMutex_lvgl);
    }

    vTaskDelay(500);
    printf("AppEDAInit:%d\r\n", err_code_ad5940);

    if (AD5940WarmUp == false) {
        if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
            lv_obj_clear_flag(ui_AD5940_WarmUp, LV_OBJ_FLAG_HIDDEN);
            xSemaphoreGive(xMutex_lvgl);
        }
        err_code_ad5940 = AppEDACtrl(APPCTRL_START, 0); /* Control BIA(EDA) measurement to start. Second parameter has no meaning with this command. */
        AppEDACtrl(EDACTRL_MEASVOLT, 0);
        printf("APPCTRL_START(WarmUp):%d\r\n", err_code_ad5940);
        // vTaskDelay(500);
        i = 0;
        while (1) {
            if (xSemaphoreTake(xSem_EDA, portMAX_DELAY) == pdTRUE) {
                AD5940_ClrMCUIntFlag(); /* Clear this flag */

                temp = APPBUFF_SIZE;

                err_code_ad5940 = -1;
                err_code_ad5940 = AppEDAISR(AppBuff, &temp); /* Deal with it and provide a buffer to store data we got */
                printf("%d AppEDAISR(WarmUp):%d\r\n", i, err_code_ad5940);

                // EDAShowResult(AppBuff, temp); /* Show the results to UART */
                i++;
            }
            if (i == 10) {
                break;
            }
            AppEDACtrl(EDACTRL_MEASVOLT, 0);
        }
        AppEDACtrl(APPCTRL_STOPNOW, 0);
        err_code_ad5940 = AppEDACtrl(APPCTRL_START, 0); /* Control BIA(EDA) measurement to start. Second parameter has no meaning with this command. */
        AD5940WarmUp = true;

        if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
            lv_obj_add_flag(ui_AD5940_WarmUp, LV_OBJ_FLAG_HIDDEN);
            xSemaphoreGive(xMutex_lvgl);
        }
    } else {
        err_code_ad5940 = AppEDACtrl(APPCTRL_START, 0); /* Control BIA(EDA) measurement to start. Second parameter has no meaning with this command. */
    }

    err_code_ad5940 = AppEDACtrl(EDACTRL_SETBASE, &EDABase);
    printf("EDACTRL_SETBASE:%d\r\n", err_code_ad5940);

    printf("EDA Started, begin to loop %s\r\n", __func__);

    while (1) {
        if (xSemaphoreTake(xSem_EDA, portMAX_DELAY) == pdTRUE) {
            AD5940_ClrMCUIntFlag(); /* Clear this flag */

            temp = APPBUFF_SIZE;

            err_code_ad5940 = -1;
            err_code_ad5940 = AppEDAISR(AppBuff, &temp); /* Deal with it and provide a buffer to store data we got */
            // printf("AppEDAISR:%d\r\n", err_code_ad5940);

            EDAShowResult(AppBuff, temp); /* Show the results to UART */
        }
        // vTaskDelay(100);
    }

    vTaskDelete(NULL);
}

void EDA_task_pre_delete()
{
    printf("EDA_task_pre_delete\r\n");

    if (EDA_handle != NULL) {
        while (uxSemaphoreGetCount(xMutex_SPI) == 0) {
            vTaskDelay(1);
            printf("while xMutex_SPI\r\n");
        }

        AD5940_HWReset();

        bflb_gpio_int_mask(gpio, KEY0, true);
        // bflb_gpio_int_clear(gpio, KEY0);

        while (uxSemaphoreGetCount(xMutex_SPI) == 0) {
            vTaskDelay(1);
            printf("while (uxSemaphoreGetCount\r\n");
        }
        printf("xMutex_SPI == 1");

        // while (uxQueueMessagesWaiting(Queue_EdaConFile) >= 4) {
        //     vTaskDelay(1);
        // }
        // while (uxQueueMessagesWaiting(Queue_EdaPhaFile) >= 4) {
        //     vTaskDelay(1);
        // }

        xQueueReset(Queue_EdaConFile);
        xQueueReset(Queue_EdaPhaFile);
        xQueueReset(Queue_EdaRealFile);
        xQueueReset(Queue_EdaImageFile);

        vTaskDelete(EDA_handle);
        printf("EDA task delete\r\n");

        // AD5940_HWReset();
        AD5940_CsSet();
        // xSemaphoreGive(xMutex_SPI);
        EDA_handle = NULL;
    }
}

void EDA_task_create()
{
    AD5940_HWReset();
    // AD5940_SoftRst();
    bflb_gpio_int_mask(gpio, KEY0, false);
    bflb_gpio_int_clear(gpio, KEY0);

    xSemaphoreGive(xSem_EDAEN);
    if (EDA_handle == NULL) {
        xTaskCreate(EDA_task, (char *)"EDA_task", 1024, NULL, configMAX_PRIORITIES - 4, &EDA_handle);
        printf("EDA task create\r\n");
    }
}

static void ACCE_task(void *pvParameters)
{
    printf("ACCE task enter \r\n");
    vTaskDelay(1000);
    lsm6dso_emb_sens_t emb_sens;
    uint8_t reg;
    uint8_t stepCounter = 0;

    /* Uncomment to configure INT 1 */
    // lsm6dso_pin_int1_route_t int1_route;
    // /* Uncomment to configure INT 2 */
    // //lsm6dso_pin_int2_route_t int2_route;
    // /* Initialize driver interface */
    // dev_ctx.write_reg = lsm6dso_platform_write;
    // dev_ctx.read_reg = lsm6dso_platform_read;
    /* Wait sensor boot time */
    platform_delay(LSM6DSO_BOOT_TIME);
    /* Check device ID */
    lsm6dso_device_id_get(&dev_ctx, &whoamI);

    if (whoamI != LSM6DSO_ID) {
        printf("LSM6DSO ADDRESS ERROR %04x\n", whoamI);
        while (1) {
            vTaskDelay(1);
        }
    }

    /* Restore default configuration */
    lsm6dso_reset_set(&dev_ctx, PROPERTY_ENABLE);

    do {
        lsm6dso_reset_get(&dev_ctx, &rst);
    } while (rst);

    /* Disable I3C interface */
    lsm6dso_i3c_disable_set(&dev_ctx, LSM6DSO_I3C_DISABLE);
    /* Set XL full scale */
    lsm6dso_xl_full_scale_set(&dev_ctx, LSM6DSO_4g);
    lsm6dso_gy_full_scale_set(&dev_ctx, LSM6DSO_1000dps);
    /* Enable Block Data Update */
    lsm6dso_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
    /* Enable latched interrupt notification. */
    //lsm6dso_int_notification_set(&dev_ctx, LSM6DSO_ALL_INT_LATCHED);
    /* Enable drdy 75 μs pulse: uncomment if interrupt must be pulsed. */
    lsm6dso_data_ready_mode_set(&dev_ctx, LSM6DSO_DRDY_PULSED);
    /*
   * Uncomment to configure INT 1
   * Remember that INT1 pin is used by sensor to switch in I3C mode
   */
    // lsm6dso_pin_int1_route_get(&dev_ctx, &int1_route);
    // int1_route.reg.emb_func_int1.int1_step_detector = PROPERTY_ENABLE;
    // int1_route.reg.emb_func_int1.lsm6dso_pin_int1_route_set(&dev_ctx, &int1_route);
    /* Uncomment to configure INT 2 */
    //lsm6dso_pin_int2_route_get(&dev_ctx, &int2_route);
    //int2_route.reg.emb_func_int2.int2_step_detector = PROPERTY_ENABLE;
    //lsm6dso_pin_int2_route_set(&dev_ctx, &int2_route);
    /* Enable xl sensor */
    lsm6dso_xl_data_rate_set(&dev_ctx, LSM6DSO_XL_ODR_26Hz);
    lsm6dso_gy_data_rate_set(&dev_ctx, LSM6DSO_GY_ODR_26Hz);

    lsm6dso_xl_hp_path_on_out_set(&dev_ctx, LSM6DSO_LP_ODR_DIV_100);
    lsm6dso_xl_filter_lp2_set(&dev_ctx, PROPERTY_ENABLE);
    /* Reset steps of pedometer */
    lsm6dso_steps_reset(&dev_ctx);
    /* Enable pedometer */
    lsm6dso_pedo_sens_set(&dev_ctx, LSM6DSO_FALSE_STEP_REJ_ADV_MODE);
    emb_sens.step = PROPERTY_ENABLE;
    emb_sens.step_adv = PROPERTY_ENABLE;
    lsm6dso_embedded_sens_set(&dev_ctx, &emb_sens);

    while (1) {
        /* Read output only if new xl value is available */
        lsm6dso_xl_flag_data_ready_get(&dev_ctx, &reg);

        if (reg) {
            /* Read acceleration field data */
            memset(data_raw_acceleration, 0x00, 3 * sizeof(int16_t));
            lsm6dso_acceleration_raw_get(&dev_ctx, data_raw_acceleration);
            acceleration_mg[0] =
                lsm6dso_from_fs4_to_mg(data_raw_acceleration[0]);
            acceleration_mg[1] =
                lsm6dso_from_fs4_to_mg(data_raw_acceleration[1]);
            acceleration_mg[2] =
                lsm6dso_from_fs4_to_mg(data_raw_acceleration[2]);
            // printf("Acceleration [mg]:%4.2f\t%4.2f\t%4.2f\r\n", acceleration_mg[0], acceleration_mg[1], acceleration_mg[2]);
            // xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
            // ui_UpdateAccelerationChart(acceleration_mg[0] * 0.016, acceleration_mg[1] * 0.016, acceleration_mg[2] * 0.016);
            // xSemaphoreGive(xMutex_lvgl);
            if (isBleConnected && isPlotAccInit) {
                xQueueSend(Queue_AccBLE_XL, acceleration_mg, 0);
                xQueueSend(Queue_AccBLE_XL, acceleration_mg + 1, 0);
                xQueueSend(Queue_AccBLE_XL, acceleration_mg + 2, 0);
            }
            xQueueSend(Queue_AccFile_XL_X, acceleration_mg, portMAX_DELAY);
            xQueueSend(Queue_AccFile_XL_Y, acceleration_mg + 1, portMAX_DELAY);
            xQueueSend(Queue_AccFile_XL_Z, acceleration_mg + 2, portMAX_DELAY);

            xQueueSend(Queue_AccPlot_XL_X, acceleration_mg, 0);
            xQueueSend(Queue_AccPlot_XL_Y, acceleration_mg + 1, 0);
            xQueueSend(Queue_AccPlot_XL_Z, acceleration_mg + 2, 0);
        }

        lsm6dso_gy_flag_data_ready_get(&dev_ctx, &reg);

        if (reg) {
            /* Read angular rate field data */
            memset(data_raw_angular_rate, 0x00, 3 * sizeof(int16_t));
            lsm6dso_angular_rate_raw_get(&dev_ctx, data_raw_angular_rate);
            angular_rate_mdps[0] =
                lsm6dso_from_fs1000_to_mdps(data_raw_angular_rate[0]);
            angular_rate_mdps[1] =
                lsm6dso_from_fs1000_to_mdps(data_raw_angular_rate[1]);
            angular_rate_mdps[2] =
                lsm6dso_from_fs1000_to_mdps(data_raw_angular_rate[2]);
            // printf("Angular rate [mdps]:%4.2f\t%4.2f\t%4.2f\r\n", angular_rate_mdps[0], angular_rate_mdps[1], angular_rate_mdps[2]);
            if (isBleConnected && isPlotAccInit) {
                xQueueSend(Queue_AccBLE_GY, angular_rate_mdps, 0);
                xQueueSend(Queue_AccBLE_GY, angular_rate_mdps + 1, 0);
                xQueueSend(Queue_AccBLE_GY, angular_rate_mdps + 2, 0);
            }
            xQueueSend(Queue_AccFile_GY_X, angular_rate_mdps, portMAX_DELAY);
            xQueueSend(Queue_AccFile_GY_Y, angular_rate_mdps + 1, portMAX_DELAY);
            xQueueSend(Queue_AccFile_GY_Z, angular_rate_mdps + 2, portMAX_DELAY);
            // printf("length Queue_AccFile_GY_Z %d\r\n", uxQueueMessagesWaiting(Queue_AccFile_GY_Z));
        }

        /* Read steps */
        if (stepCounter >= 200) {
            lsm6dso_number_of_steps_get(&dev_ctx, &stepCnt);

            if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
                ui_UpdateStepLabel(stepCnt);
                xSemaphoreGive(xMutex_lvgl);
            }
            stepCounter = 0;
        }
        stepCounter++;
        // printf("Step Count: %d\n", stepCnt);
        vTaskDelay(5);
    }

    vTaskDelete(NULL);
}

void ACCE_task_pre_delete()
{
    if (ACCE_handle != NULL) {
        while (uxSemaphoreGetCount(xMutex_IIC0) == 0) {
            vTaskDelay(1);
        }
        printf("xMutex_IIC0 == 1");

        vTaskDelete(ACCE_handle);
        printf("ACC task delete\r\n");

        xQueueReset(Queue_AccFile_XL_Z);
        xQueueReset(Queue_AccFile_GY_Z);
        xQueueReset(Queue_AccFile_XL_X);
        xQueueReset(Queue_AccFile_XL_Y);
        xQueueReset(Queue_AccFile_GY_X);
        xQueueReset(Queue_AccFile_GY_Y);

        lsm6dso_reset_set(&dev_ctx, PROPERTY_ENABLE);
        ACCE_handle = NULL;
    }
}

void ACCE_task_create()
{
    xSemaphoreGive(xSem_ACCEN);
    if (ACCE_handle == NULL) {
        xTaskCreate(ACCE_task, (char *)"ACCE_task", 2 * 1024, NULL, configMAX_PRIORITIES - 4, &ACCE_handle);
        printf("ACC task create\r\n");
    }
}

static void TEMP_task(void *pvParameters)
{
    printf("TEMP task enter \r\n");
    vTaskDelay(1000);
    printf("TEMP task start \r\n");
    printf("begin to loop %s\r\n", __func__);
    double pre_ambient, pre_object, ambient, object;
    int ret_code = 0;

    /* Check the internal version and prepare a clean start */
    ret_code = mlx90632_init();
    printf("mlx90632_init %d\n", ret_code);

    // ret_code = mlx90632_set_meas_type(MLX90632_MTYP_EXTENDED);
    // printf("MLX90632_MTYP_EXTENDED %d\r\n", ret_code);

    /* Definition of MLX90632 calibration parameters */
    int16_t ambient_new_raw;
    int16_t ambient_old_raw;
    int16_t object_new_raw;
    int16_t object_old_raw;
    int32_t PR = 0x00587f5b;
    int32_t PG = 0x04a10289;
    int32_t PT = 0xfff966f8;
    int32_t PO = 0x00001e0f;
    int32_t Ea = 4859535;
    int32_t Eb = 5686508;
    int32_t Fa = 53855361;
    int32_t Fb = 42874149;
    int32_t Ga = -14556410;
    int16_t Ha = 16384;
    int16_t Hb = 0;
    int16_t Gb = 9728;
    int16_t Ka = 10752;

    /* Read EEPROM calibration parameters */
    ret_code = mlx90632_set_meas_type(MLX90632_MTYP_MEDICAL_BURST);
    ret_code = mlx90632_read_eeprom(&PR, &PG, &PO, &PT, &Ea, &Eb, &Fa, &Fb, &Ga, &Gb, &Ha, &Hb, &Ka);

    mlx90632_set_emissivity(0.96);

    if (mlx90632_get_meas_type() != MLX90632_MTYP_MEDICAL)
        ret_code = mlx90632_set_meas_type(MLX90632_MTYP_MEDICAL);

    // printf("ret: %d\n", ret_code);
    while (1) {
        // mlx90632_set_meas_type(MLX90632_MTYP_MEDICAL);
        /* Get raw data from MLX90632 */
        mlx90632_read_temp_raw(&ambient_new_raw, &ambient_old_raw, &object_new_raw, &object_old_raw);
        /* Pre-calculations for ambient and object temperature calculation */
        pre_ambient = mlx90632_preprocess_temp_ambient(ambient_new_raw, ambient_old_raw, Gb);
        // printf("AMB %f\n", pre_ambient);
        pre_object = mlx90632_preprocess_temp_object(object_new_raw, object_old_raw, ambient_new_raw, ambient_old_raw, Ka);
        // /* Set emissivity = 1 */
        // mlx90632_set_emissivity(0.45);
        /* Calculate ambient and object temperature */
        ambient = mlx90632_calc_temp_ambient(ambient_new_raw, ambient_old_raw, PT, PR, PG, PO, Gb);
        object = mlx90632_calc_temp_object(pre_object, pre_ambient, Ea, Eb, Ga, Fa, Fb, Ha, Hb);
        // mlx90632_set_meas_type(MLX90632_MTYP_EXTENDED);

        // mlx90632_read_temp_raw_extended(&ambient_new_raw, &ambient_old_raw, &object_new_raw);
        // ambient = mlx90632_calc_temp_ambient_extended(ambient_new_raw, ambient_old_raw,
        //                                               PT, PR, PG, PO, Gb);
        // /* Get preprocessed temperatures needed for object temperature calculation */
        // pre_ambient = mlx90632_preprocess_temp_ambient_extended(ambient_new_raw,
        //                                                         ambient_old_raw, Gb);
        // pre_object = mlx90632_preprocess_temp_object_extended(object_new_raw, ambient_new_raw,
        //                                                       ambient_old_raw, Ka);

        // mlx90632_set_emissivity(1.0);
        // /* Calculate object temperature assuming the reflected temperature equals ambient*/
        // object = mlx90632_calc_temp_object_extended(pre_object, pre_ambient, ambient, Ea, Eb, Ga, Fa, Fb, Ha, Hb);

        // /* Set MLX90632 in extended burst mode */
        // ret_code = mlx90632_set_meas_type(MLX90632_MTYP_EXTENDED_BURST);

        // /* Now we read current ambient and object temperature */
        // ret_code = mlx90632_read_temp_raw_extended_burst(&ambient_new_raw, &ambient_old_raw, &object_new_raw);

        // /* Now start calculations (no more i2c accesses) */
        // /* Calculate ambient temperature */
        // ambient = mlx90632_calc_temp_ambient_extended(ambient_new_raw, ambient_old_raw,
        //                                               PT, PR, PG, PO, Gb);

        // /* Get preprocessed temperatures needed for object temperature calculation */
        // pre_ambient = mlx90632_preprocess_temp_ambient_extended(ambient_new_raw,
        //                                                         ambient_old_raw, Gb);
        // pre_object = mlx90632_preprocess_temp_object_extended(object_new_raw, ambient_new_raw,
        //                                                       ambient_old_raw, Ka);

        // mlx90632_set_emissivity(1.5);
        // /* Calculate object temperature assuming the reflected temperature equals ambient*/
        // object = mlx90632_calc_temp_object_extended(pre_object, pre_ambient, ambient, Ea, Eb, Ga, Fa, Fb, Ha, Hb);

        Tempurature = (float)object;
        TempuratureAmbient = (float)ambient;
        if (isBleConnected && isPlotTempInit) {
            xQueueSend(Queue_TempBLE, &Tempurature, portMAX_DELAY);
        }
        xQueueSend(Queue_TempFile_Ta, &TempuratureAmbient, portMAX_DELAY);
        xQueueSend(Queue_TempFile_To, &Tempurature, portMAX_DELAY);
        xQueueSend(Queue_TempPlot_To, &Tempurature, portMAX_DELAY);

        // printf("Tempurature=%f (ambient), %f (object)\r\n", ambient, object);
        // vTaskDelay(5);
    }

    vTaskDelete(NULL);
}

void TEMP_task_pre_delete()
{
    if (TEMP_handle != NULL) {
        while (uxSemaphoreGetCount(xMutex_IIC0) == 0) {
            vTaskDelay(1);
        }
        printf("xMutex_IIC0 == 1\r\n");

        // while (uxQueueMessagesWaiting(Queue_TempFile_To) >= 1) {
        //     vTaskDelay(1);
        // }
        // while (uxQueueMessagesWaiting(Queue_TempFile_Ta) >= 1) {
        //     vTaskDelay(1);
        // }

        xQueueReset(Queue_TempFile_To);
        xQueueReset(Queue_TempFile_Ta);

        vTaskDelete(TEMP_handle);
        printf("TEMP task delete\r\n");

        TEMP_handle = NULL;
    }
}

void TEMP_task_create()
{
    xSemaphoreGive(xSem_TEMPEN);
    if (TEMP_handle == NULL) {
        xTaskCreate(TEMP_task, (char *)"TEMP_task", 1024, NULL, configMAX_PRIORITIES - 4, &TEMP_handle);
        printf("TEMP task create\r\n");
    }
}

static void HR_task(void *pvParameters)
{
    printf("HR task enter \r\n");
    // vTaskDelay(1000);
    printf("HR task start \r\n");

    uint32_t NUMBER = 0;
    uint8_t uiUpdateCntHr = 0;
    uint8_t ADCRDY_count = 0;
    uint64_t startUs = 0;
    uint64_t deltaUs = 0;

    // /* AFE4404 INIT */
    initStatHRM();
    // AFE4404_RESETZ_Init(); // Included in AFE4404_PinInit
    AFE4404_Trigger_HWReset();
    AFE4404_Init();
    /* END AFE4404 INIT */

    /* AFE4404 */
    // AFE4404_Reg_Write(AFE_CONTROL0, 0x000000); // write mode
    AFE4404_Reg_Init();
    // AFE4404_Reg_Write(AFE_CONTROL0,0x000000); //write mode
    // AFE4404_Reg_Write(AFE_EXT_CLK_DIV_REG,0x04);      // CLKDIV_EXTMODE
    // AFE4404_Reg_Write(AFE_CONTROL0, 0x000001); // read mode

    AFE4404_ADCRDY_Interrupt_Enable();
    // Measure Frequency.
    startUs = bflb_mtimer_get_time_us();
    ADCRDY_count = 0;
    while (xSemaphoreTake(xSem_HR, portMAX_DELAY) == pdTRUE) {
        ADCRDY_count++;
        if (ADCRDY_count == 100)
            break;
    }
    deltaUs = bflb_mtimer_get_time_us() - startUs;
    afe4404_adcrdy_freq = 1000000.0000 / (deltaUs / 100.0);
    printf("afe4404_adcrdy_freq %.3f\r\n", afe4404_adcrdy_freq);

    CALIBRATION_ENABLED = 1;
    initCalibrationRoutine();
    offsetDACcalibFlag = 1;
    afe4404_adcrdy = 0;

    xSemaphoreGive(xSem_BVPEN);
    printf("begin to loop %s\r\n", __func__);
    while (1) {
        if (offsetDACcalibFlag == 0) {
            // printf("1\r\n");
            if (xSemaphoreTake(xSem_HR, portMAX_DELAY) == pdTRUE) {
                // printf("2\r\n");
                afe4404_adcrdy = 0;
                offsetDACcalibFlag = OFFSET_DAC_Code_Est(AFE4404_Reg_Read(45));
                printf("offsetDACcalibFlag %d\r\n", offsetDACcalibFlag);
                if (offsetDACcalibFlag)
                    AFE4404_ADCRDY_Interrupt_Enable(); // Disable DRDY interrupt
            }
        } else {
            offsetDACcalibFlag = 1;
            // if (afe4404_adcrdy) { // 读取数据
            if (xSemaphoreTake(xSem_HR, portMAX_DELAY) == pdTRUE) {
                afe4404_adcrdy = 0;
                AFE44xx_SPO2_Data_buf[0] = AFE4404_Reg_Read(42); //read LED2 Data
                AFE44xx_SPO2_Data_buf[1] = AFE4404_Reg_Read(43); //read LED3 data
                AFE44xx_SPO2_Data_buf[2] = AFE4404_Reg_Read(44); //read LED1 Data
                AFE44xx_SPO2_Data_buf[3] = AFE4404_Reg_Read(45); //read Ambient Data
                AFE44xx_SPO2_Data_buf[4] = AFE4404_Reg_Read(46); //read LED2 - LED3 Data
                AFE44xx_SPO2_Data_buf[5] = AFE4404_Reg_Read(47); //read LED1 - Ambient Data
                // AFE44xx_SPO2_Data_buf[5] = AFE4404_Reg_Read(64); //read LED1 - Ambient Data

                // printf("READ DATA\r\n");

                if (CALIBRATION_ENABLED == 1) { // 如果开启校准，进入校准过程
                    if (Calibration == 1) {     // 如果校准没有结束
                        if (LED_Sel == 2)
                            CalibrateAFE4404((long)AFE44xx_SPO2_Data_buf[0], (long)AFE44xx_SPO2_Data_buf[3]);
                        else if (LED_Sel == 3)
                            CalibrateAFE4404((long)AFE44xx_SPO2_Data_buf[1], (long)AFE44xx_SPO2_Data_buf[3]);
                        else // Default LED_Sel = 1
                            CalibrateAFE4404((long)AFE44xx_SPO2_Data_buf[2], (long)AFE44xx_SPO2_Data_buf[3]);
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
            if (xSemaphoreTake(xMutex_interArrayWindow, 0) == pdTRUE) {
                statHRMAlgo((long)AFE44xx_SPO2_Data_buf[5]);
                xSemaphoreGive(xMutex_interArrayWindow);
            }

            if (isPlotBvpInit && isBleConnected) {
                xQueueSend(Queue_HRBLE, AFE44xx_SPO2_Data_buf + 5, 0);
            }
            xQueueSend(Queue_BvpFile, AFE44xx_SPO2_Data_buf + 5, 0);
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
                // xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
                // ui_UpdateHRLabel(hr_transfer);
                // xSemaphoreGive(xMutex_lvgl);
                xQueueSend(Queue_HRLabelUpdate, &hr_transfer, 0);
                xQueueSend(Queue_HRFile, &hr_transfer, 0);
                NUMBER = 0;
                hr_transfer = 0;
            }
            if (uiUpdateCntHr >= 2) {
                // xSemaphoreTake(xMutex_lvgl, portMAX_DELAY);
                // ui_UpdatePPGChart(AFE44xx_SPO2_Data_buf[5], 0, 0);
                // xSemaphoreGive(xMutex_lvgl);
                xQueueSend(Queue_PPGChartUpdate, AFE44xx_SPO2_Data_buf + 5, 0);
                uiUpdateCntHr = 0;
            }
            if (isAlgoOn == true) {
                xQueueSend(Queue_BvpAlgo, AFE44xx_SPO2_Data_buf + 5, 0);
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
        // vTaskDelay(5);
    }

    vTaskDelete(NULL);
}

void HR_task_pre_delete()
{
    if (HR_handle != NULL) {
        bflb_gpio_int_mask(gpio, AFE_ADC_DRDY, true);
        bflb_gpio_int_clear(gpio, AFE_ADC_DRDY);

        // printf("AFE_ADC_DRDY\r\n");

        while (uxSemaphoreGetCount(xSem_HR) == 1) {
            vTaskDelay(1);
            printf("uxSemaphoreGetCount(xSem_HR) == 1\r\n");
        }
        printf("xSem_HR == 0\r\n");

        while (uxSemaphoreGetCount(xMutex_IIC0) == 0) {
            vTaskDelay(1);
        }
        printf("HR xMutex_IIC0 == 1\r\n");

        xQueueReset(Queue_BvpFile);
        xQueueReset(Queue_HRFile);

        vTaskDelete(HR_handle);
        printf("HR task delete\r\n");

        AFE4404_Trigger_HWReset();

        HR_handle = NULL;
    }
}

void HR_task_create()
{
    bflb_gpio_int_mask(gpio, AFE_ADC_DRDY, false);
    bflb_gpio_int_clear(gpio, AFE_ADC_DRDY);

    if (HR_handle == NULL) {
        xTaskCreate(HR_task, (char *)"HR_task", 1024, NULL, configMAX_PRIORITIES - 4, &HR_handle);
        printf("HR task create\r\n");
    }
}

static void BLE_trans_task(void *pvParameters)
{
    int32_t data[20] = { 0 };
    uint8_t cnt = 0;

    while (1) {
        // Send BVP
        if (uxQueueMessagesWaiting(Queue_HRBLE) >= 20) {
            cnt = 0;
            while (xQueueReceive(Queue_HRBLE, data + (cnt++), portMAX_DELAY) == pdTRUE) {
                if (cnt == 20) {
                    break;
                }
            }
            if (xSemaphoreTake(xMutex_BLE, portMAX_DELAY) == pdTRUE) {
                bt_gatt_plotbvp_update(data);
                xSemaphoreGive(xMutex_BLE);
            }
        }
        // Send Acc
        if ((uxQueueMessagesWaiting(Queue_AccBLE_GY) >= 6) && (uxQueueMessagesWaiting(Queue_AccBLE_XL) >= 6)) {
            cnt = 0;
            while (xQueueReceive(Queue_AccBLE_XL, data + (cnt++), portMAX_DELAY) == pdTRUE) {
                if (cnt == 6) {
                    break;
                }
            }
            while (xQueueReceive(Queue_AccBLE_GY, data + (cnt++), portMAX_DELAY) == pdTRUE) {
                if (cnt == 12) {
                    break;
                }
            }
            if (xSemaphoreTake(xMutex_BLE, portMAX_DELAY) == pdTRUE) {
                bt_gatt_plotacc_update(data);
                xSemaphoreGive(xMutex_BLE);
            }
        }
        // Send Temp
        if (uxQueueMessagesWaiting(Queue_TempBLE) >= 1) {
            xQueueReceive(Queue_TempBLE, data, portMAX_DELAY);
            if (xSemaphoreTake(xMutex_BLE, portMAX_DELAY) == pdTRUE) {
                bt_gatt_plottemp_update(data);
                xSemaphoreGive(xMutex_BLE);
            }
        }
        // Send Eda
        if (uxQueueMessagesWaiting(Queue_EdaBLE) >= 4) {
            xQueueReceive(Queue_EdaBLE, data, portMAX_DELAY);
            xQueueReceive(Queue_EdaBLE, data + 1, portMAX_DELAY);
            xQueueReceive(Queue_EdaBLE, data + 2, portMAX_DELAY);
            xQueueReceive(Queue_EdaBLE, data + 3, portMAX_DELAY);
            if (xSemaphoreTake(xMutex_BLE, portMAX_DELAY) == pdTRUE) {
                bt_gatt_ploteda_update(data, 4);
                xSemaphoreGive(xMutex_BLE);
            }
        }

        vTaskDelay(1);
    }
    vTaskDelete(NULL);
}

static void LVGL_task(void *pvParameters)
{
    uint32_t hrLabel = 0;
    uint8_t AlgoLabel = 0;
    int32_t ppgSignal = 0;
    uint16_t ppgNum = 0;
    float AccX = 0;
    float AccY = 0;
    float AccZ = 0;
    float Temp = 0;
    static bool isTouchLocked = false;

    printf("LVGL task enter \r\n");
    vTaskDelay(1);
    printf("LVGL task start \r\n");
    printf("begin to loop %s\r\n", __func__);

    // lv_task_handler();
    while (1) {
        if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
            if (uxQueueMessagesWaiting(Queue_HRLabelUpdate) > 0) {
                xQueueReceive(Queue_HRLabelUpdate, &hrLabel, 0);
                ui_UpdateHRLabel((uint16_t)hrLabel);
                xQueueReset(Queue_HRLabelUpdate);
            }

            if (uxQueueMessagesWaiting(Queue_AlgoPlot) > 0) {
                xQueueReceive(Queue_AlgoPlot, &AlgoLabel, 0);
                ui_UpdateAlgoChart(AlgoLabel);
            }

            if (xSemaphoreTake(xSem_BTNINT, 0) == pdTRUE) {
                if (isTouchLocked == false) {
                    isTouchLocked = true;
                    ui_back2home();
                    touchpad_remove();
                } else {
                    isTouchLocked = false;
                    touchpad_restore();
                }
                // printf("xSem_BTNINT %d\r\n", isTouchLocked);
                ui_UpdateTouchLock(isTouchLocked);
            }

            if ((uxQueueMessagesWaiting(Queue_AccPlot_XL_X) >= 4) && (uxQueueMessagesWaiting(Queue_AccPlot_XL_Y) >= 4) && (uxQueueMessagesWaiting(Queue_AccPlot_XL_Z) >= 4)) {
                for (int8_t i = 0; i < 4; i++) {
                    xQueueReceive(Queue_AccPlot_XL_X, &AccX, portMAX_DELAY);
                    xQueueReceive(Queue_AccPlot_XL_Y, &AccY, portMAX_DELAY);
                    xQueueReceive(Queue_AccPlot_XL_Z, &AccZ, portMAX_DELAY);
                    ui_UpdateAccelerationChart(AccX, AccY, AccZ);
                }
            }

            if (uxQueueMessagesWaiting(Queue_TempPlot_To) > 0) {
                xQueueReceive(Queue_TempPlot_To, &Temp, portMAX_DELAY);
                ui_UpdateTemperatureChart(Temp);
                ui_UpdateTempLabel(Temp);
            }

            ppgNum = 2;
            while (ppgNum) {
                xQueueReceive(Queue_PPGChartUpdate, &ppgSignal, 0);
                ui_UpdatePPGChart(ppgSignal, 0, 0);
                ppgNum--;
            }
            if (wifi_connected) {
                ui_setWifiImg(1);
            } else {
                ui_setWifiImg(0);
            }
            lv_task_handler();
            xSemaphoreGive(xMutex_lvgl);
        }
        vTaskDelay(20);
    }
    vTaskDelete(NULL);
}

static void CLOCK_task(void *pvParameters)
{
    printf("CLOCK task enter \r\n");
    vTaskDelay(10);
    bflb_rtc_set_time(rtc, BFLB_RTC_SEC2TIME(0));
    long long rtc_sec = 0;
    uint8_t rtc_sec0 = 0;
    uint8_t rtc_min = 0;
    uint8_t rtc_hour = 0;
    uint8_t rtc_mon = 0;
    uint8_t rtc_date = 0;
    uint8_t rtc_week = 7;
    uint16_t rtc_year = 0;
    struct bflb_tm time;
    int reset = 0;

    printf("CLOCK task start \r\n");

    ins5699_init_client(&reset);

    ins5699_get_time(&time);      // get time from rtc ins5699.
    bflb_rtc_set_utc_time(&time); // set time into on-chip rtc.

    while (1) {
        if (xSemaphoreTake(xMutex_RTC, portMAX_DELAY) == pdTRUE) {
            bflb_rtc_get_local_time(&time, +8); // get utc +8 time.
            xSemaphoreGive(xMutex_RTC);
        }

        // update label.
        if (time.tm_sec != rtc_sec0) {
            rtc_sec0 = time.tm_sec;
            if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
                ui_UpdateSecLabel(time.tm_sec);
                xSemaphoreGive(xMutex_lvgl);
            }
        }

        if (time.tm_min != rtc_min) {
            rtc_min = time.tm_min;
            if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
                ui_UpdateMinLabel(rtc_min);
                xSemaphoreGive(xMutex_lvgl);
            }
        }

        if (time.tm_hour != rtc_hour) {
            rtc_hour = time.tm_hour;
            if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
                ui_UpdateHourLabel(rtc_hour);
                xSemaphoreGive(xMutex_lvgl);
            }
        }

        if (time.tm_mday != rtc_date) {
            rtc_date = time.tm_mday;
            if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
                ui_UpdateDateLabel(rtc_date);
                xSemaphoreGive(xMutex_lvgl);
            }
        }

        if (time.tm_mon != rtc_mon) {
            rtc_mon = time.tm_mon;
            if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
                ui_UpdateMonLabel(rtc_mon + 1);
                xSemaphoreGive(xMutex_lvgl);
            }
        }

        if (time.tm_year != rtc_year) {
            rtc_year = time.tm_year;
            if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
                ui_UpdateYearLabel(rtc_year + 1900);
                xSemaphoreGive(xMutex_lvgl);
            }
        }

        if (time.tm_wday != rtc_week) {
            rtc_week = time.tm_wday;
            if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
                ui_UpdateWeekLabel(rtc_week);
                xSemaphoreGive(xMutex_lvgl);
            }
        }

        // sntp set time.
        if (wifi_connected && !timeCorrected) {
            sntp_setoperatingmode(SNTP_OPMODE_POLL);
            sntp_init();
            sntp_setservername(0, "pool.ntp.org");
            sntp_setservername(1, "ntp1.aliyun.com");
            sntp_setservername(2, "158.69.48.97");
            timeCorrected = true;
        }
        if (!wifi_connected && timeCorrected) {
            sntp_stop();
            timeCorrected = false;
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
    vTaskDelay(20000);

    wifi_mgmr_sta_quickconnect("cgh1", "asdfghjk", 2400, 5000);

    while (1) {
        if (!wifi_connected) {
            wifi_mgmr_sta_quickconnect("cgh1", "asdfghjk", 2400, 5000); // Non-blocking.
            vTaskDelay(20000);
        } else {
            // onenet_transfer(Tempurature, hr_onenet, output_class);
            // // if(eda_transfer_flag){
            // //     eda_transfer_flag = 0;
            // //     onenet_transfer_GSR(EDAwindow_transfer_filtered, 240);
            // // }

            if (weatherCounter >= 6) {
                memset(weatherCityName, 0, 10);
                memset(weatherText, 0, 10);
                memset(weatherTemp, 0, 10);
                weather_get("nanjing", weatherCityName, weatherText, &weatherCode, weatherTemp);
                // weather_get("ip", weatherCityName, weatherText, &weatherCode, weatherTemp);
                if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
                    ui_updateWeather(weatherCityName, weatherText, weatherCode, weatherTemp);
                    xSemaphoreGive(xMutex_lvgl);
                }
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
    uint8_t resultArray[350] = { 0 };
    uint8_t result = 0;
    int16_t accurateNumber = 0;
    int64_t meanBVP = 0;
    uint64_t startMs = 0;
    uint32_t deltaMs = 0;
    uint32_t sumMs = 0;

    uint16_t pointerBvpO = 0;
    uint16_t pointerEdaO = 0;

    printf("Algo task enter \r\n");
    vTaskDelay(20);

    printf("Algo task start \r\n");

    setup();
    printf("model load successfully!!\r\n");

    sumMs = 0;
    // DEMO
    vTaskDelay(5000);
    for (uint16_t i = 0; i < 350; i++) {
        startMs = bflb_mtimer_get_time_ms();
        // resultArray[i] = Calculate(test_data_input[i], 2720);
        resultArray[i] = Calculate2(test_data_input[i], test_data_input[i] + 2560, 2560, 160);
        deltaMs = bflb_mtimer_get_time_ms() - startMs;
        sumMs += deltaMs;
        printf("%d, time %d\r\n", resultArray[i], deltaMs);
        if (resultArray[i] == test_label[i]) {
            accurateNumber++;
        }
        vTaskDelay(1);
    }
    printf("accurateNumber %d\r\n", accurateNumber);
    printf("Avg Time %d\r\n", sumMs);

    isAlgoOn = true;
    while (1) {
        if ((uxQueueMessagesWaiting(Queue_BvpAlgo) >= 320) && (uxQueueMessagesWaiting(Queue_EdaAlgo) >= 20)) {
            // printf("Enter\r\n");
            if (pointerBvpO < 64 * 40) {
                for (int16_t i = pointerBvpO; i < pointerBvpO + 320; i++) {
                    xQueueReceive(Queue_BvpAlgo, BVP_Window_ori + i, 0);
                    // printf("{Bvp} %d\r\n", *(BVP_Window_ori + i));
                }

                if (pointerBvpO == 0) {
                    meanBVP = 0;
                    for (int16_t i = 0; i < 64; i++) {
                        meanBVP += BVP_Window_ori[i];
                    }
                } else {
                    meanBVP = 0;
                    for (int16_t i = pointerBvpO - 32 - 32; i < pointerBvpO; i++) {
                        meanBVP += BVP_Window_ori[i];
                    }
                }

                for (int16_t i = pointerBvpO - 32; i < pointerBvpO + 320; i++) {
                    if (i < 32) {
                        BVP_Window_scale[i] = BVP_Window_ori[i] - meanBVP / 64;
                    } else if (i < pointerBvpO + 320 - 32) {
                        BVP_Window_scale[i] = BVP_Window_ori[i] - meanBVP / 64;
                        meanBVP -= BVP_Window_ori[i - 32];
                        meanBVP += BVP_Window_ori[i + 32];
                        // BVP_Window_scale[i] = BVP_Window_ori[i] - mean(i-32, i+31);
                    } else {
                        // BVP_Window_scale[i] = BVP_Window_ori[i] - mean(pointerBvpO + 320 - 1 - 63, pointerBvpO + 320 - 1);
                        BVP_Window_scale[i] = BVP_Window_ori[i] - meanBVP / 64;
                    }
                    // printf("{BVP_Window_scale} %d\r\n", BVP_Window_scale[i]);
                }
                pointerBvpO += 320;
            } else {
                for (int16_t i = 0, j = 64 * 5; j < 64 * 40; i++, j++) {
                    BVP_Window_ori[i] = BVP_Window_ori[j];
                    BVP_Window_scale[i] = BVP_Window_scale[j];
                }
                pointerBvpO = 64 * 35;
                while (pointerBvpO < 64 * 40) {
                    // printf("pointerBvpO %d\r\n", pointerBvpO);
                    xQueueReceive(Queue_BvpAlgo, BVP_Window_ori + pointerBvpO, 0);
                    // printf("{Bvp} %d\r\n", *(BVP_Window_ori + pointerBvpO));
                    pointerBvpO++;
                }

                meanBVP = 0;
                for (int16_t i = 64 * 35 - 32 - 32; i < 64 * 35; i++) {
                    meanBVP += BVP_Window_ori[i];
                }

                for (int16_t i = 64 * 35 - 32; i < pointerBvpO; i++) {
                    if (i < pointerBvpO - 32) {
                        // BVP_Window_scale[i] = BVP_Window_ori[i] - mean(i - 32, i + 31);
                        BVP_Window_scale[i] = BVP_Window_ori[i] - meanBVP / 64;
                        meanBVP -= BVP_Window_ori[i - 32];
                        meanBVP += BVP_Window_ori[i + 32];
                    } else {
                        // BVP_Window_scale[i] = BVP_Window_ori[i] - mean(pointerBvpO - 1 - 63, pointerBvpO - 1);
                        BVP_Window_scale[i] = BVP_Window_ori[i] - meanBVP / 64;
                    }
                    // printf("{BVP_Window_scale} %d\r\n", BVP_Window_scale[i]);
                }
            }

            if (pointerEdaO < 4 * 40) {
                for (int i = pointerEdaO; i < pointerEdaO + 20; i++) {
                    xQueueReceive(Queue_EdaAlgo, EDA_Window_ori + i, 0);
                    // printf("{Eda} %.4f\r\n", *(EDA_Window_ori + i));
                }
                pointerEdaO += 20;
            } else {
                for (int16_t i = 0, j = 4 * 5; j < 4 * 40; i++, j++) {
                    EDA_Window_ori[i] = EDA_Window_ori[j];
                }
                pointerEdaO = 4 * 35;
                while (pointerEdaO < 4 * 40) {
                    xQueueReceive(Queue_EdaAlgo, EDA_Window_ori + pointerEdaO, 0);
                    // printf("pointerEdaO %d\r\n", pointerEdaO);
                    // printf("{Eda} %.4f\r\n", *(EDA_Window_ori + pointerEdaO));
                    pointerEdaO++;
                }
            }

            if ((pointerEdaO >= 4 * 40) && (pointerBvpO >= 64 * 40)) {
                // Scale.
                printf("Calculate\r\n");
                for (int16_t i = 0; i < 4 * 40; i++) {
                    EDA_Window_scale[i] = (float)(EDA_Window_ori[i] - 13.0) / 30.0f;
                }

                for (int16_t i = 0; i < 64 * 40; i++) {
                    BVP_Window_scale2[i] = (float)(BVP_Window_scale[i] / 80000.0f);
                }

                // Calculate on model.
                result = Calculate2(BVP_Window_scale2, EDA_Window_scale, 2560, 160);
                printf("{result} %d\r\n", result);
                output_class = result;
                xQueueSend(Queue_AlgoFile, &result, portMAX_DELAY);
                xQueueSend(Queue_AlgoPlot, &result, portMAX_DELAY);
            }
        }

        vTaskDelay(1);
    }

    vTaskDelete(NULL);
}

void Algo_task_pre_delete()
{
    if (Algo_handle != NULL) {
        isAlgoOn = false;

        xQueueReset(Queue_BvpAlgo);
        xQueueReset(Queue_EdaAlgo);

        vTaskDelete(Algo_handle);
        printf("Algo task delete\r\n");

        Algo_handle = NULL;
    }
}

void Algo_task_create()
{
    xQueueReset(Queue_BvpAlgo);
    xQueueReset(Queue_EdaAlgo);

    if (Algo_handle == NULL) {
        xSemaphoreGive(xSem_ALGOEN);
        xTaskCreate(Algo_task, (char *)"Algo_task", 5 * 1024, NULL, configMAX_PRIORITIES - 5, &Algo_handle);
        printf("Algo task create\r\n");
    }
}

static void BLE_task(void *pvParameters)
{
    // char tempArray[5] = { 0 };
    // // hrs_init();
    // // bas_init();
    // mysvc_init();

    // while (1) {
    //     // bt_gatt_bas_set_battery_level(5);
    //     // bt_hrs_notify(HeartRate);
    //     sprintf(tempArray, "%.1f", Tempurature);
    //     bt_gatt_mysvc_update(hr_onenet, stepCnt, tempArray, output_class);
    //     vTaskDelay(250);
    // }
    // vTaskDelete(NULL);
    char tempArray[5] = { 0 };
    // hrs_init();
    // bas_init();
    cts_init();
    mysvc_init();
    ploteda_init();
    plotbvp_init();
    plotacc_init();
    plottemp_init();
    isPlotedaInit = true;
    isPlotBvpInit = true;
    isPlotTempInit = true;
    isPlotAccInit = true;

    while (1) {
        // bt_gatt_bas_set_battery_level(5);
        // bt_hrs_notify(HeartRate);
        if (isBleConnected) {
            sprintf(tempArray, "%.1f", Tempurature);
            if (xSemaphoreTake(xMutex_BLE, portMAX_DELAY) == pdTRUE) {
                bt_gatt_mysvc_update(hr_onenet, stepCnt, tempArray, output_class);
                xSemaphoreGive(xMutex_BLE);
            }
        }
        vTaskDelay(250);
    }
    vTaskDelete(NULL);
}

static void MOTOR_task(void *pvParameters)
{
    bflb_pwm_v2_channel_set_threshold(pwm, PWM_CH2, 10, 90); /* duty = (90-10)/100 = 80% */
    bflb_pwm_v2_channel_positive_start(pwm, PWM_CH2);

    // while (1) {
    // if (xSemaphoreTake(xSem_MOTOR, portMAX_DELAY) == pdTRUE) {
    bflb_pwm_v2_start(pwm);
    bflb_gpio_set(gpio, GPIO_PIN_13);

    vTaskDelay(250);
    bflb_pwm_v2_stop(pwm);
    vTaskDelay(20); // break.
    bflb_gpio_reset(gpio, GPIO_PIN_13);
    // bflb_pwm_v2_stop(pwm);
    // }
    // }
    vTaskDelete(NULL);
}

static void BAT_task(void *pvParameters)
{
    printf("BAT TASK\r\n");
    vTaskDelay(20);
    printf("MAX17048 VERSION: %04x\r\n", max17048_get_version());

    if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
        ui_updateBat((uint8_t)max17048_get_SOC());
        xSemaphoreGive(xMutex_lvgl);
    }

    if (bflb_gpio_read(gpio, GPIO_PIN_33)) { // 1:2 Not Charging
        CHG_Interrupted = 2;
    } else { // 0:1 Charging
        CHG_Interrupted = 1;
    }

    while (1) {
        if (CHG_Interrupted) {
            if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
                ui_updateBatColor(CHG_Interrupted, (uint8_t)max17048_get_SOC());
                xSemaphoreGive(xMutex_lvgl);
            }
            CHG_Interrupted = 0;
        }

        if (xSemaphoreTake(xMutex_lvgl, portMAX_DELAY) == pdTRUE) {
            ui_updateBat((uint8_t)max17048_get_SOC());
            xSemaphoreGive(xMutex_lvgl);
        }
        // printf("CHG %d\r\n", bflb_gpio_read(gpio, GPIO_PIN_33));
        // printf("kfree_size %d, pfree_size %d\r\n", kfree_size(), pfree_size());

        vTaskDelay(250);
    }
    vTaskDelete(NULL);
}

#define SDU_DATA_CHECK 1

char test_data[] =
    "I've been reading books of old \r\n\
    The legends and the myths \r\n\
    Achilles and his gold \r\n\
    Hercules and his gifts \r\n\
    Spiderman's control \r\n\
    And Batman with his fists\r\n\
    And clearly I don't see myself upon that list\r\n\
    But she said, where'd you wanna go?\r\n\
    How much you wanna risk?\r\n\
    I'm not looking for somebody\r\n\
    With some superhuman gifts\r\n\
    Some superhero\r\n\
    Some fairytale bliss\r\n\
    Just something I can turn to\r\n\
    Somebody I can kiss\r\n\
    I want something just like this\r\n\r\n";

ATTR_NOCACHE_NOINIT_RAM_SECTION __attribute((aligned(64))) BYTE RW_Buffer[4 * 1024] = { 0 };
#if SDU_DATA_CHECK
__attribute((aligned(64))) BYTE Check_Buffer[sizeof(RW_Buffer)] = { 0 };
#endif

ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) FIL fnew;

void fatfs_write_read_test()
{
    FRESULT ret;
    UINT fnum;

    vTaskDelay(10);

    uint32_t time_node, i, j;

    /* full test data to buff */
    for (uint32_t cnt = 0; cnt < (sizeof(RW_Buffer) / sizeof(test_data)); cnt++) {
        memcpy(&RW_Buffer[cnt * sizeof(test_data)], test_data, sizeof(test_data));
#if SDU_DATA_CHECK
        memcpy(&Check_Buffer[cnt * sizeof(test_data)], test_data, sizeof(test_data));
#endif
    }

    /* write test */
    LOG_RI("\r\n");
    LOG_I("******************** be about to write test... **********************\r\n");
    ret = f_open(&fnew, "/sd2/test_file.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if (ret == FR_OK) {
        time_node = (uint32_t)bflb_mtimer_get_time_ms();
        /*write into file*/
        // ret = f_write(&fnew, RW_Buffer, 1024, &fnum);
        for (i = 0; i < 1024; i++) {
            ret = f_write(&fnew, RW_Buffer, sizeof(RW_Buffer), &fnum);
            if (ret) {
                break;
            }
        }

        /* close file */
        ret |= f_close(&fnew);
        /* get time */
        time_node = (uint32_t)bflb_mtimer_get_time_ms() - time_node;

        if (ret == FR_OK) {
            LOG_I("Write Test Succeed! \r\n");
            LOG_I("Single data size:%d Byte, Write the number:%d, Total size:%d KB\r\n", sizeof(RW_Buffer), i, sizeof(RW_Buffer) * i >> 10);
            LOG_I("Time:%dms, Write Speed:%d KB/s \r\n", time_node, ((sizeof(RW_Buffer) * i) >> 10) * 1000 / time_node);
        } else {
            LOG_F("Fail to write files(%d) num:%d\n", ret, i);
            return;
        }
    } else {
        LOG_F("Fail to open or create files: %d.\r\n", ret);
        return;
    }

    /* read test */
    LOG_RI("\r\n");
    LOG_I("******************** be about to read test... **********************\r\n");
    ret = f_open(&fnew, "/sd2/test_file.txt", FA_OPEN_EXISTING | FA_READ);
    if (ret == FR_OK) {
        time_node = (uint32_t)bflb_mtimer_get_time_ms();

        // ret = f_read(&fnew, RW_Buffer, 1024, &fnum);
        for (i = 0; i < 1024; i++) {
            ret = f_read(&fnew, RW_Buffer, sizeof(RW_Buffer), &fnum);
            if (ret) {
                break;
            }
        }
        /* close file */
        ret |= f_close(&fnew);
        /* get time */
        time_node = (uint32_t)bflb_mtimer_get_time_ms() - time_node;

        if (ret == FR_OK) {
            LOG_I("Read Test Succeed! \r\n");
            LOG_I("Single data size:%dByte, Read the number:%d, Total size:%d KB\r\n", sizeof(RW_Buffer), i, sizeof(RW_Buffer) * i >> 10);
            LOG_I("Time:%dms, Read Speed:%d KB/s \r\n", time_node, ((sizeof(RW_Buffer) * i) >> 10) * 1000 / time_node);
        } else {
            LOG_F("Fail to read file: (%d), num:%d\n", ret, i);
            return;
        }
    } else {
        LOG_F("Fail to open files.\r\n");
        return;
    }

    /* check data */
#if SDU_DATA_CHECK
    LOG_RI("\r\n");
    LOG_I("******************** be about to check test... **********************\r\n");
    ret = f_open(&fnew, "/sd2/test_file.txt", FA_OPEN_EXISTING | FA_READ);
    if (ret == FR_OK) {
        // ret = f_read(&fnew, RW_Buffer, 1024, &fnum);
        for (i = 0; i < 1024; i++) {
            memset(RW_Buffer, 0x55, sizeof(RW_Buffer));
            ret = f_read(&fnew, RW_Buffer, sizeof(RW_Buffer), &fnum);
            if (ret) {
                break;
            }
            for (j = 0; j < sizeof(RW_Buffer); j++) {
                if (RW_Buffer[j] != Check_Buffer[j]) {
                    LOG_I(" RW_Buffer: %c, Check_Buffer %c\r\n", RW_Buffer[j], Check_Buffer[j]);
                    LOG_I("Data Error!  Num:%d/1024, Byte:%d/%d\r\n", i, j, sizeof(RW_Buffer));
                    break;
                }
            }
            if (j < sizeof(RW_Buffer)) {
                break;
            }
        }
        /* close file */
        ret |= f_close(&fnew);

        if (ret == FR_OK) {
            if (i < 1024 || j < sizeof(RW_Buffer)) {
                LOG_I("Check Test Error! \r\n");
                LOG_I("Data Error!  Num:%d/1024, Byte:%d/%d", i, j, sizeof(RW_Buffer));
            } else {
                LOG_I("Check Test Succeed! \r\n");
                LOG_I("All Data Is Good! \r\n");
            }

        } else {
            LOG_F("Fail to read file: (%d), num:%d\n", ret, i);
            return;
        }
    } else {
        LOG_F("Fail to open files.\r\n");
        return;
    }
#endif
}

extern void msc_ram_init(void);

void usbd_event_handler(uint8_t event)
{
    printf("USBD_EVENT %d\r\n", event);
    switch (event) {
        case USBD_EVENT_RESET:
            // if (HR_handle != NULL) {
            //     vTaskDelete(HR_handle);
            // }
            break;
        case USBD_EVENT_CONNECTED:
            break;
        case USBD_EVENT_DISCONNECTED:
            break;
        case USBD_EVENT_RESUME:
            break;
        case USBD_EVENT_SUSPEND:
            break;
        case USBD_EVENT_CONFIGURED:
            break;
        case USBD_EVENT_SET_REMOTE_WAKEUP:
            break;
        case USBD_EVENT_CLR_REMOTE_WAKEUP:
            break;

        default:
            break;
    }
}

static void FILE_task(void *pvParameters)
{
    printf("FILE TASK\r\n");
    struct bflb_tm time;

    vTaskDelay(10);
    // RW_Buffer[0] = 11;
    // RW_Buffer[1] = 129;
    // RW_Buffer[2] = 130;
    // RW_Buffer[511] = 120;
    // RW_Buffer[512] = 121;
    // RW_Buffer[1020] = 128;
    // RW_Buffer[1024] = 127;
    // RW_Buffer[2047] = 13;

    // while (NAND_WriteMultiBlocks(RW_Buffer, 0, 512, 4) == 1)
    //     ;
    // while (NAND_WriteMultiBlocks(RW_Buffer, 4, 512, 16) == 1)
    //     ;
    // while (NAND_WriteMultiBlocks(RW_Buffer, 8, 512, 1) == 1)
    //     ;
    // while (NAND_WriteMultiBlocks(RW_Buffer, 12, 512, 1) == 1)
    //     ;
    // while (NAND_WriteMultiBlocks(RW_Buffer, 16, 512, 1) == 1)
    //     ;
    // while (NAND_WriteMultiBlocks(RW_Buffer, 32, 512, 1) == 1)
    //     ;
    // while (NAND_WriteMultiBlocks(RW_Buffer, 63, 512, 1) == 1)
    //     ;
    // // printf("NAND_Write\r\n");
    // while (NAND_ReadMultiBlocks(RW_Buffer, 0, 512, 1) == 1)
    //     ;

    // for (uint8_t i = 0; i < 64; i++) {
    //     printf("Read %d\r\n", RW_Buffer[i]);
    // }
    // printf("Read end1020 %d\r\n", RW_Buffer[1020]);
    // printf("Read end1024 %d\r\n", RW_Buffer[1024]);
    // printf("Read end2047 %d\r\n", RW_Buffer[2047]);
    // printf("Read end511 %d\r\n", RW_Buffer[511]);
    // printf("Read end512 %d\r\n", RW_Buffer[512]);
    // while (NAND_WriteMultiBlocks(RW_Buffer, 95, 512, 32) == 1)
    //     ;
    // while (NAND_WriteMultiBlocks(RW_Buffer, 95, 512, 128) == 1)
    //     ;

    // printf("TEST END\r\n");

    // INIT
    FRESULT ret;
    UINT fnum;
    uint32_t edaFileCnt = 0;
    uint32_t tempFileCnt = 0;
    uint32_t bvpFileCnt = 0;
    uint32_t accFileCnt = 0;
    uint32_t AlgoFileCnt = 0;
    float edaBuffer[4] = { 0 };
    float edaPhaBuffer[4] = { 0 };
    float edaRealBuffer[4] = { 0 };
    float edaImageBuffer[4] = { 0 };
    int32_t bvpBuffer = 0;
    int32_t hrBuffer = 0;
    int8_t AlgoBuffer = 0;
    float accXLxBuffer[26] = { 0 };
    float accXLyBuffer[26] = { 0 };
    float accXLzBuffer[26] = { 0 };
    float accGYxBuffer[26] = { 0 };
    float accGYyBuffer[26] = { 0 };
    float accGYzBuffer[26] = { 0 };
    float tempToBuffer = { 0 };
    float tempTaBuffer = { 0 };
    char FileNameEda[128] = { 0 };
    char FileNameTemp[128] = { 0 };
    char FileNameBvp[128] = { 0 };
    char FileNameAcc[128] = { 0 };
    char FileNameAlgo[128] = { 0 };
    char tempBuffer[1024] = { 0 };

    fatfs_nand_driver_register();

    ret = f_mount(&fs, "/sd2", 1);
    // fs.fs_type = FS_FAT16;
    // ret = FR_NO_FILESYSTEM;

    if (ret == FR_NO_FILESYSTEM) {
        LOG_W("No filesystem yet, try to be formatted...\r\n");

        ret = f_mkfs("/sd2", &fs_para, workbuf, sizeof(workbuf));

        if (ret != FR_OK) {
            LOG_F("fail to make filesystem %d\r\n", ret);
            _CALL_ERROR();
        }

        if (ret == FR_OK) {
            LOG_I("done with formatting.\r\n");
            LOG_I("first start to unmount.\r\n");
            ret = f_mount(NULL, "/sd2", 1);
            LOG_I("then start to remount.\r\n");
            ret = f_mount(&fs, "/sd2", 1);
        }
    } else if (ret != FR_OK) {
        LOG_F("fail to mount filesystem,error= %d\r\n", ret);
        LOG_F("SD card might fail to initialise.\r\n");
        _CALL_ERROR();
    } else {
        LOG_D("Succeed to mount filesystem\r\n");
    }

    if (ret == FR_OK) {
        LOG_I("FileSystem cluster size:%d-sectors (%d-Byte)\r\n", fs.csize, fs.csize * 512);
    } else {
        LOG_F("fail to mount filesystem,error= %d\r\n", ret);
    }

    // vTaskDelay(1000);
    // fatfs_write_read_test();

    printf("Start to create file.\r\n");
    if (xSemaphoreTake(xMutex_RTC, portMAX_DELAY) == pdTRUE) {
        bflb_rtc_get_local_time(&time, +8); // get utc +8 time.
        xSemaphoreGive(xMutex_RTC);
    }
    printf("Get time\r\n");

    // // // Create file name with time.
    // // memset(FileNameEda, 0, 128 * sizeof(char));
    // // strcat(FileNameEda, "/sd2/eda_");
    // // sprintf(FileNameEda + strlen(FileNameEda), "%4d%02d%02d_%02d%02d%02d", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
    // // strcat(FileNameEda, ".csv");
    // // ret = f_open(&f_eda, FileNameEda, FA_OPEN_APPEND | FA_WRITE);
    // // sprintf(RW_Buffer, "Eda Num, Real(Ohm), Image(Ohm), Conductance(uS), Phase(°)\r\n");
    // // f_write(&f_eda, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);
    // // f_close(&f_eda);

    // // memset(FileNameTemp, 0, 128 * sizeof(char));
    // // strcat(FileNameTemp, "/sd2/temp_");
    // // sprintf(FileNameTemp + strlen(FileNameTemp), "%4d%02d%02d_%02d%02d%02d", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
    // // strcat(FileNameTemp, ".csv");
    // // ret = f_open(&f_temp, FileNameTemp, FA_OPEN_APPEND | FA_WRITE);
    // // sprintf(RW_Buffer, "Temp Num, Object(℃), Ambient(℃)\r\n");
    // // f_write(&f_temp, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);
    // // f_close(&f_temp);

    // // memset(FileNameBvp, 0, 128 * sizeof(char));
    // // strcat(FileNameBvp, "/sd2/bvp_");
    // // sprintf(FileNameBvp + strlen(FileNameBvp), "%4d%02d%02d_%02d%02d%02d", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
    // // strcat(FileNameBvp, ".csv");
    // // ret = f_open(&f_bvp, FileNameBvp, FA_OPEN_APPEND | FA_WRITE);
    // // sprintf(RW_Buffer, "Bvp Num, Bvp\r\n");
    // // f_write(&f_bvp, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);
    // // f_close(&f_bvp);

    // // memset(FileNameAcc, 0, 128 * sizeof(char));
    // // strcat(FileNameAcc, "/sd2/acc_");
    // // sprintf(FileNameAcc + strlen(FileNameAcc), "%4d%02d%02d_%02d%02d%02d", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
    // // strcat(FileNameAcc, ".csv");
    // // ret = f_open(&f_acc, FileNameAcc, FA_OPEN_APPEND | FA_WRITE);
    // // sprintf(RW_Buffer, "Acc Num, Acceleration1(mg), Acceleration2(mg), Acceleration3(mg), Angular Rate1(mdps), Angular Rate2(mdps), Angular Rate3(mdps)\r\n");
    // // f_write(&f_acc, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);
    // // f_close(&f_acc);

    // printf("FileNameEda %s\r\n", FileNameEda);

    while (1) {
        if (xSemaphoreTake(xSem_EDAEN, 0) == pdTRUE) {
            if (xSemaphoreTake(xMutex_RTC, portMAX_DELAY) == pdTRUE) {
                bflb_rtc_get_local_time(&time, +8); // get utc +8 time.
                xSemaphoreGive(xMutex_RTC);
            }

            memset(FileNameEda, 0, 128 * sizeof(char));
            strcat(FileNameEda, "/sd2/eda_");
            sprintf(FileNameEda + strlen(FileNameEda), "%4d%02d%02d_%02d%02d%02d", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
            strcat(FileNameEda, ".csv");
            ret = f_open(&f_eda, FileNameEda, FA_OPEN_APPEND | FA_WRITE);
            sprintf(RW_Buffer, "EdaNum, Real(Ohm), Image(Ohm), Conductance(uS), Phase(°)\r\n");
            f_write(&f_eda, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);
            f_close(&f_eda);
        }
        if (xSemaphoreTake(xSem_TEMPEN, 0) == pdTRUE) {
            if (xSemaphoreTake(xMutex_RTC, portMAX_DELAY) == pdTRUE) {
                bflb_rtc_get_local_time(&time, +8); // get utc +8 time.
                xSemaphoreGive(xMutex_RTC);
            }

            memset(FileNameTemp, 0, 128 * sizeof(char));
            strcat(FileNameTemp, "/sd2/temp_");
            sprintf(FileNameTemp + strlen(FileNameTemp), "%4d%02d%02d_%02d%02d%02d", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
            strcat(FileNameTemp, ".csv");
            ret = f_open(&f_temp, FileNameTemp, FA_OPEN_APPEND | FA_WRITE);
            sprintf(RW_Buffer, "TempNum, Object(℃), Ambient(℃)\r\n");
            f_write(&f_temp, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);
            f_close(&f_temp);
        }
        if (xSemaphoreTake(xSem_BVPEN, 0) == pdTRUE) {
            if (xSemaphoreTake(xMutex_RTC, portMAX_DELAY) == pdTRUE) {
                bflb_rtc_get_local_time(&time, +8); // get utc +8 time.
                xSemaphoreGive(xMutex_RTC);
            }

            memset(FileNameBvp, 0, 128 * sizeof(char));
            strcat(FileNameBvp, "/sd2/bvp_");
            sprintf(FileNameBvp + strlen(FileNameBvp), "%4d%02d%02d_%02d%02d%02d", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
            strcat(FileNameBvp, ".csv");
            ret = f_open(&f_bvp, FileNameBvp, FA_OPEN_APPEND | FA_WRITE);
            sprintf(RW_Buffer, "Num, Bvp(%.6fHz), Hr\r\n", afe4404_adcrdy_freq);
            f_write(&f_bvp, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);
            f_close(&f_bvp);
        }
        if (xSemaphoreTake(xSem_ACCEN, 0) == pdTRUE) {
            if (xSemaphoreTake(xMutex_RTC, portMAX_DELAY) == pdTRUE) {
                bflb_rtc_get_local_time(&time, +8); // get utc +8 time.
                xSemaphoreGive(xMutex_RTC);
            }

            memset(FileNameAcc, 0, 128 * sizeof(char));
            strcat(FileNameAcc, "/sd2/acc_");
            sprintf(FileNameAcc + strlen(FileNameAcc), "%4d%02d%02d_%02d%02d%02d", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
            strcat(FileNameAcc, ".csv");
            ret = f_open(&f_acc, FileNameAcc, FA_OPEN_APPEND | FA_WRITE);
            sprintf(RW_Buffer, "AccNum, Acceleration1(mg), Acceleration2(mg), Acceleration3(mg), Angular Rate1(mdps), Angular Rate2(mdps), Angular Rate3(mdps)\r\n");
            f_write(&f_acc, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);
            f_close(&f_acc);
        }
        if (xSemaphoreTake(xSem_ALGOEN, 0) == pdTRUE) {
            if (xSemaphoreTake(xMutex_RTC, portMAX_DELAY) == pdTRUE) {
                bflb_rtc_get_local_time(&time, +8); // get utc +8 time.
                xSemaphoreGive(xMutex_RTC);
            }

            memset(FileNameAlgo, 0, 128 * sizeof(char));
            strcat(FileNameAlgo, "/sd2/algo_");
            sprintf(FileNameAlgo + strlen(FileNameAlgo), "%4d%02d%02d_%02d%02d%02d", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
            strcat(FileNameAlgo, ".csv");
            ret = f_open(&f_algo, FileNameAlgo, FA_OPEN_APPEND | FA_WRITE);
            sprintf(RW_Buffer, "AlgoNum, output\r\n");
            f_write(&f_algo, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);
            f_close(&f_algo);
        }
        if ((uxQueueMessagesWaiting(Queue_EdaConFile) >= 4) && (uxQueueMessagesWaiting(Queue_EdaPhaFile) >= 4) && (uxQueueMessagesWaiting(Queue_EdaRealFile) >= 4) && (uxQueueMessagesWaiting(Queue_EdaImageFile) >= 4)) {
            ret = f_open(&f_eda, FileNameEda, FA_OPEN_APPEND | FA_WRITE);
            if (ret) {
                printf("f_open failed ret: %d\r\n", ret);
                continue;
            }

            xQueueReceive(Queue_EdaConFile, edaBuffer, portMAX_DELAY);
            xQueueReceive(Queue_EdaConFile, edaBuffer + 1, portMAX_DELAY);
            xQueueReceive(Queue_EdaConFile, edaBuffer + 2, portMAX_DELAY);
            xQueueReceive(Queue_EdaConFile, edaBuffer + 3, portMAX_DELAY);

            xQueueReceive(Queue_EdaPhaFile, edaPhaBuffer, portMAX_DELAY);
            xQueueReceive(Queue_EdaPhaFile, edaPhaBuffer + 1, portMAX_DELAY);
            xQueueReceive(Queue_EdaPhaFile, edaPhaBuffer + 2, portMAX_DELAY);
            xQueueReceive(Queue_EdaPhaFile, edaPhaBuffer + 3, portMAX_DELAY);

            xQueueReceive(Queue_EdaRealFile, edaRealBuffer, portMAX_DELAY);
            xQueueReceive(Queue_EdaRealFile, edaRealBuffer + 1, portMAX_DELAY);
            xQueueReceive(Queue_EdaRealFile, edaRealBuffer + 2, portMAX_DELAY);
            xQueueReceive(Queue_EdaRealFile, edaRealBuffer + 3, portMAX_DELAY);

            xQueueReceive(Queue_EdaImageFile, edaImageBuffer, portMAX_DELAY);
            xQueueReceive(Queue_EdaImageFile, edaImageBuffer + 1, portMAX_DELAY);
            xQueueReceive(Queue_EdaImageFile, edaImageBuffer + 2, portMAX_DELAY);
            xQueueReceive(Queue_EdaImageFile, edaImageBuffer + 3, portMAX_DELAY);

            sprintf(RW_Buffer, "%d, %.4f, %.4f, %.8f, %.4f\r\n%d, %.4f, %.4f, %.8f, %.4f\r\n%d, %.4f, %.4f, %.8f, %.4f\r\n%d, %.4f, %.4f, %.8f, %.4f\r\n",
                    edaFileCnt++, edaRealBuffer[0], edaImageBuffer[0], edaBuffer[0], edaPhaBuffer[0],
                    edaFileCnt++, edaRealBuffer[1], edaImageBuffer[1], edaBuffer[1], edaPhaBuffer[1],
                    edaFileCnt++, edaRealBuffer[2], edaImageBuffer[2], edaBuffer[2], edaPhaBuffer[2],
                    edaFileCnt++, edaRealBuffer[3], edaImageBuffer[3], edaBuffer[3], edaPhaBuffer[3]);

            ret = f_write(&f_eda, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);
            // printf("f_write ret %d, fnum %d\r\n", ret, fnum);
            f_close(&f_eda);
        }

        if ((uxQueueMessagesWaiting(Queue_TempFile_To) >= 1) && (uxQueueMessagesWaiting(Queue_TempFile_Ta) >= 1)) {
            ret = f_open(&f_temp, FileNameTemp, FA_OPEN_APPEND | FA_WRITE);
            if (ret) {
                printf("f_open failed ret: %d\r\n", ret);
                continue;
            }

            xQueueReceive(Queue_TempFile_To, &tempToBuffer, portMAX_DELAY);
            xQueueReceive(Queue_TempFile_Ta, &tempTaBuffer, portMAX_DELAY);

            sprintf(RW_Buffer, "%d, %.3f, %.3f\r\n", tempFileCnt++, tempToBuffer, tempTaBuffer);

            ret = f_write(&f_temp, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);
            f_close(&f_temp);
        }

        // printf("uxQueueMessagesWaiting(Queue_BvpFile) %d\r\n", uxQueueMessagesWaiting(Queue_BvpFile));
        if ((uxQueueMessagesWaiting(Queue_BvpFile) >= 64)) {
            ret = f_open(&f_bvp, FileNameBvp, FA_OPEN_APPEND | FA_WRITE);
            if (ret) {
                printf("f_open failed ret: %d\r\n", ret);
                continue;
            }
            uint16_t bufferPtr = 0;

            xQueueReceive(Queue_HRFile, &hrBuffer, 0);

            memset(RW_Buffer, 0x00, sizeof(RW_Buffer));
            for (int16_t i = 0; i < 64; i++) {
                xQueueReceive(Queue_BvpFile, &bvpBuffer, portMAX_DELAY);
                xQueueReceive(Queue_HRFile, &hrBuffer, 0);
                sprintf(tempBuffer, "%d, %d, %d\r\n", bvpFileCnt++, bvpBuffer, hrBuffer);
                strcat(RW_Buffer, tempBuffer);
            }

            f_write(&f_bvp, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);
            // printf("length Queue_BvpFile %d\r\n", uxQueueMessagesWaiting(Queue_BvpFile));

            // ret = f_write(&f_temp, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);
            f_close(&f_bvp);
        }
        // printf("uxQueueMessagesWaiting(Queue_AccFile_GY_Z) %d\r\n", uxQueueMessagesWaiting(Queue_AccFile_GY_Z));
        // printf("uxQueueMessagesWaiting(Queue_AccFile_XL_Z) %d\r\n", uxQueueMessagesWaiting(Queue_AccFile_XL_Z));

        if ((uxQueueMessagesWaiting(Queue_AccFile_GY_Z) >= 26) && (uxQueueMessagesWaiting(Queue_AccFile_XL_Z) >= 26)) {
            ret = f_open(&f_acc, FileNameAcc, FA_OPEN_APPEND | FA_WRITE);
            if (ret) {
                printf("f_open failed ret: %d\r\n", ret);
                continue;
            }

            memset(RW_Buffer, 0x00, sizeof(RW_Buffer));
            for (int16_t i = 0; i < 26; i++) {
                xQueueReceive(Queue_AccFile_XL_X, accXLxBuffer + i, portMAX_DELAY);
                xQueueReceive(Queue_AccFile_XL_Y, accXLyBuffer + i, portMAX_DELAY);
                xQueueReceive(Queue_AccFile_XL_Z, accXLzBuffer + i, portMAX_DELAY);
                xQueueReceive(Queue_AccFile_GY_X, accGYxBuffer + i, portMAX_DELAY);
                xQueueReceive(Queue_AccFile_GY_Y, accGYyBuffer + i, portMAX_DELAY);
                xQueueReceive(Queue_AccFile_GY_Z, accGYzBuffer + i, portMAX_DELAY);
                sprintf(tempBuffer, "%d, %.4f, %.4f, %.4f, %.4f, %.4F, %.4f\r\n", accFileCnt++, accXLxBuffer[i], accXLyBuffer[i], accXLzBuffer[i], accGYxBuffer[i], accGYyBuffer[i], accGYzBuffer[i]);

                strcat(RW_Buffer, tempBuffer);
            }

            f_write(&f_acc, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);
            // printf("AccWrite\r\n");
            // ret = f_write(&f_temp, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);
            f_close(&f_acc);
        }

        if ((uxQueueMessagesWaiting(Queue_AlgoFile) >= 1)) {
            ret = f_open(&f_algo, FileNameAlgo, FA_OPEN_APPEND | FA_WRITE);
            if (ret) {
                printf("f_open failed ret: %d\r\n", ret);
                continue;
            }

            memset(RW_Buffer, 0x00, sizeof(RW_Buffer));
            xQueueReceive(Queue_AlgoFile, &AlgoBuffer, portMAX_DELAY);
            sprintf(RW_Buffer, "%d, %d\r\n", AlgoFileCnt++, AlgoBuffer);

            f_write(&f_algo, RW_Buffer, sizeof(RW_Buffer[0]) * strlen(RW_Buffer), &fnum);

            f_close(&f_algo);
        }

        // printf("Write Cycle End\r\n");
        vTaskDelay(1);
    }

    // fatfs_write_read_test();
    // FIL fnew;
    // UINT fnum;
    // uint32_t num = 32;
    // ret = f_open(&fnew, "/sd2/single.txt", FA_OPEN_EXISTING | FA_READ);
    // ret = f_read(&fnew, readbuf, num, &fnum);
    // char charbuf[32] = { 0 };
    // printf(readbuf);

    // ret = f_mount(NULL, "/sd2", 1);

    // msc_ram_init();
    // while (1) {
    //     if (usbd_msc_set_popup()) {
    //         vTaskDelay(100);
    //         usbd_deinitialize();
    //     }
    //     vTaskDelay(100);
    // }

    while (1) {
        vTaskDelay(1000);
    }
    vTaskDelete(NULL);
}

DWORD get_fattime()
{
    struct bflb_tm time;
    uint32_t ret = 0;
    if (xSemaphoreTake(xMutex_RTC, portMAX_DELAY) == pdTRUE) {
        bflb_rtc_get_local_time(&time, +8); // get utc +8 time.
        xSemaphoreGive(xMutex_RTC);
    }

    ret |= ((time.tm_year - 80) & 0x7F) << 25;
    ret |= ((time.tm_mon + 1) & 0x0F) << 21;
    ret |= ((time.tm_mday) & 0x1F) << 16;
    ret |= ((time.tm_hour) & 0x1F) << 11;
    ret |= ((time.tm_min) & 0x3F) << 5;
    ret |= ((time.tm_sec) & 0x1F);

    return (DWORD)ret;
}

void fatfs2msc()
{
    FRESULT ret;
    // Wait for write finish.
    while (uxQueueMessagesWaiting(Queue_EdaConFile) >= 4) {
        printf("Queue_EdaConFile %d\r\n", uxQueueMessagesWaiting(Queue_EdaConFile));
        vTaskDelay(1);
    }
    while (uxQueueMessagesWaiting(Queue_EdaPhaFile) >= 4) {
        printf("Queue_EdaPhaFile %d\r\n", uxQueueMessagesWaiting(Queue_EdaPhaFile));
        vTaskDelay(1);
    }
    while (uxQueueMessagesWaiting(Queue_TempFile_To) >= 1) {
        printf("Queue_TempFile_To %d\r\n", uxQueueMessagesWaiting(Queue_TempFile_To));
        vTaskDelay(1);
    }
    while (uxQueueMessagesWaiting(Queue_TempFile_Ta) >= 1) {
        printf("Queue_TempFile_Ta %d\r\n", uxQueueMessagesWaiting(Queue_TempFile_Ta));
        vTaskDelay(1);
    }
    while (uxQueueMessagesWaiting(Queue_BvpFile) >= 64) {
        printf("Queue_BvpFile %d\r\n", uxQueueMessagesWaiting(Queue_BvpFile));
        vTaskDelay(1);
    }
    while (uxQueueMessagesWaiting(Queue_AccFile_XL_Z) >= 26) {
        printf("Queue_AccFile_XL_Z %d\r\n", uxQueueMessagesWaiting(Queue_AccFile_XL_Z));
        vTaskDelay(1);
    }
    while (uxQueueMessagesWaiting(Queue_AccFile_GY_Z) >= 26) {
        printf("Queue_AccFile_GY_Z %d\r\n", uxQueueMessagesWaiting(Queue_AccFile_GY_Z));
        vTaskDelay(1);
    }

    xQueueReset(Queue_EdaConFile);
    xQueueReset(Queue_EdaPhaFile);
    xQueueReset(Queue_EdaRealFile);
    xQueueReset(Queue_EdaImageFile);
    xQueueReset(Queue_TempFile_To);
    xQueueReset(Queue_TempFile_Ta);
    xQueueReset(Queue_BvpFile);
    xQueueReset(Queue_HRFile);
    xQueueReset(Queue_AccFile_XL_Z);
    xQueueReset(Queue_AccFile_GY_Z);
    xQueueReset(Queue_AccFile_XL_X);
    xQueueReset(Queue_AccFile_XL_Y);
    xQueueReset(Queue_AccFile_GY_X);
    xQueueReset(Queue_AccFile_GY_Y);
    xQueueReset(Queue_AlgoFile);

    while (f_eda.obj.fs != 0) {
        vTaskDelay(1);
    }
    while (f_temp.obj.fs != 0) {
        vTaskDelay(1);
    }
    while (f_bvp.obj.fs != 0) {
        vTaskDelay(1);
    }
    while (f_acc.obj.fs != 0) {
        vTaskDelay(1);
    }
    while (f_algo.obj.fs != 0) {
        vTaskDelay(1);
    }

    while (uxSemaphoreGetCount(xMutex_SPI) == 0) {
        vTaskDelay(1);
        printf("while xMutex_SPI\r\n");
    }
    if (FILE_handle != NULL) {
        ret = f_mount(NULL, "/sd2", 1);
        vTaskDelete(FILE_handle);
        printf("File task delete\r\n");
        printf("f_mount ret %d\r\n", ret);
    }
    FILE_handle = NULL;

    msc_ram_init();
}

void msc2fatfs()
{
    while (uxSemaphoreGetCount(xMutex_SPI) == 0) {
        vTaskDelay(1);
        printf("while xMutex_SPI\r\n");
    }

    usbd_deinitialize();

    vTaskDelay(10);

    xQueueReset(Queue_EdaConFile);
    xQueueReset(Queue_EdaPhaFile);
    xQueueReset(Queue_EdaRealFile);
    xQueueReset(Queue_EdaImageFile);
    xQueueReset(Queue_TempFile_To);
    xQueueReset(Queue_TempFile_Ta);
    xQueueReset(Queue_BvpFile);
    xQueueReset(Queue_HRFile);
    xQueueReset(Queue_AccFile_XL_Z);
    xQueueReset(Queue_AccFile_GY_Z);
    xQueueReset(Queue_AccFile_XL_X);
    xQueueReset(Queue_AccFile_XL_Y);
    xQueueReset(Queue_AccFile_GY_X);
    xQueueReset(Queue_AccFile_GY_Y);
    xQueueReset(Queue_AlgoFile);

    if (FILE_handle == NULL) {
        xTaskCreate(FILE_task, (char *)"File_task", 4 * 1024, NULL, configMAX_PRIORITIES - 4, &FILE_handle);
        printf("File task create\r\n");
    }
}

void ble_on()
{
    int32_t err_code = -99;
    if (isBleConnected == false) {
        btble_controller_init(configMAX_PRIORITIES - 1);

        hci_driver_init();

        err_code = bt_enable(bt_enable_cb);
    }

    if (err_code != 0) {
        printf("bt_enable error: %d\r\n", err_code);
    } else {
        printf("bt_enable\r\n");
    }
}

void ble_off()
{
    int32_t err_code = -99;

    if (isBleConnected == true) {
        err_code = bt_conn_disconnect(my_conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
        if (err_code == 0) {
            printf("bt_conn_disconnect\r\n");
        } else {
            printf("bt_conn_disconnect Error. %d\r\n", err_code);
        }
        xSemaphoreGive(xSem_BLEDISCONN);
    } else {
        bt_le_adv_stop();

        err_code = bt_disable();

        if (err_code == 0) {
            printf("bt_disable\r\n");
        } else {
            printf("bt_disable Error. %d\r\n", err_code);
        }
    }

    // isPlotedaInit = false;
    // isPlotBvpInit = false;
    // isPlotTempInit = false;
    // isPlotAccInit = false;
}

void wifi_on()
{
    // int32_t err_code = -99;

    // wifi_start_firmware_task();
}

void wifi_off()
{
    // if (wifi_connected){
    //     wifi_sta_disconnect();
    // }

    // wifi_stop_firmware_task();
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
    i2c1 = bflb_device_get_by_name("i2c1");
    pwm = bflb_device_get_by_name("pwm_v2_0");
    dma0_ch0 = bflb_device_get_by_name("dma0_ch0");
    dma0_ch1 = bflb_device_get_by_name("dma0_ch1");

    configASSERT((configMAX_PRIORITIES > 5));

    /* Shell init */ // No Shell.
    // shell_init_with_task(uart0);
    // printf("shell_init_with_task\r\n");

    bflb_mtd_init();
    /* ble stack need easyflash kv */
    easyflash_init();

    if (0 != rfparam_init(0, NULL, 0)) {
        LOG_I("PHY RF init failed!\r\n");
        return 0;
    }

    LOG_I("PHY RF init success!\r\n");

    // /* set ble controller EM Size */
    // btblecontroller_em_config();
    // printf("btblecontroller_em_config\r\n");

    // /* Init rf */
    // if (0 != rfparam_init(0, NULL, 0)) {
    //     printf("PHY RF init failed!\r\n");
    //     return 0;
    // }

// Initialize BLE controller
#if defined(BL702) || defined(BL602)
    ble_controller_init(configMAX_PRIORITIES - 1);
#else
    btble_controller_init(configMAX_PRIORITIES - 5);
#endif
    printf("btble_controller_init\r\n");
    // Initialize BLE Host stack
    hci_driver_init();
    printf("hci_driver_init\r\n");
    if (1) {
        bt_enable(bt_enable_cb);
        printf("bt_enable\r\n");
    }
    if (0) {
        tcpip_init(NULL, NULL);
        printf("tcpip_init\r\n");
        wifi_start_firmware_task();
        printf("wifi_start\r\n");
    }

    /* Peripheral Init (spi0, i2c0)*/
    peripheral_init();

    // afe4404 reset
    bflb_gpio_reset(gpio, AFE_RESETZ);
    bflb_mtimer_delay_us(60); // ~30us delay
    bflb_gpio_set(gpio, AFE_RESETZ);
    bflb_mtimer_delay_ms(10); // ~10ms delay with 16MHz clock

    // ad5941 reset
    AD5940_RstClr();
    bflb_mtimer_delay_ms(1); /* Delay some time */
    AD5940_RstSet();
    bflb_mtimer_delay_us(200); /* AD5940 need some time to exit reset status. 200us looks good. */

    printf("peripheral_init\n");

    /* LSM6DSO INIT */
    /* Uncomment to configure INT 1 */
    //lsm6dso_pin_int1_route_t int1_route;
    /* Uncomment to configure INT 2 */
    //lsm6dso_pin_int2_route_t int2_route;
    /* Initialize driver interface */
    dev_ctx.write_reg = lsm6dso_platform_write;
    dev_ctx.read_reg = lsm6dso_platform_read;

    xMutex_IIC0 = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex_IIC0);

    xMutex_IIC1 = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex_IIC1);

    xMutex_SPI = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex_SPI);

    xMutex_printf = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex_printf);

    xMutex_lvgl = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex_lvgl);

    xMutex_interArrayWindow = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex_interArrayWindow);

    xMutex_DSP = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex_DSP);

    xMutex_BLE = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex_BLE);

    xMutex_RTC = xSemaphoreCreateMutex();

    xSem_HR = xSemaphoreCreateBinary();
    // xSemaphoreTake(xSem_HR, (TickType_t)1);

    xSem_EDA = xSemaphoreCreateBinary();
    // xSemaphoreTake(xSem_EDA, (TickType_t)1);

    xSem_EDAEN = xSemaphoreCreateBinary();
    xSem_ACCEN = xSemaphoreCreateBinary();
    xSem_BVPEN = xSemaphoreCreateBinary();
    xSem_TEMPEN = xSemaphoreCreateBinary();
    xSem_ALGOEN = xSemaphoreCreateBinary();
    xSem_BLEDISCONN = xSemaphoreCreateBinary();
    xSemaphoreTake(xSem_BLEDISCONN, 0);
    xSem_BTNINT = xSemaphoreCreateBinary();
    xSemaphoreTake(xSem_BTNINT, 0);
    xSem_spirx = xSemaphoreCreateBinary();
    xSemaphoreTake(xSem_spirx, 0);
    xSem_spitx = xSemaphoreCreateBinary();
    xSemaphoreTake(xSem_spitx, 0);
    xSem_spitc = xSemaphoreCreateBinary();
    xSemaphoreTake(xSem_spitc, 0);

    // xSem_usb = xSemaphoreCreateBinary();
    Queue_HRBLE = xQueueCreate(200, sizeof(int32_t));
    Queue_AccBLE_GY = xQueueCreate(52, sizeof(float));
    Queue_AccBLE_XL = xQueueCreate(52, sizeof(float));
    Queue_TempBLE = xQueueCreate(8, sizeof(float));
    Queue_EdaBLE = xQueueCreate(16, sizeof(float));
    Queue_TempPlot_To = xQueueCreate(8, sizeof(float));
    Queue_EdaConFile = xQueueCreate(64, sizeof(float));
    Queue_EdaPhaFile = xQueueCreate(64, sizeof(float));
    Queue_EdaRealFile = xQueueCreate(64, sizeof(float));
    Queue_EdaImageFile = xQueueCreate(64, sizeof(float));
    Queue_TempFile_Ta = xQueueCreate(8, sizeof(float));
    Queue_TempFile_To = xQueueCreate(8, sizeof(float));
    Queue_AccFile_GY_X = xQueueCreate(256, sizeof(float));
    Queue_AccFile_GY_Y = xQueueCreate(256, sizeof(float));
    Queue_AccFile_GY_Z = xQueueCreate(256, sizeof(float));
    Queue_AccFile_XL_X = xQueueCreate(256, sizeof(float));
    Queue_AccFile_XL_Y = xQueueCreate(256, sizeof(float));
    Queue_AccFile_XL_Z = xQueueCreate(256, sizeof(float));
    Queue_AccPlot_XL_X = xQueueCreate(256, sizeof(float));
    Queue_AccPlot_XL_Y = xQueueCreate(256, sizeof(float));
    Queue_AccPlot_XL_Z = xQueueCreate(256, sizeof(float));
    Queue_BvpFile = xQueueCreate(1000, sizeof(int32_t));
    Queue_BvpAlgo = xQueueCreate(1600, sizeof(int32_t)); // 320.
    Queue_EdaAlgo = xQueueCreate(100, sizeof(float));    // 20.
    Queue_HRFile = xQueueCreate(4, sizeof(uint32_t));
    Queue_PPGChartUpdate = xQueueCreate(100, sizeof(int32_t));
    Queue_HRLabelUpdate = xQueueCreate(10, sizeof(uint32_t));
    Queue_AlgoFile = xQueueCreate(5, sizeof(uint8_t));
    Queue_AlgoPlot = xQueueCreate(5, sizeof(uint8_t));

    xSem_MOTOR = xSemaphoreCreateBinary();

    if (xMutex_IIC0 == NULL | xMutex_IIC1 == NULL | xMutex_SPI == NULL | xMutex_printf == NULL | xMutex_lvgl == NULL | xMutex_interArrayWindow == NULL | xMutex_DSP == NULL | xMutex_BLE == NULL) {
        printf("Create sem fail\r\n");
        while (1) {
        }
    }

    /* lvgl init */
    lv_log_register_print_cb(lv_log_print_g_cb);
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    ui_init();

    // msc_ram_init();

    // xTaskCreate(EDA_task, (char *)"EDA_task", 1024, NULL, configMAX_PRIORITIES - 4, &EDA_handle);
    // xTaskCreate(Algo_task, (char *)"Algo_task", 4 * 1024, NULL, configMAX_PRIORITIES - 5, &Algo_handle);
    // xTaskCreate(ACCE_task, (char *)"ACCE_task", 2 * 1024, NULL, configMAX_PRIORITIES - 4, &ACCE_handle);
    // xTaskCreate(TEMP_task, (char *)"TEMP_task", 1024, NULL, configMAX_PRIORITIES - 4, &TEMP_handle);
    // xTaskCreate(HR_task, (char *)"HR_task", 1024, NULL, configMAX_PRIORITIES - 4, &HR_handle);
    xTaskCreate(LVGL_task, (char *)"LVGL_task", 8 * 1024, NULL, configMAX_PRIORITIES - 4, &LVGL_handle);
    xTaskCreate(CLOCK_task, (char *)"Clock_task", 1024, NULL, configMAX_PRIORITIES - 4, &CLOCK_handle);
    // xTaskCreate(WIFI_task, (char *)"Wifi_task", 2 * 1024, NULL, configMAX_PRIORITIES - 4, &WIFI_handle);
    xTaskCreate(BLE_task, (char *)"Ble_task", 1024, NULL, configMAX_PRIORITIES - 4, &BLE_handle);
    xTaskCreate(BLE_trans_task, (char *)"Ble_trans_task", 1024, NULL, configMAX_PRIORITIES - 5, &BLE_trans_handle);
    xTaskCreate(MOTOR_task, (char *)"Motor_task", 1024, NULL, configMAX_PRIORITIES - 5, &MOTOR_handle);
    xTaskCreate(BAT_task, (char *)"Bat_task", 1024, NULL, configMAX_PRIORITIES - 4, &BAT_handle);
    xTaskCreate(FILE_task, (char *)"File_task", 4 * 1024, NULL, configMAX_PRIORITIES - 4, &FILE_handle);

    printf("kfree_size %d, pfree_size %d\r\n", kfree_size(), pfree_size());

    vTaskStartScheduler();
    while (1) {
    }
}
