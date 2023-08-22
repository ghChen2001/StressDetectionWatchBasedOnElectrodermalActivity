#include "AD5940Port.h"
#include "ad5940.h"
#include "stdio.h"
#include "bflb_spi.h"
#include "board.h"
#include "bflb_gpio.h"
#include "bflb_mtimer.h"
#include "bflb_dma.h"
#include "bflb_core.h"

extern struct bflb_device_s *spi0;                  /**< SPI instance. */
extern struct bflb_device_s *gpio;
extern bool spi0_tc;  /**< Flag used to indicate that SPI instance completed the transfer. */
// extern struct bflb_device_s *dma0_ch0;
// extern struct bflb_device_s *dma0_ch1;

#define SPI_MASTER_CASE 0
#define SPI_SLAVE_CASE  1
#define SPI_CASE_SELECT SPI_MASTER_CASE


extern uint8_t ucInterrupted;

// static const uint8_t m_length = BUFFERLEN;                           /**< Transfer length. */
// static ATTR_NOCACHE_NOINIT_RAM_SECTION uint8_t tx_buffer[BUFFERLEN]; /**< TX buffer. */
// static ATTR_NOCACHE_NOINIT_RAM_SECTION uint8_t rx_buffer[BUFFERLEN]; /**< RX buffer. */

// struct bflb_dma_channel_lli_pool_s tx_llipool[1];
// struct bflb_dma_channel_lli_transfer_s tx_transfers[1];
// struct bflb_dma_channel_lli_pool_s rx_llipool[1];
// struct bflb_dma_channel_lli_transfer_s rx_transfers[1];

/**
	@brief Using SPI to transmit N bytes and return the received bytes. This function targets to 
                     provide a more efficent way to transmit/receive data.
	@param pSendBuffer :{0 - 0xFFFFFFFF}
      - Pointer to the data to be sent.
	@param pRecvBuff :{0 - 0xFFFFFFFF}
      - Pointer to the buffer used to store received data.
	@param length :{0 - 0xFFFFFFFF}
      - Data length in SendBuffer.
	@return None.
**/

// void AD5940_ReadWriteNBytes(unsigned char *pSendBuffer,unsigned char *pRecvBuff,unsigned long length)
// {
//   spi_tc = false;
//   printf("Enter AD5940_ReadWriteNBytes");
//   tx_transfers[0].src_addr = (uint32_t)tx_buffer;
//   tx_transfers[0].dst_addr = (uint32_t)DMA_ADDR_SPI0_TDR;
//   tx_transfers[0].nbytes = length;

//   rx_transfers[0].src_addr = (uint32_t)DMA_ADDR_SPI0_RDR;
//   rx_transfers[0].dst_addr = (uint32_t)rx_buffer;
//   rx_transfers[0].nbytes = length;

//   bflb_dma_channel_lli_reload(dma0_ch0, tx_llipool, 1, tx_transfers, 1);
//   bflb_dma_channel_lli_reload(dma0_ch1, rx_llipool, 1, rx_transfers, 1);
//   printf("bflb_dma_channel_lli_reload");
//   bflb_dma_channel_start(dma0_ch0);
//   printf("bflb_dma_channel_start(dma0_ch0)");
//   bflb_dma_channel_start(dma0_ch1);
//   printf("bflb_dma_channel_start");
//   while (!spi_tc) {
//       //__WFE();
//   }
// }

void AD5940_CsClr(void)
{
  // gpio = bflb_device_get_by_name("gpio");
  bflb_gpio_reset(gpio, AD5940_CS_PIN);
}

void AD5940_CsSet(void)
{
  // gpio = bflb_device_get_by_name("gpio");
  bflb_gpio_set(gpio, AD5940_CS_PIN);
  //NRF_LOG_INFO("AD5940_CsSet1");
}

void AD5940_RstSet(void)
{
  // gpio = bflb_device_get_by_name("gpio");
  bflb_gpio_set(gpio, AD5940_RST_PIN);
}

void AD5940_RstClr(void)
{
  // gpio = bflb_device_get_by_name("gpio");
  bflb_gpio_reset(gpio, AD5940_RST_PIN);
}

void AD5940_Delay10us(uint32_t time)
{
  time=time*10;
  bflb_mtimer_delay_us(time);
}

uint32_t AD5940_GetMCUIntFlag(void)
{
	return ucInterrupted;
}

uint32_t AD5940_ClrMCUIntFlag(void)
{
	ucInterrupted = 0;
	return 1;
}

// void spi_isr(int irq, void *arg)
// {
//     uint32_t intstatus = bflb_spi_get_intstatus(spi0);
//     if (intstatus & SPI_INTSTS_TC) {
//     bflb_spi_int_clear(spi0, SPI_INTCLR_TC);
//     spi0_tc = true;
//     printf("tc done\r\n");
//     }
//     if (intstatus & SPI_INTSTS_TX_FIFO) {
//     //printf("tx fifo\r\n");
//     }
//     if (intstatus & SPI_INTSTS_RX_FIFO) {
//     //printf("rx fifo\r\n");
//     }
// }

// void dma0_ch0_isr(void *arg)
// {
//     printf("tc done\r\n");
// }

// void dma0_ch1_isr(void *arg)
// {
//     printf("rx done\r\n");
// }

// uint32_t AD5940_MCUResourceInit(void *pCfg)
// {
//     /* Step1, initialize SPI peripheral and its GPIOs for CS/RST */
//     board_spi0_gpio_init();
//     gpio = bflb_device_get_by_name("gpio");

//     //Interrupt Key
//     bflb_gpio_init(gpio, KEY0, GPIO_INPUT | GPIO_PULLUP | GPIO_SMT_DIS | GPIO_INT_TRIG_MODE_ASYNC_FALLING_EDGE);
//     bflb_gpio_int_mask(gpio, KEY0, false);
//     bflb_irq_attach(gpio->irq_num, KEY0_Interrupt, gpio);
//     bflb_irq_enable(gpio->irq_num);

//     //RST key
//     bflb_gpio_init(gpio, AD5940_RST_PIN, GPIO_OUTPUT);

//     // //Buffer init
//     // uint32_t i;

//     // for (i = 0; i < m_length; i++) {
//     //     tx_buffer[i] = 0;
//     //     rx_buffer[i] = 0;
//     // }
	
//     AD5940_CsSet();
//     AD5940_RstSet();
  
// /* Set the SPI parameters */
//     struct bflb_spi_config_s spi_cfg = {
//   #if (SPI_CASE_SELECT == SPI_MASTER_CASE)
//         .freq = 1 * 1000 * 1000,
//         .role = SPI_ROLE_MASTER,
//   #else
//         .freq = 32 * 1000 * 1000,
//         .role = SPI_ROLE_SLAVE,
//   #endif
//         .mode = SPI_MODE3,
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

//     spi0 = bflb_device_get_by_name("spi0");
//     bflb_spi_init(spi0, &spi_cfg);
//     bflb_spi_tcint_mask(spi0, false);
//     bflb_irq_attach(spi0->irq_num, spi_isr, NULL);
//     bflb_irq_enable(spi0->irq_num);

//     // bflb_spi_link_txdma(spi0, true);
//     // bflb_spi_link_rxdma(spi0, true);

//     // dma0_ch0 = bflb_device_get_by_name("dma0_ch0");
//     // dma0_ch1 = bflb_device_get_by_name("dma0_ch1");

//     // bflb_dma_channel_init(dma0_ch0, &tx_config);
//     // bflb_dma_channel_init(dma0_ch1, &rx_config);

//     // bflb_dma_channel_irq_attach(dma0_ch0, dma0_ch0_isr, NULL);
//     // bflb_dma_channel_irq_attach(dma0_ch1, dma0_ch1_isr, NULL);

//   return 0;
// }

// void SPI0_DMA0_StartTransfer(uint8_t *rx_buf, uint8_t *tx_buf, uint32_t len)
// {
//   tx_transfers[0].src_addr = (uint32_t)rx_buf;
//   tx_transfers[0].dst_addr = (uint32_t)DMA_ADDR_SPI0_TDR;
//   tx_transfers[0].nbytes = len;

//   rx_transfers[0].src_addr = (uint32_t)DMA_ADDR_SPI0_RDR;
//   rx_transfers[0].dst_addr = (uint32_t)tx_buf;
//   rx_transfers[0].nbytes = len;

//   bflb_dma_channel_lli_reload(dma0_ch0, tx_llipool, 1, tx_transfers, 1);
//   bflb_dma_channel_lli_reload(dma0_ch1, rx_llipool, 1, rx_transfers, 1);

//   bflb_dma_channel_start(dma0_ch0);
//   bflb_dma_channel_start(dma0_ch1);
// }

// /* MCU related external line interrupt service routine */
// void KEY0_Interrupt(int irq, void *arg)
// {
//   bool intstatus = bflb_gpio_get_intstatus(gpio, KEY0);
//   if (intstatus) {
//         ucInterrupted = 1;
//         bflb_gpio_int_clear(gpio, KEY0);
//   }
// }
