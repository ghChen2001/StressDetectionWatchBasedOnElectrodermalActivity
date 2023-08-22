#include "m117_drv.h"

/**-----------------------------------------------------------------------
  * @brief  计算多个字节序列的校验和
  * @param  data：字节数组指针
  * @param  nbrOfBytes：字节数组的长度
  * @retval 校验和（CRC）
-------------------------------------------------------------------------*/
#define POLYNOMIAL 0x131 //100110001
static uint8_t MY_I2C_CRC8(uint8_t *data1, uint8_t nbrOfBytes)
{
    uint8_t bit1;       // bit mask
    uint8_t crc = 0xFF; // calculated checksum
    uint8_t byteCtr;    // byte counter
    // calculates 8-Bit checksum with given polynomial
    for (byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++) {
        crc ^= (data1[byteCtr]);
        for (bit1 = 8; bit1 > 0; --bit1) {
            if (crc & 0x80)
                crc = (crc << 1) ^ POLYNOMIAL;
            else
                crc = (crc << 1);
        }
    }
    return crc;
}

void m117_soft_reset()
{
    m117_i2c_write(M117_SOFT_RST, NULL, 0);
}

void m117_config(void)
{
    uint8_t txbuf[3] = { 0x02, 0xFF, 0x00};
    txbuf[2] = MY_I2C_CRC8(txbuf, 2);
    m117_i2c_write(M117_WRITE_CONFIG, txbuf, 3);
}

void m117_convert()
{
    m117_i2c_write(M117_START_COV, NULL, 0);
}

uint8_t m117_read_value(uint16_t * valuebuf)
{
    uint8_t temp[3] = {0}; 
    m117_i2c_read(M117_READ_TEMP, temp, 3);
    if(temp[3] == MY_I2C_CRC8(temp, 2))
    {
        *valuebuf = ((uint16_t)temp[0] << 8) | (temp[1]);
        return 1;
    }
    else
        return 0; // CRC Error
}

