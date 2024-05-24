/**
 * @file fatfs_spi_nand.c
 * @brief
 * No Brief. 
 *
 */

#if defined(BL616) || defined(BL808) || defined(BL628) || defined(BL606P)

#include "ff.h"     /* Obtains integer types */
#include "diskio.h" /* Declarations of disk functions */
#include "NAND_SPI.h"
#include "bflb_irq.h"

#define SINGLE_READ_LENGTH 1024
#define SINGLE_WRITE_LENGTH 1024 * 32

int MMC_disk_status()
{
    return 0;
}

int MMC_disk_initialize()
{
    static uint8_t inited = 0;

    if (inited == 0) {
        if (nandCardInfo.inited == true) {
            inited = 1;
            return 0;
        } else {
            return -1;
        }
    }

    return 0;
}

int MMC_disk_read(BYTE *buff, LBA_t sector, UINT count)
{
    uint8_t ret;
    // uint16_t rCnt = 0;

// _retry:
//     ret = NAND_ReadMultiBlocks(buff, sector, nandCardInfo.blockSize, count);

    // printf("count %d\r\n", count);
    // while (count >= SINGLE_READ_LENGTH) {
    _retry:
        ret = NAND_ReadMultiBlocks((uint8_t *)buff, sector, nandCardInfo.blockSize, count);
        if (1 == ret) {
            goto _retry;
        }
    //     count -= SINGLE_READ_LENGTH;
    //     rCnt++;
    //     vTaskDelay(1);
    // }
    // if (count > 0) {
    // _retry_:
    //     ret = NAND_ReadMultiBlocks((uint8_t *)buff + nandCardInfo.blockSize * SINGLE_READ_LENGTH * rCnt, sector + SINGLE_READ_LENGTH * rCnt, nandCardInfo.blockSize, count);
    //     vTaskDelay(1);
    //     if (1 == ret) {
    //         goto _retry_;
    //     }
    // }

    if (0 == ret) {
        return 0;
    } else {
        return -1;
    }
}

int MMC_disk_write(const BYTE *buff, LBA_t sector, UINT count)
{
    uint8_t ret;
    // uint16_t wCnt = 0;

    // printf("count %d\r\n", count);
    // while (count >= SINGLE_WRITE_LENGTH) {
    _retry:
        ret = NAND_WriteMultiBlocks((uint8_t *)buff, sector, nandCardInfo.blockSize, count);
        if (1 == ret) {
            goto _retry;
        }
    //     count -= SINGLE_WRITE_LENGTH;
    //     wCnt++;
    //     vTaskDelay(1);
    // }
    // if (count > 0) {
    // _retry_:
    //     ret = NAND_WriteMultiBlocks((uint8_t *)buff + nandCardInfo.blockSize * SINGLE_WRITE_LENGTH * wCnt, sector + SINGLE_WRITE_LENGTH * wCnt, nandCardInfo.blockSize, count);
    //     vTaskDelay(1);
    //     if (1 == ret) {
    //         goto _retry_;
    //     }
    // }

    if (0 == ret) {
        return 0;
    } else {
        return -1;
    }
}

int MMC_disk_ioctl(BYTE cmd, void *buff)
{
    // printf("CMD %d\r\n", cmd);
    switch (cmd) {
        // Get R/W sector size (WORD)
        case GET_SECTOR_SIZE:
            *(WORD *)buff = nandCardInfo.blockSize;
            break;

        // Get erase block size in unit of sector (DWORD)
        case GET_BLOCK_SIZE:
            *(DWORD *)buff = 1;
            break;

        case GET_SECTOR_COUNT:
            *(DWORD *)buff = nandCardInfo.blockCount;
            break;

        case CTRL_SYNC:
            break;

        default:
            break;
    }

    return 0;
}

DSTATUS Translate_Result_Code(int result)
{
    return result;
}

void fatfs_nand_driver_register(void)
{
    FATFS_DiskioDriverTypeDef NAND_DiskioDriver = { NULL };

    NAND_DiskioDriver.disk_status = MMC_disk_status;
    NAND_DiskioDriver.disk_initialize = MMC_disk_initialize;
    NAND_DiskioDriver.disk_write = MMC_disk_write;
    NAND_DiskioDriver.disk_read = MMC_disk_read;
    NAND_DiskioDriver.disk_ioctl = MMC_disk_ioctl;
    NAND_DiskioDriver.error_code_parsing = Translate_Result_Code;

    disk_driver_callback_init(DEV_SD2, &NAND_DiskioDriver);
}

#endif
