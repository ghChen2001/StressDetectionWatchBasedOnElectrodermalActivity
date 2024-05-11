#ifndef NAND_SPI_H__
#define NAND_SPI_H__

#include "bl616_glb.h"

// SD卡类型定义
#define NAND_TYPE_ERR          0X00 //卡类型错误
#define NAND_TYPE_MMC          0X01 //MMC卡
#define NAND_TYPE_V1           0X02
#define NAND_TYPE_V2           0X04
#define NAND_TYPE_V2HC         0X06

// SD卡指令表
#define NAND_CMD0              0 | 0x40  //卡复位
#define NAND_CMD2              2 | 0x40  //读CID数据
#define NAND_CMD8              8 | 0x40  //命令8 ，SEND_IF_COND
#define NAND_CMD9              9 | 0x40  //命令9 ，读CSD数据
#define NAND_CMD10             10 | 0x40 //命令10，读CID数据
#define NAND_CMD12             12 | 0x40 //命令12，停止数据传输
#define NAND_CMD16             16 | 0x40 //命令16，设置扇区大小 应返回0x00
#define NAND_CMD17             17 | 0x40 //命令17，读扇区
#define NAND_CMD18             18 | 0x40 //命令18，读Multi 扇区
#define NAND_CMD23             23 | 0x40 //命令23，设置多扇区写入前预先擦除N个block
#define NAND_CMD24             24 | 0x40 //命令24，写扇区
#define NAND_CMD25             25 | 0x40 //命令25，写多个扇区
#define NAND_CMD41             41 | 0x40 //命令41，应返回0x00
#define NAND_CMD55             55 | 0x40 //命令55，应返回0x01
#define NAND_CMD58             58 | 0x40 //命令58，读OCR信息
#define NAND_CMD59             59 | 0x40 //命令59，使能/禁止CRC，应返回0x00、

/*SD卡回应标记字*/
#define NAND_RESPONSE_NO_ERROR 0x00 //正确回应
#define NAND_SD_IN_IDLE_STATE  0x01 //闲置状态
#define NAND_SD_ERASE_RESET    0x02 //擦除复位
#define NAND_RESPONSE_FAILURE  0xFF //响应失败

/*! @brief SD card CID register */
typedef struct _nand_cid {
    uint8_t manufacturerID;       /*!< Manufacturer ID [127:120] */
    uint16_t applicationID;       /*!< OEM/Application ID [119:104] */
    uint8_t productName[5];       /*!< Product name [103:64] */
    uint8_t productVersion;       /*!< Product revision [63:56] */
    uint32_t productSerialNumber; /*!< Product serial number [55:24] */
    uint16_t manufacturerData;    /*!< Manufacturing date [19:8] */
} nand_cid_t;

/*! @brief SD card CSD register */
typedef struct _nand_csd {
    uint8_t csdStructure;        /*!< CSD structure [127:126] */
    uint8_t dataReadAccessTime1; /*!< Data read access-time-1 [119:112] */
    uint8_t dataReadAccessTime2; /*!< Data read access-time-2 in clock cycles (NSAC*100) [111:104] */
    uint8_t transferSpeed;       /*!< Maximum data transfer rate [103:96] */
    uint16_t cardCommandClass;   /*!< Card command classes [95:84] */
    uint8_t readBlockLength;     /*!< Maximum read data block length [83:80] */
    uint16_t flags;              /*!< Flags in _sd_csd_flag */
    uint32_t deviceSize;         /*!< Device size [73:62] */
    /* Following fields from 'readCurrentVddMin' to 'deviceSizeMultiplier' exist in CSD version 1 */
    uint8_t readCurrentVddMin;    /*!< Maximum read current at VDD min [61:59] */
    uint8_t readCurrentVddMax;    /*!< Maximum read current at VDD max [58:56] */
    uint8_t writeCurrentVddMin;   /*!< Maximum write current at VDD min [55:53] */
    uint8_t writeCurrentVddMax;   /*!< Maximum write current at VDD max [52:50] */
    uint8_t deviceSizeMultiplier; /*!< Device size multiplier [49:47] */

    uint8_t eraseSectorSize;       /*!< Erase sector size [45:39] */
    uint8_t writeProtectGroupSize; /*!< Write protect group size [38:32] */
    uint8_t writeSpeedFactor;      /*!< Write speed factor [28:26] */
    uint8_t writeBlockLength;      /*!< Maximum write data block length [25:22] */
    uint8_t fileFormat;            /*!< File format [11:10] */
} nand_csd_t;

/*!
 * @brief SD card state
 *
 * Define the card structure including the necessary fields to identify and describe the card.
 */
typedef struct _nand_card {
    uint32_t relativeAddress; /*!< Relative address of the card */
    uint32_t version;         /*!< Card version */
    uint32_t flags;           /*!< Flags in _nand_card_flag */
    uint8_t rawCid[16];      /*!< Raw CID content */
    uint8_t rawCsd[16];      /*!< Raw CSD content */
    uint32_t ocr;             /*!< Raw OCR content */
    nand_cid_t cid;             /*!< CID */
    nand_csd_t csd;             /*!< CSD */
    uint32_t blockCount;      /*!< Card total block number */
    uint32_t blockSize;       /*!< Card block size */
    uint32_t capacity;
    bool inited;
} nand_card_t;

nand_card_t nandCardInfo;

// void NAND_ReadWriteNBytes(unsigned char *pSendBuffer, unsigned char *pRecvBuff, unsigned long length);

// uint8_t NAND_WaitBusy(void); //等待SD卡准备 return 0: not busy, 1: busy

// uint8_t NAND_GetAck(uint8_t Response); //获得应答 return 0: not ack, 1: ack

uint8_t NAND_DeviceInit(void); //初始化

// uint8_t SDCardReadData(uint8_t *buf, uint32_t sector, uint32_t cnt); //读块(扇区)

// uint8_t SDCardWriteData(uint8_t *buf, uint32_t sector, uint32_t cnt); //写块(扇区)

// uint32_t GetSDCardSectorCount(void); //读扇区数

// uint8_t GetSDCardCISDCardOutnfo(uint8_t *cid_data); //读SD卡CID

// uint8_t GetSDCardCSSDCardOutnfo(uint8_t *csd_data); //读SD卡CSD

void NAND_GetInfo(void);

uint8_t NAND_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
uint8_t NAND_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);

#endif