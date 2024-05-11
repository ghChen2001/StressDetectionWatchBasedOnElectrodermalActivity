#include "NAND_SPI.h"
#include <FreeRTOS.h>
#include "semphr.h"
#include "board.h"
#include "bflb_spi.h"
#include "bflb_core.h"

extern SemaphoreHandle_t xMutex_SPI;
extern struct bflb_device_s *gpio;
extern struct bflb_device_s *spi0;

uint8_t NAND_DeviceInit(void)
{
    uint8_t sDummy = 0xFF;
    uint8_t recv = 0xFF;
    bool isSDINIT = false;

    // Wake Up NAND
    uint8_t sendDummy[16];
    for (uint8_t i = 0; i < 16; i++) {
        sendDummy[i] = 0xFF;
    }
    bflb_spi_poll_exchange(spi0, sendDummy, NULL, 12); // At least 74 clks.

    // WRITE CMD0
    bflb_gpio_reset(gpio, GPIO_PIN_22);
    uint8_t CMD0_WRITE[6] = { NAND_CMD0, 0x00, 0x00, 0x00, 0x00, 0x95 };
    recv = 0xFF;
    bflb_spi_poll_exchange(spi0, CMD0_WRITE, NULL, 6);
    while (recv != 0x01) {
        bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
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
    recv = 0xFF;
    while ((recv & 0x80) != 0x00) {
        bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
    }
    bflb_spi_poll_exchange(spi0, sendDummy, NULL, 5);
    bflb_gpio_set(gpio, GPIO_PIN_22);
    if (recv == 0x01) {
        printf("SD 2.0\r\n");
    } else {
        printf("NOT SD 2.0 %02x\r\n", recv);
    }

    uint8_t timeout = 16;
    while (!isSDINIT) {
        // WRITE CMD55
        bflb_gpio_reset(gpio, GPIO_PIN_22);
        uint8_t CMD55_WRITE[6] = { NAND_CMD55, 0x00, 0x00, 0x00, 0x00, 0xFF };
        bflb_spi_poll_exchange(spi0, CMD55_WRITE, NULL, 6);
        recv = 0xFF;
        while (recv != 0x01) {
            bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
        }
        bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
        bflb_gpio_set(gpio, GPIO_PIN_22);
        // printf("CMD55\r\n");

        // WRITE ACMD41
        bflb_gpio_reset(gpio, GPIO_PIN_22);
        uint8_t CMD41_WRITE[6] = { NAND_CMD41, 0x40, 0x00, 0x00, 0x00, 0xFF };
        bflb_spi_poll_exchange(spi0, CMD41_WRITE, NULL, 6);
        recv = 0xFF;
        timeout = 16;
        while ((recv != 0x00) && (timeout != 0)) {
            bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
            timeout--;
        }
        bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
        bflb_gpio_set(gpio, GPIO_PIN_22);

        if (recv == 0x00) {
            isSDINIT = true;
        }
    }
    nandCardInfo.inited = true;

    printf("SD INIT\r\n");

    // Read OCR
    bflb_gpio_reset(gpio, GPIO_PIN_22);
    uint8_t CMD58_WRITE[6] = { NAND_CMD58, 0x00, 0x00, 0x00, 0x00, 0xFF };
    bflb_spi_poll_exchange(spi0, CMD58_WRITE, NULL, 6);
    recv = 0xFF;
    while ((recv & 0x80) != 0x00) {
        bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
    }
    bflb_spi_poll_exchange(spi0, sendDummy, (uint8_t *)&(nandCardInfo.ocr), 4);
    bflb_gpio_set(gpio, GPIO_PIN_22);
    bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
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
    recv = 0xFF;
    while (recv != 0x00) {
        bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
        // printf("CMD16 recv 0x%02x\r\n", recv);
    }
    nandCardInfo.blockSize = 512;
    bflb_gpio_set(gpio, GPIO_PIN_22);
    bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);

    return 0;
}

void NAND_GetInfo(void)
{
    uint8_t sDummy = 0xFF;
    uint8_t recv = 0xFF;

    uint8_t sendDummy[16];
    for (uint8_t i = 0; i < 16; i++) {
        sendDummy[i] = 0xFF;
    }

    // xSemaphoreTake(xMutex_SPI,
    //                portMAX_DELAY);

    // Read CSD
    bflb_gpio_reset(gpio, GPIO_PIN_22);
    uint8_t CMD9_WRITE[6] = { NAND_CMD9, 0x00, 0x00, 0x00, 0x00, 0xFF };
    bflb_spi_poll_exchange(spi0, CMD9_WRITE, NULL, 6);
    recv = 0xFF;
    while (recv != 0xFE) {
        bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
    }
    // printf("recv %02x\r\n", recv);
    bflb_spi_poll_exchange(spi0, sendDummy, nandCardInfo.rawCsd, 16);
    bflb_gpio_set(gpio, GPIO_PIN_22);
    bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);

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
}

uint8_t NAND_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
    uint8_t sDummy = 0xFF;
    uint8_t recv = 0xFF;
    uint16_t timeout = 128;

    // printf("NAND W WAIT MUTEX\r\n");
    xSemaphoreTake(xMutex_SPI,
                   portMAX_DELAY);

    bflb_spi_feature_control(spi0, SPI_CMD_SET_FREQ, 48 * 1000 * 1000);
    // taskENTER_CRITICAL();
    // bflb_mtimer_delay_us(10);
    // printf("NAND W TAKE MUTEX\r\n");
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
        bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
        timeout--;
        if (timeout == 0) {
            bflb_gpio_set(gpio, GPIO_PIN_22);
            bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
            xSemaphoreGive(xMutex_SPI);
            return 1;
        }
    }
    // printf("Not Busy\r\n");

    if (NumberOfBlocks == 1) {
        uint8_t SingleStart = 0xFE;
        // CMD24
        // WriteAddr *= BlockSize;
        uint8_t CMD24_WRITE[6] = { NAND_CMD24, (WriteAddr >> 24) & 0xFF, (WriteAddr >> 16) & 0xFF, (WriteAddr >> 8) & 0xFF, WriteAddr & 0xFF, 0xFF };
        bflb_spi_poll_exchange(spi0, CMD24_WRITE, NULL, 6);
        recv = 0xFF;
        // timeout = 128;
        while (recv != 0x00) {
            bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
            // printf("CMD24 recv 0x%02x\r\n", recv);
        }
        // printf("CMD24\r\n");

        timeout = 512;
        recv = 0x00;
        while ((recv != 0xFF) && timeout) {
            bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
            timeout--;
            if (timeout == 0) {
                bflb_gpio_set(gpio, GPIO_PIN_22);
                bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
                xSemaphoreGive(xMutex_SPI);
                return 1;
            }
        }

        bflb_spi_poll_exchange(spi0, &SingleStart, NULL, 1);
        bflb_spi_poll_exchange(spi0, writebuff, NULL, BlockSize);
        bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
        bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
        // bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
        recv = 0x00;
        // timeout = 256;
        while ((recv & 0x1F) != 0x05) { // Wait 0x05.
            bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
            // bflb_mtimer_delay_us(1);
            // printf("Single recv 0x%02x\r\n", recv);
        }
        recv = 0x00;
        while (recv != 0xFF) { // Wait Busy.
            bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
            // printf("End recv 0x%02x\r\n", recv);
        }

        bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
        bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
        bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
        // printf("Write End\r\n");

        // CMD

    } else {
        // CMD25
        // WriteAddr *= BlockSize;
        uint8_t CMD25_WRITE[6] = { NAND_CMD25, (WriteAddr >> 24) & 0xFF, (WriteAddr >> 16) & 0xFF, (WriteAddr >> 8) & 0xFF, WriteAddr & 0xFF, 0xFF };
        bflb_spi_poll_exchange(spi0, CMD25_WRITE, NULL, 6);
        recv = 0xFF;
        while (recv != 0x00) {
            bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
        }
        bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
        // printf("CMD25\r\n");

        timeout = 512;
        recv = 0x00;
        while ((recv != 0xFF) && timeout) {
            bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
            timeout--;
            if (timeout == 0) {
                bflb_gpio_set(gpio, GPIO_PIN_22);
                bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
                xSemaphoreGive(xMutex_SPI);
                return 1;
            }
        }

        // Start Transfer
        uint32_t pData = 0;
        uint8_t MultiStart = 0xFC;
        uint8_t MultiEnd = 0xFD;
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

        for (uint8_t i = 0; i < NumberOfBlocks; i++) {
            // printf("WRITE %d\r\n", i);
            bflb_spi_poll_exchange(spi0, &MultiStart, NULL, 1);
            bflb_spi_poll_exchange(spi0, writebuff + pData, NULL, BlockSize);
            bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
            bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
            // bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
            pData += BlockSize;
            recv = 0x00;
            // timeout = 256;
            while ((recv & 0x1F) != 0x05) { // Wait 0x05
                bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
                // printf("Multi recv 0x%02x\r\n", recv);
            }
            recv = 0x00;
            while (recv != 0xFF) { // Wait Busy.
                bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
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

        bflb_spi_poll_exchange(spi0, &MultiEnd, NULL, 1);
        // printf("Write End\r\n");

        recv = 0xFF;
        timeout = 256;
        while (recv == 0xFF) {
            bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
            // printf("End recv 0x%02x\r\n", recv);
        }
    }

    bflb_gpio_set(gpio, GPIO_PIN_22);
    bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
    bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
    bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);

    // bflb_mtimer_delay_us(10);
    // printf("NAND W GIVE MUTEX\r\n");
    xSemaphoreGive(xMutex_SPI);
    // taskEXIT_CRITICAL();
    // printf("give xMutex_SPI\r\n");

    return 0;
}

uint8_t NAND_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
    uint8_t sDummy = 0xFF;
    uint8_t recv = 0xFF;
    uint8_t sendDummy[BlockSize];
    uint16_t timeout = 128;

    for (uint16_t i = 0; i < BlockSize; i++) {
        sendDummy[i] = 0xFF;
    }

    // printf("NAND RD WAIT MUTEX\r\n");
    xSemaphoreTake(xMutex_SPI,
                   portMAX_DELAY);
    // bflb_mtimer_delay_us(10);
    // printf("NAND RD TAKE MUTEX\r\n");
    // taskENTER_CRITICAL();
    bflb_spi_feature_control(spi0, SPI_CMD_SET_FREQ, 48 * 1000 * 1000);
    bflb_gpio_reset(gpio, GPIO_PIN_22);

    // Wait Busy
    timeout = 512;
    recv = 0x00;
    while ((recv != 0xFF) && timeout) {
        bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
        timeout--;
        if (timeout == 0) {
            bflb_gpio_set(gpio, GPIO_PIN_22);
            bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
            xSemaphoreGive(xMutex_SPI);
            return 1;
        }
    }
    // printf("Not Busy\r\n");

    if (NumberOfBlocks == 1){
        // CMD18
        // ReadAddr *= BlockSize;
        uint8_t CMD17_WRITE[6] = { NAND_CMD17, (ReadAddr >> 24) & 0xFF, (ReadAddr >> 16) & 0xFF, (ReadAddr >> 8) & 0xFF, ReadAddr & 0xFF, 0xFF };
        bflb_spi_poll_exchange(spi0, CMD17_WRITE, NULL, 6);
        recv = 0xFF;
        timeout = 128;
        while ((recv != 0x00) && timeout) {
            bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
            // printf("CMD18 0x%02x\r\n", recv);
            timeout--;
            if (timeout == 0) {
                bflb_gpio_set(gpio, GPIO_PIN_22);
                bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
                xSemaphoreGive(xMutex_SPI);
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
            bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
            // printf("Read 0xFE recv 0x%02x\r\n", recv);
        }

        bflb_spi_poll_exchange(spi0, sendDummy, readbuff, BlockSize);
        bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
        bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);



    } else {
        // CMD18
        // ReadAddr *= BlockSize;
        uint8_t CMD18_WRITE[6] = { NAND_CMD18, (ReadAddr >> 24) & 0xFF, (ReadAddr >> 16) & 0xFF, (ReadAddr >> 8) & 0xFF, ReadAddr & 0xFF, 0xFF };
        bflb_spi_poll_exchange(spi0, CMD18_WRITE, NULL, 6);
        recv = 0xFF;
        timeout = 128;
        while ((recv != 0x00) && timeout) {
            bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
            // printf("CMD18 0x%02x\r\n", recv);
            timeout--;
            if (timeout == 0) {
                bflb_gpio_set(gpio, GPIO_PIN_22);
                bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
                xSemaphoreGive(xMutex_SPI);
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
                bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
                // printf("Read 0xFE recv 0x%02x\r\n", recv);
            }
            // printf("Wait 0xFE\r\n");

            bflb_spi_poll_exchange(spi0, sendDummy, readbuff + pData, BlockSize);
            bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
            bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
            pData += BlockSize;
        }

        // Stop Receive: CMD12
        uint8_t CMD12_WRITE[6] = { NAND_CMD12, 0x00, 0x00, 0x00, 0x00, 0xFF };
        bflb_spi_poll_exchange(spi0, CMD12_WRITE, NULL, 6);
        recv = 0xFF;
        while (recv != 0x00) {
            bflb_spi_poll_exchange(spi0, &sDummy, &recv, 1);
            // printf("Read CMD12 recv 0x%02x\r\n", recv);
        }
        // printf("CMD12\r\n");
    }

    bflb_gpio_set(gpio, GPIO_PIN_22);
    bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
    bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);
    bflb_spi_poll_exchange(spi0, &sDummy, NULL, 1);

    // printf("NAND RD GIVE MUTEX\r\n");
    // bflb_mtimer_delay_us(10);
    xSemaphoreGive(xMutex_SPI);
    // taskEXIT_CRITICAL();
    // printf("give xMutex_SPI\r\n");

    return 0;
}