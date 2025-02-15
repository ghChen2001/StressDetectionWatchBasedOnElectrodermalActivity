#include "NAND_SPI.h"
#include <FreeRTOS.h>
#include "semphr.h"
#include "board.h"
#include "bflb_spi.h"
#include "bflb_core.h"
#include "bflb_dma.h"

extern SemaphoreHandle_t xMutex_SPI;
// extern SemaphoreHandle_t xSem_spirx;
// extern SemaphoreHandle_t xSem_spitx;
// extern SemaphoreHandle_t xSem_spitc;
extern struct bflb_device_s *gpio;
extern struct bflb_device_s *spi0;
// extern struct bflb_device_s *dma0_ch0;
// extern struct bflb_device_s *dma0_ch1;
// extern struct bflb_dma_channel_lli_pool_s tx_llipool[1];
// extern struct bflb_dma_channel_lli_pool_s rx_llipool[1];
// static struct bflb_dma_channel_lli_transfer_s tx_transfers_nand[1];
// static struct bflb_dma_channel_lli_transfer_s rx_transfers_nand[1];
static ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) uint8_t sDummy = 0xFF;
static ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) uint8_t recv = 0xFF;
static ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) uint8_t CMD24_WRITE[6] = { NAND_CMD24, 0x00, 0x00, 0x00, 0x00, 0xFF };
static ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) uint8_t CMD25_WRITE[6] = { NAND_CMD25, 0x00, 0x00, 0x00, 0x00, 0xFF };
static ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) uint8_t CMD17_WRITE[6] = { NAND_CMD17, 0x00, 0x00, 0x00, 0x00, 0xFF };
static ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) uint8_t CMD18_WRITE[6] = { NAND_CMD18, 0x00, 0x00, 0x00, 0x00, 0xFF };
static ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) uint8_t CMD12_WRITE[6] = { NAND_CMD12, 0x00, 0x00, 0x00, 0x00, 0xFF };
static ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) uint8_t CMD55_WRITE[6] = { NAND_CMD55, 0x00, 0x00, 0x00, 0x00, 0xFF };
static ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) uint8_t CMD23_WRITE[6] = { NAND_CMD23, 0x00, 0x00, 0x00, 0x00, 0xFF };
static ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) uint8_t SingleStart = 0xFE;
static ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) uint8_t MultiStart = 0xFC;
static ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) uint8_t MultiEnd = 0xFD;
static ATTR_NOCACHE_RAM_SECTION __attribute((aligned(8))) uint8_t sendDummy[1024];

uint8_t spi_rw_dma(unsigned char *pSendBuffer, unsigned char *pRecvBuff, uint32_t length)
{
    // // bflb_spi_feature_control(spi0, SPI_CMD_SET_FREQ, 16 * 1000 * 1000);
    // tx_transfers_nand[0].src_addr = (uint32_t)pSendBuffer;
    // tx_transfers_nand[0].dst_addr = (uint32_t)DMA_ADDR_SPI0_TDR;
    // tx_transfers_nand[0].nbytes = length;
    // bflb_dma_channel_lli_reload(dma0_ch0, tx_llipool, 1, tx_transfers_nand, 1);

    // rx_transfers_nand[0].src_addr = (uint32_t)DMA_ADDR_SPI0_RDR;
    // rx_transfers_nand[0].dst_addr = (uint32_t)pRecvBuff;
    // rx_transfers_nand[0].nbytes = length;
    // bflb_dma_channel_lli_reload(dma0_ch1, rx_llipool, 1, rx_transfers_nand, 1);
    // bflb_dma_channel_start(dma0_ch1);

    // bflb_dma_channel_start(dma0_ch0);

    // // printf("spi_rw_dma length %d\r\n", length);
    // // vTaskDelay(10);

    // if (xSemaphoreTake(xSem_spitx, portMAX_DELAY) == pdFALSE) {
    //     return;
    // }
    // // printf("xSem_spitx\r\n");

    // if (xSemaphoreTake(xSem_spirx, portMAX_DELAY) == pdFALSE) {
    //     return;
    // }
    // // printf("xSem_spirx\r\n");

    // if (xSemaphoreTake(xSem_spitc, portMAX_DELAY) == pdFALSE) {
    //     return;
    // }
    // // printf("Exit spi_rw_dma\r\n");
    bflb_spi_poll_exchange(spi0, pSendBuffer, pRecvBuff, length);
}

uint8_t spi_w_dma(unsigned char *pSendBuffer, uint32_t length)
{
    // // bflb_spi_feature_control(spi0, SPI_CMD_SET_FREQ, 16 * 1000 * 1000);
    // tx_transfers_nand[0].src_addr = (uint32_t)pSendBuffer;
    // tx_transfers_nand[0].dst_addr = (uint32_t)DMA_ADDR_SPI0_TDR;
    // tx_transfers_nand[0].nbytes = length;
    // bflb_dma_channel_lli_reload(dma0_ch0, tx_llipool, 1, tx_transfers_nand, 1);

    // bflb_dma_channel_start(dma0_ch0);

    // // printf("spi_w_dma length %d\r\n", length);
    // // vTaskDelay(10);

    // if (xSemaphoreTake(xSem_spitx, portMAX_DELAY) == pdFALSE) {
    //     return;
    // }
    // // printf("xSem_spitx\r\n");

    // if (xSemaphoreTake(xSem_spitc, portMAX_DELAY) == pdFALSE) {
    //     return;
    // }
    // // printf("Exit spi_w_dma\r\n");
    bflb_spi_poll_exchange(spi0, pSendBuffer, NULL, length);
}

uint8_t NAND_DeviceInit(void)
{
    uint8_t sDummy_ = 0xFF;
    uint8_t recv_ = 0xFF;
    bool isSDINIT = false;

    // Wake Up NAND
    uint8_t sendDummy_[16];
    for (uint8_t i = 0; i < 16; i++) {
        sendDummy_[i] = 0xFF;
    }
    bflb_spi_poll_exchange(spi0, sendDummy_, NULL, 12); // At least 74 clks.
    printf("NAND WAKEUP\r\n");

    // WRITE CMD0
    bflb_gpio_reset(gpio, GPIO_PIN_22);
    uint8_t CMD0_WRITE[6] = { NAND_CMD0, 0x00, 0x00, 0x00, 0x00, 0x95 };
    recv_ = 0xFF;
    bflb_spi_poll_exchange(spi0, CMD0_WRITE, NULL, 6);
    while (recv_ != 0x01) {
        bflb_spi_poll_exchange(spi0, &sDummy_, &recv_, 1);
        printf("recv_ %02x\r\n", recv_);
    }
    printf("NAND is in SPI mode.\r\n");
    bflb_spi_poll_exchange(spi0, NULL, NULL, 1);
    bflb_gpio_set(gpio, GPIO_PIN_22);

    bflb_mtimer_delay_us(10);

    // WRITE CMD8
    // To check if it is a SD 2.0 card.
    bflb_gpio_reset(gpio, GPIO_PIN_22);
    uint8_t CMD8_WRITE[6] = { NAND_CMD8, 0x00, 0x00, 0x01, 0xAA, 0x87 };
    bflb_spi_poll_exchange(spi0, CMD8_WRITE, NULL, 6);
    recv_ = 0xFF;
    while ((recv_ & 0x80) != 0x00) {
        bflb_spi_poll_exchange(spi0, &sDummy_, &recv_, 1);
    }
    bflb_spi_poll_exchange(spi0, sendDummy_, NULL, 5);
    bflb_gpio_set(gpio, GPIO_PIN_22);
    if (recv_ == 0x01) {
        printf("SD 2.0\r\n");
    } else {
        printf("NOT SD 2.0 %02x\r\n", recv_);
    }

    uint8_t timeout = 16;
    while (!isSDINIT) {
        // WRITE CMD55
        bflb_gpio_reset(gpio, GPIO_PIN_22);
        uint8_t CMD55_WRITE_[6] = { NAND_CMD55, 0x00, 0x00, 0x00, 0x00, 0xFF };
        bflb_spi_poll_exchange(spi0, CMD55_WRITE_, NULL, 6);
        recv_ = 0xFF;
        while (recv_ != 0x01) {
            bflb_spi_poll_exchange(spi0, &sDummy_, &recv_, 1);
        }
        bflb_spi_poll_exchange(spi0, &sDummy_, NULL, 1);
        bflb_gpio_set(gpio, GPIO_PIN_22);
        // printf("CMD55\r\n");

        // WRITE ACMD41
        bflb_gpio_reset(gpio, GPIO_PIN_22);
        uint8_t CMD41_WRITE[6] = { NAND_CMD41, 0x40, 0x00, 0x00, 0x00, 0xFF };
        bflb_spi_poll_exchange(spi0, CMD41_WRITE, NULL, 6);
        recv_ = 0xFF;
        timeout = 16;
        while ((recv_ != 0x00) && (timeout != 0)) {
            bflb_spi_poll_exchange(spi0, &sDummy_, &recv_, 1);
            timeout--;
        }
        bflb_spi_poll_exchange(spi0, &sDummy_, NULL, 1);
        bflb_gpio_set(gpio, GPIO_PIN_22);

        if (recv_ == 0x00) {
            isSDINIT = true;
        }
    }
    nandCardInfo.inited = true;

    printf("SD INIT\r\n");

    // Read OCR
    bflb_gpio_reset(gpio, GPIO_PIN_22);
    uint8_t CMD58_WRITE[6] = { NAND_CMD58, 0x00, 0x00, 0x00, 0x00, 0xFF };
    bflb_spi_poll_exchange(spi0, CMD58_WRITE, NULL, 6);
    recv_ = 0xFF;
    while ((recv_ & 0x80) != 0x00) {
        bflb_spi_poll_exchange(spi0, &sDummy_, &recv_, 1);
    }
    bflb_spi_poll_exchange(spi0, sendDummy_, (uint8_t *)&(nandCardInfo.ocr), 4);
    bflb_gpio_set(gpio, GPIO_PIN_22);
    bflb_spi_poll_exchange(spi0, &sDummy_, NULL, 1);
    if (((uint8_t *)&(nandCardInfo.ocr))[0] & 0x40) {
        printf("SDHC\r\n");
    } else {
        printf("SDSC\r\n");
    }

    // // Read CID
    // bflb_gpio_reset(gpio, GPIO_PIN_22);
    // uint8_t CMD10_WRITE[6] = { NAND_CMD10, 0x00, 0x00, 0x00, 0x00, 0xFF };
    // bflb_spi_poll_exchange(spi0, CMD10_WRITE, NULL, 6);
    // recv = 0xFF;
    // while ((recv & 0x80) != 0x00) {
    //     bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
    // }
    // bflb_spi_poll_exchange(spi0, sendDummy, nandCardInfo.rawCid, 16);
    // bflb_gpio_set(gpio, GPIO_PIN_22);
    // bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
    // printf("manufacturerDate: y %d, m %d\r\n", ((nandCardInfo.rawCid[13] & 0x0F) << 4) | ((nandCardInfo.rawCid[14] & 0xF0) >> 4), nandCardInfo.rawCid[14] & 0x0F);
    // printf("%02x\r\n", nandCardInfo.rawCid[14]);

    // Set Block Size to 512 KB
    bflb_gpio_reset(gpio, GPIO_PIN_22);
    uint8_t CMD16_WRITE[6] = { NAND_CMD16, 0x00, 0x00, 0x02, 0x00, 0xFF };
    bflb_spi_poll_exchange(spi0, CMD16_WRITE, NULL, 6);
    recv_ = 0xFF;
    while (recv_ != 0x00) {
        bflb_spi_poll_exchange(spi0, &sDummy_, &recv_, 1);
        // printf("CMD16 recv 0x%02x\r\n", recv);
    }
    nandCardInfo.blockSize = 512;
    bflb_gpio_set(gpio, GPIO_PIN_22);
    bflb_spi_poll_exchange(spi0, &sDummy_, NULL, 1);

    return 0;
}

void NAND_GetInfo(void)
{
    uint8_t sDummy_ = 0xFF;
    uint8_t recv_ = 0xFF;

    uint8_t sendDummy_[16];
    for (uint8_t i = 0; i < 16; i++) {
        sendDummy_[i] = 0xFF;
    }

    // xSemaphoreTake(xMutex_SPI,
    //                portMAX_DELAY);

    // Read CSD
    bflb_gpio_reset(gpio, GPIO_PIN_22);
    uint8_t CMD9_WRITE[6] = { NAND_CMD9, 0x00, 0x00, 0x00, 0x00, 0xFF };
    bflb_spi_poll_exchange(spi0, CMD9_WRITE, NULL, 6);
    recv_ = 0xFF;
    while (recv_ != 0xFE) {
        bflb_spi_poll_exchange(spi0, &sDummy_, &recv_, 1);
    }
    // printf("recv %02x\r\n", recv);
    bflb_spi_poll_exchange(spi0, sendDummy_, nandCardInfo.rawCsd, 16);
    bflb_gpio_set(gpio, GPIO_PIN_22);
    bflb_spi_poll_exchange(spi0, &sDummy_, NULL, 1);

    printf("%02x\r\n", (nandCardInfo.rawCsd[0] & 0xC0));

    for (int i = 0; i < 15; i++) {
        printf("%02x", nandCardInfo.rawCsd[i]);
    }
    printf("\r\n");

    uint8_t len;

    if ((nandCardInfo.rawCsd[0] & 0xC0) == 0x40) //V2.00的卡
    {
        printf("SDHC\r\n");
        nandCardInfo.blockCount = (nandCardInfo.rawCsd[9] + ((uint32_t)nandCardInfo.rawCsd[8] << 8) + ((uint32_t)(nandCardInfo.rawCsd[7] & 63) << 16) + 1) << 9;
        nandCardInfo.capacity = nandCardInfo.blockCount * 1024;
    } else if ((nandCardInfo.rawCsd[0] & 0xC0) == 0x00) //V1.XX的卡
    {
        printf("SDSC\r\n");
        uint16_t csize = ((uint16_t)(nandCardInfo.rawCsd[8] & 0xC0) >> 6) + ((uint16_t)nandCardInfo.rawCsd[7] << 2) + ((uint16_t)(nandCardInfo.rawCsd[6] & 0x03) << 10);
        uint8_t mult = ((nandCardInfo.rawCsd[10] & 0x80) >> 7) + ((nandCardInfo.rawCsd[9] & 0x03) << 1);
        len = (nandCardInfo.rawCsd[5] & 0x0F);
        // uint16_t csize = (nandCardInfo.rawCsd[8] >> 6) + ((uint16_t)nandCardInfo.rawCsd[7] << 2) + ((uint16_t)(nandCardInfo.rawCsd[6] & 3) << 10) + 1;
        nandCardInfo.capacity = (uint32_t)(csize + 1) * (0x01 << (mult + 2)) * (0x01 << len);
        nandCardInfo.blockCount = nandCardInfo.capacity / nandCardInfo.blockSize;
        printf("csize %d, mult %d, len %d ", csize, mult, len);
    }
    printf("nandCardInfo.blockCount %d\r\n", nandCardInfo.blockCount);
    printf("Size %u\r\n", nandCardInfo.capacity);

    // xSemaphoreGive(xMutex_SPI);
    for (uint16_t i = 0; i < 1024; i++) {
        sendDummy[i] = 0xFF;
    }
    // tx_transfers_nand[0].dst_addr = (uint32_t)DMA_ADDR_SPI0_TDR;
    // rx_transfers_nand[0].src_addr = (uint32_t)DMA_ADDR_SPI0_RDR;
}

uint8_t NAND_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
    uint16_t timeout = 128;

    // printf("NAND W WAIT MUTEX\r\n");
    if (xSemaphoreTake(xMutex_SPI,
                       portMAX_DELAY) == pdTRUE) {
        // printf("SPI0 FREQ %d\r\n", bflb_spi_feature_control(spi0, SPI_CMD_GET_FREQ, 0));
        bflb_spi_feature_control(spi0, SPI_CMD_SET_FREQ, 40 * 1000 * 1000);
        // printf("SPI freq %d\r\n", bflb_spi_feature_control(spi0, SPI_CMD_GET_FREQ, NULL));
        // taskENTER_CRITICAL();
        // bflb_mtimer_delay_us(10);
        // printf("NAND W TAKE xMutex_SPI\r\n");
        // printf("After xMutex_SPI\r\n");

        bflb_gpio_reset(gpio, GPIO_PIN_22);

        // // Write ACMD55
        // uint8_t CMD55_WRITE[6] = { NAND_CMD55, 0x00, 0x00, 0x00, 0x00, 0xFF };
        // bflb_spi_poll_exchange(spi0, CMD55_WRITE, NULL, 6);
        // recv = 0xFF;
        // while (recv != 0x01) {
        //     bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
        // }
        // bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
        // printf("Write CMD55\r\n");

        // // CMD23
        // uint8_t CMD23_WRITE[6] = { NAND_CMD23, (NumberOfBlocks >> 24) & 0xFF, (NumberOfBlocks >> 16) & 0xFF, (NumberOfBlocks >> 8) & 0xFF, NumberOfBlocks & 0xFF, 0xFF };
        // bflb_spi_poll_exchange(spi0, CMD23_WRITE, NULL, 6);
        // recv = 0xFF;
        // while (recv != 23) {
        //     bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
        // }
        // bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
        // printf("Write CMD23\r\n");

        // Wait Busy
        recv = 0x00;
        timeout = 512;
        while ((recv != 0xFF) && timeout) {
            // sDummy = 0xFF;
            spi_rw_dma(&sDummy, &recv, 1);
            timeout--;
            // printf("0xFF recv 0x%02x\r\n", recv);
            if (timeout == 0) {
                bflb_gpio_set(gpio, GPIO_PIN_22);
                spi_w_dma(&sDummy, 1);
                xSemaphoreGive(xMutex_SPI);
                printf("Wait Busy Time Out\r\n");
                return 1;
            }
        }
        // printf("Not Busy\r\n");

        if (NumberOfBlocks == 1) {
            // CMD24 set address
            if ((nandCardInfo.rawCsd[0] & 0xC0) == 0x00) { // SD1.0
                WriteAddr *= BlockSize;
            }
            // CMD24_WRITE[0] = NAND_CMD24;
            CMD24_WRITE[1] = (WriteAddr >> 24) & 0xFF;
            CMD24_WRITE[2] = (WriteAddr >> 16) & 0xFF;
            CMD24_WRITE[3] = (WriteAddr >> 8) & 0xFF;
            CMD24_WRITE[4] = (WriteAddr) & 0xFF;
            // CMD24_WRITE[5] = 0xFF;
            spi_w_dma(CMD24_WRITE, 6);
            // vTaskDelay(1);
            recv = 0xFF;
            // timeout = 128;
            while (recv != 0x00) {
                spi_rw_dma(&sDummy, &recv, 1);
                // printf("CMD24 recv 0x%02x\r\n", recv);
            }
            // printf("CMD24\r\n");

            // Wait Busy
            recv = 0x00;
            timeout = 512;
            while ((recv != 0xFF) && timeout) {
                spi_rw_dma(&sDummy, &recv, 1);
                // printf("0xFF recv 0x%02x\r\n", recv);
                timeout--;
                if (timeout == 0) {
                    bflb_gpio_set(gpio, GPIO_PIN_22);
                    spi_w_dma(&sDummy, 1);
                    xSemaphoreGive(xMutex_SPI);
                    printf("Wait Busy Time Out\r\n");
                    return 1;
                }
            }

            spi_w_dma(&SingleStart, 1);
            spi_w_dma(writebuff, BlockSize);
            spi_w_dma(&sDummy, 1);
            spi_w_dma(&sDummy, 1);
            // bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
            recv = 0x00;
            // timeout = 256;
            while ((recv & 0x1F) != 0x05) { // Wait 0x05. write success.
                spi_rw_dma(&sDummy, &recv, 1);
                // bflb_mtimer_delay_us(1);
                // printf("Single recv 0x%02x\r\n", recv);
            }
            // while (1)
            //     ;
            recv = 0x00;
            while (recv != 0xFF) { // Wait Busy.
                spi_rw_dma(&sDummy, &recv, 1);
                // printf("End recv 0x%02x\r\n", recv);
            }

            spi_w_dma(&sDummy, 1);
            spi_w_dma(&sDummy, 1);
            spi_w_dma(&sDummy, 1);
            // printf("Write End\r\n");

            // CMD

        } else {
            // CMD55
            spi_w_dma(CMD55_WRITE, 6);
            recv = 0xFF;
            while (recv != 0x00) {
                spi_rw_dma(&sDummy, &recv, 1);
                // printf("CMD55 recv 0x%02x\r\n", recv);
            }

            // ACMD23
            CMD23_WRITE[1] = (NumberOfBlocks >> 24) & 0xFF;
            CMD23_WRITE[2] = (NumberOfBlocks >> 16) & 0xFF;
            CMD23_WRITE[3] = (NumberOfBlocks >> 8) & 0xFF;
            CMD23_WRITE[4] = NumberOfBlocks & 0xFF;
            spi_w_dma(CMD23_WRITE, 6);
            recv = 0xFF;
            while (recv != 0x00) {
                spi_rw_dma(&sDummy, &recv, 1);
                // printf("CMD23 recv 0x%02x\r\n", recv);
            }

            // CMD25
            if ((nandCardInfo.rawCsd[0] & 0xC0) == 0x00) { // SD1.0
                WriteAddr *= BlockSize;
                // printf("sd1.0\r\n");
            }
            // CMD25_WRITE[0] = NAND_CMD25;
            CMD25_WRITE[1] = (WriteAddr >> 24) & 0xFF;
            CMD25_WRITE[2] = (WriteAddr >> 16) & 0xFF;
            CMD25_WRITE[3] = (WriteAddr >> 8) & 0xFF;
            CMD25_WRITE[4] = WriteAddr & 0xFF;
            // CMD25_WRITE[5] = 0xFF;
            spi_w_dma(CMD25_WRITE, 6);
            recv = 0xFF;
            // vTaskDelay(1);
            while (recv != 0x00) {
                spi_rw_dma(&sDummy, &recv, 1);
                // printf("CMD25 recv 0x%02x\r\n", recv);
            }
            spi_w_dma(&sDummy, 1);
            // printf("CMD25\r\n");

            timeout = 512;
            recv = 0x00;
            while ((recv != 0xFF) && timeout) {
                spi_rw_dma(&sDummy, &recv, 1);
                timeout--;
                // printf("0xFF recv 0x%02x\r\n", recv);
                if (timeout == 0) {
                    bflb_gpio_set(gpio, GPIO_PIN_22);
                    spi_w_dma(&sDummy, 1);
                    xSemaphoreGive(xMutex_SPI);
                    printf("Wait Busy Time Out\r\n");
                    return 1;
                }
            }
            // printf("Not Busy\r\n");

            // Start Transfer
            uint32_t pData = 0;
            // // Wait Busy
            // recv = 0xFF;
            // timeout = 512;
            // while ((recv != 0xFF) && timeout) {
            //     bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
            //     timeout--;
            //     if (timeout == 0) {
            //         return 1;
            //     }
            // }
            sDummy = 0xFF;
            for (uint8_t i = 0; i < NumberOfBlocks; i++) {
                // printf("WRITE %d\r\n", i);
                spi_w_dma(&MultiStart, 1);
                spi_w_dma(writebuff + pData, BlockSize);
                // spi_rw_dma(&sDummy, NULL, 1);
                // spi_rw_dma(&sDummy, NULL, 1);
                // spi_rw_dma(&sDummy, NULL, 1);
                // bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
                pData += BlockSize;
                recv = 0x00;
                // timeout = 256;
                while ((recv & 0x1F) != 0x05) { // Wait 0x05
                                                // while (recv != 0x05) { // Wait 0x05
                    spi_rw_dma(&sDummy, &recv, 1);
                    // printf("Multi recv 0x%02x\r\n", recv);
                }
                recv = 0x00;
                while (recv != 0xFF) { // Wait Busy.
                    spi_rw_dma(&sDummy, &recv, 1);
                    // printf("End recv 0x%02x\r\n", recv);
                }
            }
            // // Wait Busy
            // timeout = 1024;
            // while ((recv != 0xFF) && timeout) {
            //     bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
            //     printf("End recv 0x%02x\r\n", recv);
            //     timeout--;
            //     if (timeout == 0) {
            //         return 1;
            //     }
            // }

            spi_w_dma(&MultiEnd, 1);
            // printf("Write End\r\n");

            recv = 0xFF;
            timeout = 256;
            while (recv == 0xFF) {
                spi_rw_dma(&sDummy, &recv, 1);
                // printf("End recv 0x%02x\r\n", recv);
            }
        }

        bflb_gpio_set(gpio, GPIO_PIN_22);
        spi_w_dma(&sDummy, 1);
        spi_w_dma(&sDummy, 1);
        spi_w_dma(&sDummy, 1);

        // bflb_mtimer_delay_us(10);
        // printf("NAND W GIVE MUTEX\r\n");
        xSemaphoreGive(xMutex_SPI);
    } else {
        return 1;
    }
    // taskEXIT_CRITICAL();
    // printf("give xMutex_SPI\r\n");

    return 0;
}

uint8_t NAND_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
    uint16_t timeout = 128;

    // printf("NAND RD WAIT MUTEX\r\n");
    if (xSemaphoreTake(xMutex_SPI,
                       portMAX_DELAY) == pdTRUE) {
        // bflb_mtimer_delay_us(10);
        // printf("NAND RD TAKE MUTEX\r\n");
        // taskENTER_CRITICAL();
        bflb_spi_feature_control(spi0, SPI_CMD_SET_FREQ, 40 * 1000 * 1000);
        bflb_gpio_reset(gpio, GPIO_PIN_22);

        // Wait Busy
        timeout = 512;
        recv = 0x00;
        while ((recv != 0xFF) && timeout) {
            spi_rw_dma(&sDummy, &recv, 1);
            timeout--;
            if (timeout == 0) {
                bflb_gpio_set(gpio, GPIO_PIN_22);
                spi_w_dma(&sDummy, 1);
                xSemaphoreGive(xMutex_SPI);
                // printf("NAND RD GIVE MUTEX\r\n");
                return 1;
            }
        }
        // printf("Not Busy\r\n");

        if (NumberOfBlocks == 1) {
            // CMD17
            if ((nandCardInfo.rawCsd[0] & 0xC0) == 0x00) { // SD1.0
                ReadAddr *= BlockSize;
            }
            // CMD17_WRITE[0] = NAND_CMD17;
            CMD17_WRITE[1] = (ReadAddr >> 24) & 0xFF;
            CMD17_WRITE[2] = (ReadAddr >> 16) & 0xFF;
            CMD17_WRITE[3] = (ReadAddr >> 8) & 0xFF;
            CMD17_WRITE[4] = (ReadAddr) & 0xFF;
            // CMD17_WRITE[5] = 0xFF;

            spi_w_dma(CMD17_WRITE, 6);
            recv = 0xFF;
            timeout = 128;
            while ((recv != 0x00) && timeout) {
                spi_rw_dma(&sDummy, &recv, 1);
                // printf("CMD18 0x%02x\r\n", recv);
                timeout--;
                if (timeout == 0) {
                    bflb_gpio_set(gpio, GPIO_PIN_22);
                    spi_w_dma(&sDummy, 1);
                    xSemaphoreGive(xMutex_SPI);
                    // printf("NAND RD GIVE MUTEX\r\n");
                    return 1;
                }
            }

            // // Wait Busy.
            // timeout = 512;
            // recv = 0x00;
            // while ((recv != 0xFF) && timeout) {
            //     bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
            //     // printf("Wait CMD18 recv 0x%02x\r\n", recv);
            //     timeout--;
            //     if (timeout == 0) {
            //         bflb_gpio_set(gpio, GPIO_PIN_22);
            //         bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
            //         xSemaphoreGive(xMutex_SPI);
            //         return 1;
            //     }
            // }

            // Start Read

            recv = 0xFF;
            while (recv != 0xFE) {
                spi_rw_dma(&sDummy, &recv, 1);
                // printf("Read 0xFE recv 0x%02x\r\n", recv);
            }

            spi_rw_dma(sendDummy, readbuff, BlockSize);
            spi_w_dma(&sDummy, 1);
            spi_w_dma(&sDummy, 1);

        } else {
            // CMD18
            if ((nandCardInfo.rawCsd[0] & 0xC0) == 0x00) { // SD1.0
                ReadAddr *= BlockSize;
            }
            // CMD18_WRITE[0] = NAND_CMD18;
            CMD18_WRITE[1] = (ReadAddr >> 24) & 0xFF;
            CMD18_WRITE[2] = (ReadAddr >> 16) & 0xFF;
            CMD18_WRITE[3] = (ReadAddr >> 8) & 0xFF;
            CMD18_WRITE[4] = (ReadAddr) & 0xFF;
            // CMD18_WRITE[5] = 0xFF;
            spi_w_dma(CMD18_WRITE, 6);
            recv = 0xFF;
            timeout = 128;
            while ((recv != 0x00) && timeout) {
                spi_rw_dma(&sDummy, &recv, 1);
                // printf("CMD18 0x%02x\r\n", recv);
                timeout--;
                if (timeout == 0) {
                    bflb_gpio_set(gpio, GPIO_PIN_22);
                    spi_w_dma(&sDummy, 1);
                    xSemaphoreGive(xMutex_SPI);
                    // printf("NAND RD GIVE MUTEX\r\n");
                    return 1;
                }
            }
            // printf("CMD18\r\n");

            // timeout = 512;
            // recv = 0x00;
            // while ((recv != 0xFF) && timeout) {
            //     bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
            //     // printf("Wait CMD18 recv 0x%02x\r\n", recv);
            //     timeout--;
            //     if (timeout == 0) {
            //         bflb_gpio_set(gpio, GPIO_PIN_22);
            //         bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
            //         xSemaphoreGive(xMutex_SPI);
            //         return 1;
            //     }
            // }

            // Start Read
            uint32_t pData = 0;

            // Start Receive
            for (uint8_t i = 0; i < NumberOfBlocks; i++) {
                // Wait 0xFE
                // timeout = 128;
                recv = 0xFF;
                while (recv != 0xFE) {
                    spi_rw_dma(&sDummy, &recv, 1);
                    // printf("Read 0xFE recv 0x%02x\r\n", recv);
                }
                // printf("Wait 0xFE\r\n");

                spi_rw_dma(sendDummy, readbuff + pData, BlockSize);
                spi_w_dma(&sDummy, 1);
                spi_w_dma(&sDummy, 1);
                pData += BlockSize;
            }

            // Stop Receive: CMD12
            spi_w_dma(CMD12_WRITE, 6);
            recv = 0xFF;
            while (recv != 0x00) {
                spi_rw_dma(&sDummy, &recv, 1);
                // printf("Read CMD12 recv 0x%02x\r\n", recv);
            }
            // printf("CMD12\r\n");
        }

        bflb_gpio_set(gpio, GPIO_PIN_22);
        spi_w_dma(&sDummy, 1);
        spi_w_dma(&sDummy, 1);
        spi_w_dma(&sDummy, 1);

        // printf("NAND RD GIVE MUTEX\r\n");
        // bflb_mtimer_delay_us(10);
        xSemaphoreGive(xMutex_SPI);
    } else {
        return 1;
    }
    // taskEXIT_CRITICAL();
    // printf("give xMutex_SPI\r\n");

    return 0;
}