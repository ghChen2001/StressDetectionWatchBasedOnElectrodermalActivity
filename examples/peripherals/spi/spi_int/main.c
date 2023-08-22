#include "bflb_mtimer.h"
#include "bflb_spi.h"
#include "board.h"
#include "AD5940Port.h"
#include "AD5940.h"

#define SPI_MASTER_CASE 0
#define SPI_SLAVE_CASE  1

#define SPI_CASE_SELECT SPI_MASTER_CASE

// #define BUFF_LEN (8 * 1024)

// uint32_t tx_buff[BUFF_LEN / 4];
// uint32_t rx_buff[BUFF_LEN / 4];

// struct bflb_device_s *spi0;

static struct bflb_device_s *uart0;
struct bflb_device_s *gpio;
struct bflb_device_s *spi0;
// struct bflb_device_s *dma0_ch0;
// struct bflb_device_s *dma0_ch1;

uint8_t ucInterrupted;
float LFOSCFreq; /* Measured LFOSC frequency */
#define BUFFERLEN 32

static const uint8_t m_length = BUFFERLEN;                           /**< Transfer length. */
static ATTR_NOCACHE_NOINIT_RAM_SECTION uint8_t tx_buffer[BUFFERLEN]; /**< TX buffer. */
static ATTR_NOCACHE_NOINIT_RAM_SECTION uint8_t rx_buffer[BUFFERLEN]; /**< RX buffer. */

// struct bflb_dma_channel_lli_pool_s tx_llipool[1];
// struct bflb_dma_channel_lli_transfer_s tx_transfers[1];
// struct bflb_dma_channel_lli_pool_s rx_llipool[1];
// struct bflb_dma_channel_lli_transfer_s rx_transfers[1];

volatile bool spi0_tc = false; /**< Flag used to indicate that SPI instance completed the transfer. */

// /* poll test func */
// int bflb_spi_poll_test(uint32_t data_width)
// {
//     uint32_t data_mask;
//     uint32_t *p_tx = (uint32_t *)tx_buff;
//     uint32_t *p_rx = (uint32_t *)rx_buff;

//     switch (data_width) {
//         case SPI_DATA_WIDTH_8BIT:
//             data_mask = 0x000000FF;
//             break;
//         case SPI_DATA_WIDTH_16BIT:
//             data_mask = 0x0000FFFF;
//             break;
//         case SPI_DATA_WIDTH_24BIT:
//             data_mask = 0x00FFFFFF;
//             break;
//         case SPI_DATA_WIDTH_32BIT:
//             data_mask = 0xFFFFFFFF;
//             break;
//         default:
//             printf("data_width err\r\n");
//             return -1;
//             break;
//     }

//     /* data init */
//     for (uint16_t i = 0; i < BUFF_LEN / 4; i++) {
//         p_tx[i] = i;
//         p_rx[i] = 0;
//     }

//     /* set data width */
//     bflb_spi_feature_control(spi0, SPI_CMD_SET_DATA_WIDTH, data_width);

//     /* send data */
//     for (uint16_t i = 0; i < BUFF_LEN / 4; i++) {
//         p_rx[i] = bflb_spi_poll_send(spi0, p_tx[i]);
//     }

//     /* check data */
//     for (uint16_t i = 0; i < BUFF_LEN / 4; i++) {
//         if (p_rx[i] != (p_tx[i] & data_mask)) {
//             printf("data error, data[%d]:tx 0x%08lX, rx 0x%08lX\r\n", i, p_tx[i], p_rx[i]);
//             return -1;
//         }
//     }
//     printf("data check success\r\n");

//     return 0;
// }

// /* poll_exchange test func */
// int bflb_spi_poll_exchange_test(uint32_t data_width)
// {
//     void *p_tx = (uint32_t *)tx_buff;
//     void *p_rx = (uint32_t *)rx_buff;

//     /* data init */
//     switch (data_width) {
//         case SPI_DATA_WIDTH_8BIT:
//             for (uint16_t i = 0; i < BUFF_LEN; i++) {
//                 ((uint8_t *)p_tx)[i] = i;
//                 ((uint8_t *)p_rx)[i] = 0;
//             }
//             break;
//         case SPI_DATA_WIDTH_16BIT:
//             for (uint16_t i = 0; i < BUFF_LEN / 2; i++) {
//                 ((uint16_t *)p_tx)[i] = i << 0;
//                 ((uint16_t *)p_rx)[i] = 0;
//             }
//             break;
//         case SPI_DATA_WIDTH_24BIT:
//             for (uint16_t i = 0; i < BUFF_LEN / 4; i++) {
//                 ((uint32_t *)p_tx)[i] = ((i << 0) | i) & 0x00FFFFFF;
//                 ((uint32_t *)p_rx)[i] = 0;
//             }
//             break;
//         case SPI_DATA_WIDTH_32BIT:
//             for (uint16_t i = 0; i < BUFF_LEN / 4; i++) {
//                 ((uint32_t *)p_tx)[i] = (i << 0) | i;
//                 ((uint32_t *)p_rx)[i] = 0;
//             }
//             break;
//         default:
//             return -1;
//             break;
//     }

//     /* set data width */
//     bflb_spi_feature_control(spi0, SPI_CMD_SET_DATA_WIDTH, data_width);

//     /* send data */
//     printf("spi poll exchange width %ld, len %d\r\n", data_width, BUFF_LEN);
//     bflb_spi_poll_exchange(spi0, p_tx, p_rx, BUFF_LEN);

//     /* check data */
//     for (uint16_t i = 0; i < BUFF_LEN / 4; i++) {
//         if (((uint32_t *)p_rx)[i] != ((uint32_t *)p_tx)[i]) {
//             printf("data error, data[%d]:tx 0x%08lX, rx 0x%08lX\r\n", i, ((uint32_t *)p_tx)[i], ((uint32_t *)p_rx)[i]);
//             return -1;
//         }
//     }
//     printf("data check success\r\n");

//     return 0;
// }

<<<<<<< Updated upstream
    /* send data */
    for (uint16_t i = 0; i < BUFF_LEN / 4; i++) {
        p_rx[i] = bflb_spi_poll_send(spi0, p_tx[i]);
#if (SPI_CASE_SELECT == SPI_MASTER_CASE)
        bflb_mtimer_delay_us(10); /* delay for slave device prepare ok */
#endif
    }
=======
volatile uint32_t spi_tc_done_count = 0;
>>>>>>> Stashed changes

// void spi_isr(int irq, void *arg)
// {
//     uint32_t intstatus = bflb_spi_get_intstatus(spi0);
//     if (intstatus & SPI_INTSTS_TC) {
//         bflb_spi_int_clear(spi0, SPI_INTCLR_TC);
//         printf("tc done\r\n");
//         spi_tc_done_count++;
//     }
//     if (intstatus & SPI_INTSTS_TX_FIFO) {
//         //printf("tx fifo\r\n");
//     }
//     if (intstatus & SPI_INTSTS_RX_FIFO) {
//         //printf("rx fifo\r\n");
//     }
// }

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
    printf("Freq:%.3f", LFOSCFreq);

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

    bflb_spi_poll_exchange(spi0, pSendBuffer, pRecvBuff, length);
    // while (!spi0_tc) {
    // //     //__WFE();
    // }
}

void spi_isr(int irq, void *arg)
{
    uint32_t intstatus = bflb_spi_get_intstatus(spi0);
    // printf("tc done1\r\n");
    if (intstatus & SPI_INTSTS_TC) {
        bflb_spi_int_clear(spi0, SPI_INTCLR_TC);
        spi0_tc = true;
        printf("tc done\r\n");
    }
    if (intstatus & SPI_INTSTS_TX_FIFO) {
        // printf("tx fifo\r\n");
    }
    if (intstatus & SPI_INTSTS_RX_FIFO) {
        // printf("rx fifo\r\n");
    }
}

uint32_t AD5940_MCUResourceInit(void *pCfg)
{
    /* Step1, initialize SPI peripheral and its GPIOs for CS/RST */
    board_spi0_gpio_init();
    gpio = bflb_device_get_by_name("gpio");

    //Interrupt Key
    bflb_gpio_init(gpio, KEY0, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_DIS | GPIO_INT_TRIG_MODE_ASYNC_FALLING_EDGE);
    bflb_gpio_int_mask(gpio, KEY0, false);
    bflb_irq_attach(gpio->irq_num, KEY0_Interrupt, gpio);
    bflb_irq_enable(gpio->irq_num);

    //RST key
    bflb_gpio_init(gpio, AD5940_RST_PIN, GPIO_OUTPUT);

    // //Buffer init
    // uint32_t i;

    // for (i = 0; i < m_length; i++) {
    //     tx_buffer[i] = 0;
    //     rx_buffer[i] = 0;
    // }

    AD5940_CsSet();
    AD5940_RstSet();

    /* Set the SPI parameters */
    struct bflb_spi_config_s spi_cfg = {
#if (SPI_CASE_SELECT == SPI_MASTER_CASE)
        .freq = 1 * 125 * 1000,
        .role = SPI_ROLE_MASTER,
#else
        .freq = 32 * 1000 * 1000,
        .role = SPI_ROLE_SLAVE,
#endif
        .mode = SPI_MODE0,
        .data_width = SPI_DATA_WIDTH_8BIT,
        .bit_order = SPI_BIT_MSB,
        .byte_order = SPI_BYTE_MSB,
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

    spi0 = bflb_device_get_by_name("spi0");
    bflb_spi_init(spi0, &spi_cfg);
    // bflb_spi_txint_mask(spi0, false);
    // bflb_spi_rxint_mask(spi0, false);
    bflb_spi_tcint_mask(spi0, false);
    bflb_irq_attach(spi0->irq_num, spi_isr, NULL);
    bflb_irq_enable(spi0->irq_num);

    bflb_spi_feature_control(spi0, SPI_CMD_SET_CS_INTERVAL, 0);

    // bflb_spi_link_txdma(spi0, true);
    // bflb_spi_link_rxdma(spi0, true);

    // dma0_ch0 = bflb_device_get_by_name("dma0_ch0");
    // dma0_ch1 = bflb_device_get_by_name("dma0_ch1");

    // bflb_dma_channel_init(dma0_ch0, &tx_config);
    // bflb_dma_channel_init(dma0_ch1, &rx_config);

    // bflb_dma_channel_irq_attach(dma0_ch0, dma0_ch0_isr, NULL);
    // bflb_dma_channel_irq_attach(dma0_ch1, dma0_ch1_isr, NULL);

    return 0;
}

int main(void)
{
    board_init();
//     board_spi0_gpio_init();

//     struct bflb_spi_config_s spi_cfg = {
// #if (SPI_CASE_SELECT == SPI_MASTER_CASE)
//         .freq = 1 * 1000 * 1000,
//         .role = SPI_ROLE_MASTER,
// #else
//         .freq = 32 * 1000 * 1000,
//         .role = SPI_ROLE_SLAVE,
// #endif
//         .mode = SPI_MODE3,
//         .data_width = SPI_DATA_WIDTH_8BIT,
//         .bit_order = SPI_BIT_MSB,
//         .byte_order = SPI_BYTE_LSB,
//         .tx_fifo_threshold = 0,
//         .rx_fifo_threshold = 0,
//     };

//     spi0 = bflb_device_get_by_name("spi0");
//     bflb_spi_init(spi0, &spi_cfg);

//     // bflb_spi_txint_mask(spi0, false);
//     // bflb_spi_rxint_mask(spi0, false);
//     bflb_spi_tcint_mask(spi0, false);
//     bflb_irq_attach(spi0->irq_num, spi_isr, NULL);
//     bflb_irq_enable(spi0->irq_num);

//     bflb_spi_feature_control(spi0, SPI_CMD_SET_CS_INTERVAL, 0);

    AD5940_MCUResourceInit(0);
    printf("AD5940_MCUResourceInit(0)\r\n");
    bflb_mtimer_delay_ms(100);
    AD5940_HWReset();
    printf("AD5940_HWReset\r\n");
    AD5940_Initialize();
    printf("AD5940_Initialize\r\n");
    AD5940PlatformCfg();
    printf("AD5940PlatformCfg\r\n");

    while (1) {
    }
}
