/*******************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#include "max30101_drv.h"

//*****************************************************************************
int32_t enableInterrupts(const union InterruptBitField_u data)
{
    char cmdData[3];
    cmdData[0] = InterruptEnable1;
    cmdData[1] = (data.all & 0xF0);
    cmdData[2] = (data.all & 0x02);

    return max30101_i2c_write(I2C_W_ADRS, cmdData, 3);
}

//*****************************************************************************
int32_t getInterruptStatus(union InterruptBitField_u *data)
{
    char local_data[3];
    local_data[0] = InterruptStatus1;

    int8_t result = max30101_i2c_write(I2C_W_ADRS, local_data, 1);
    if (result == 0) {
        result = max30101_i2c_read(I2C_R_ADRS, (local_data + 1), 2);
        if (result == 0) {
            (*data).all = local_data[1] + local_data[2];
        }
    }

    return result;
}

//*****************************************************************************
int32_t setFIFOConfiguration(const union FIFO_Configuration_u config)
{
    return writeRegister(FIFO_Configuration, config.all);
}

//*****************************************************************************
int32_t FIFOConfiguration(union FIFO_Configuration_u *config)
{
    return readRegister(FIFO_Configuration, &((*config).all));
}

//*****************************************************************************
int32_t setModeConfiguration(const union ModeConfiguration_u config)
{
    return writeRegister(ModeConfiguration, (config.all & 0xC7));
}

//*****************************************************************************
int32_t getModeConfiguration(union ModeConfiguration_u *config)
{
    return readRegister(ModeConfiguration, &((*config).all));
}

//*****************************************************************************
int32_t setSpO2Configuration(const union SpO2Configuration_u config)
{
    return writeRegister(SpO2Configuration, (config.all & 0x7F));
}

//*****************************************************************************
int32_t getSpO2Configuration(union SpO2Configuration_u *config)
{
    return readRegister(SpO2Configuration, &((*config).all));
}

//*****************************************************************************
int32_t setLEDPulseAmplitude(enum Registers_e reg, const uint8_t amp)
{
    return writeRegister(reg, amp);
}

//*****************************************************************************
int32_t getLEDPulseAmplitude(enum Registers_e reg, uint8_t *amp)
{
    return readRegister(reg, amp);
}

//*****************************************************************************
int32_t setMultiLEDModeControl(enum Registers_e reg, const union ModeControlReg_u data)
{
    return writeRegister(reg, data.all);
}

//*****************************************************************************
int32_t getMultiLEDModeControl(enum Registers_e reg, union ModeControlReg_u *data)
{
    return readRegister(reg, &((*data).all));
}

//*****************************************************************************
int32_t getDieTemperature(uint16_t *data)
{
    int32_t result = -1;

    //Die temp conversion time is 30ms
    //ATTEMPTS > than 30ms at 100KHz SCL for getInterruptStatus call
    const uint32_t ATTEMPTS = 100;
    uint32_t num_reads = 0;

    //local i2c transaction buffer
    char local_data[2];
    local_data[0] = DieTempConfig;
    local_data[1] = 1;

    //initiate die temp conversion
    result = max30101_i2c_write(I2C_W_ADRS, local_data, 2);
    if (result == 0) {
        //object for holding status registers data
        union InterruptBitField_u status;
        status.all = 0;

        //poll status registers until temp ready, or read fails
        do {
            result = getInterruptStatus(&status);
            num_reads++;
        } while (!status.bits.die_temp && (result == 0) && (num_reads < ATTEMPTS));

        if (status.bits.die_temp) {
            //set pointer to temperature integer register
            local_data[0] = DieTempInt;
            result = max30101_i2c_write(I2C_W_ADRS, local_data, 1);
            if (result == 0) {
                //read two bytes
                result = max30101_i2c_read(I2C_R_ADRS, local_data, 2);
                if (result == 0) {
                    //stuff data
                    *data = ((local_data[0] << 8) | (local_data[1] << 4));
                    *data = ((*data) >> 4);
                }
            }
        } else {
            //if result equals 0 because num_reads exceeded ATTEMPTS,
            //change result to -1.  Otherwise keep result's error code.
            result = (result == 0) ? -1 : result;
        }
    }
    return result;
}

//*****************************************************************************
int32_t getDieTemperatureC(float *data)
{
    uint16_t raw_temp;

    int32_t result = getDieTemperature(&raw_temp);
    if (result == 0) {
        if (raw_temp & 0x0800) {
            *data = ((0xFFFFF000 | raw_temp) / 16.0F);
        } else {
            *data = (raw_temp / 16.0F);
        }
    }

    return result;
}

//*****************************************************************************
float celsius2fahrenheit(float c)
{
    return ((1.8F * c) + 32.0F);
}

//*****************************************************************************
int32_t setProxIntThreshold(const uint8_t data)
{
    return writeRegister(ProxIntThreshold, data);
}

//*****************************************************************************
int32_t getProxIntThreshold(uint8_t *data)
{
    return readRegister(ProxIntThreshold, data);
}

//*****************************************************************************
int32_t readFIFO(enum LedChannels_e numLeds, uint8_t *data, uint16_t *readBytes)
{
    int32_t result = -1;

    readBytes = 0;

    //Get write pointer
    result = readRegister(FIFO_WritePointer, &m_fifoWritePtr);
    if (result == 0) {
        //Get read pointer
        result = readRegister(FIFO_ReadPointer, &m_fifoReadPtr);
        if (result == 0) {
            //Calculate num bytes to read
            if (m_fifoWritePtr > m_fifoReadPtr) {
                m_fifoNumBytes = ((m_fifoWritePtr - m_fifoReadPtr) *
                                  (BYTES_PER_CH * numLeds));
            } else {
                m_fifoNumBytes = (((32 - m_fifoReadPtr) + m_fifoWritePtr) *
                                  (BYTES_PER_CH * numLeds));
            }

            //temporary buffer for data
            char local_data[MAX_FIFO_BYTES];
            local_data[0] = FIFO_DataRegister;

            //Set fifo data ptr
            result = max30101_i2c_write(I2C_W_ADRS, local_data, 1);
            if (result == 0) {
                //read fifo
                result = max30101_i2c_read(I2C_R_ADRS, local_data, m_fifoNumBytes);
                if (result == 0) {
                    //move data to user buffer
                    memcpy(data, local_data, m_fifoNumBytes);
                    readBytes = m_fifoNumBytes;
                }
            }
        }
    }

    return result;
}

//*****************************************************************************
int32_t writeRegister(enum Registers_e reg, uint8_t value)
{
    uint8_t local_data[2] = { reg, value };

    return max30101_i2c_write(I2C_W_ADRS, local_data, 2);
}

//*****************************************************************************
int32_t readRegister(enum Registers_e reg, uint8_t *value)
{
    int8_t result;

    char local_data[2];
    local_data[0] = reg;

    result = max30101_i2c_write(I2C_W_ADRS, local_data, 1);
    if (result == 0) {
        result = max30101_i2c_read(I2C_R_ADRS, (local_data + 1), 1);
        if (result == 0) {
            *value = local_data[1];
        }
    }

    return result;
}

//*****************************************************************************
uint8_t max30101_config()
{
    //Reset Device
    union ModeConfiguration_u modeConfig;
    modeConfig.all = 0;
    modeConfig.bits.reset = 1;
    modeConfig.bits.mode = MultiLedMode; // Sets SPO2 Mode
    int32_t rc = setModeConfiguration(modeConfig);

    //enable MAX30101 interrupts
    union InterruptBitField_u ints;
    if (rc == 0) {
        ints.all = 0;
        ints.bits.a_full = 1;  // Enable FIFO almost full interrupt
        ints.bits.ppg_rdy = 1; //Enables an interrupt when a new sample is ready
        rc = enableInterrupts(ints);
    }

    //configure FIFO
    union FIFO_Configuration_u fifoConfig;
    if (rc == 0) {
        fifoConfig.all = 0;
        fifoConfig.bits.fifo_a_full = 10;                             // Max level of 17 samples
        fifoConfig.bits.sample_average = AveragedSamples_0; // Average 0 samples
        rc = setFIFOConfiguration(fifoConfig);
    }

    union SpO2Configuration_u spo2Config;
    if (rc == 0) {
        spo2Config.all = 0;                            // clears register
        spo2Config.bits.spo2_adc_range = 1;            //sets resolution to 4096 nAfs
        spo2Config.bits.spo2_sr = SR_100_Hz; // SpO2 SR = 100Hz
        spo2Config.bits.led_pw = PW_3;       // 18-bit ADC resolution ~400us
        rc = setSpO2Configuration(spo2Config);
    }

    //Set time slots for LEDS
    union ModeControlReg_u multiLED;
    if (rc == 0) {
        //sets timing for control register 1
        multiLED.bits.lo_slot = 1;
        multiLED.bits.hi_slot = 2;
        rc = setMultiLEDModeControl(ModeControlReg1, multiLED);
        if (rc == 0) {
            multiLED.bits.lo_slot = 3;
            multiLED.bits.hi_slot = 0;
            rc = setMultiLEDModeControl(ModeControlReg2, multiLED);
        }
    }

    //Set LED drive currents
    if (rc == 0) {
        // Heart Rate only, 1 LED channel, Pulse amp. = ~7mA
        rc = setLEDPulseAmplitude(LED1_PA, 0x24);
        //To include SPO2, 2 LED channel, Pulse amp. ~7mA
        if (rc == 0) {
            rc = setLEDPulseAmplitude(LED2_PA, 0x24);
        }
        if (rc == 0) {
            rc = setLEDPulseAmplitude(LED3_PA, 0x24);
        }
    }

    //Set operating mode
    modeConfig.all = 0;
    if (rc == 0) {
        modeConfig.bits.mode = MultiLedMode; // Sets multiLED mode
        rc = setModeConfiguration(modeConfig);
    }

    return rc;
}