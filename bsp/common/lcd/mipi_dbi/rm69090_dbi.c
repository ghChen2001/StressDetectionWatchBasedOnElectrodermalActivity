/**
 * @file rm69090_dbi.c
 * @brief
 *
 * Copyright (c) 2021 Bouffalolab team
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 */

#include "../lcd.h"
#include "rm69090_dbi.h"

#if defined(LCD_DBI_RM69090)

#if (LCD_DBI_INTERFACE_TYPE == 1)
/* dbi */
#include "bl_mipi_dbi.h"

#define lcd_dbi_init                          lcd_dbi_init
#define lcd_dbi_isbusy                        lcd_dbi_is_busy

#define lcd_dbi_transmit_cmd_para             lcd_dbi_transmit_cmd_para
#define lcd_dbi_transmit_cmd_pixel_sync       lcd_dbi_transmit_cmd_pixel_sync
#define lcd_dbi_transmit_cmd_pixel_fill_sync  lcd_dbi_transmit_cmd_pixel_fill_sync

#define lcd_dbi_sync_callback_enable          lcd_dbi_async_callback_enable
#define lcd_dbi_async_callback_register       lcd_dbi_async_callback_register
#define lcd_dbi_transmit_cmd_pixel_async      lcd_dbi_transmit_cmd_pixel_async
#define lcd_dbi_transmit_cmd_pixel_fill_async lcd_dbi_transmit_cmd_pixel_fill_async

lcd_dbi_init_t dbi_para = {
    .clock_freq = 48 * 1000 * 1000,
#if (RM69090_DBI_PIXEL_FORMAT == 1)
    .pixel_format = LCD_DBI_LCD_PIXEL_FORMAT_RGB565,
#elif (RM69090_DBI_PIXEL_FORMAT == 2)
    .pixel_format = LCD_DBI_LCD_PIXEL_FORMAT_NRGB8888,
#endif
};

/* Max clock frequency for rm69090 spi is 50 MHz */

#elif (LCD_DBI_INTERFACE_TYPE == 2)

#else

#error "Configuration error"

#endif

const rm69090_dbi_init_cmd_t rm69090_dbi_init_cmds[] = {
    { 0xFE, "\x00", 1 }, // User Command
    { 0x34, NULL, 0 },   // Disable TE
    { 0x53, "\x20", 1 }, // Dimming Off
    { 0x51, "\xFF", 1 }, // Brightness Max
    // { 0x2A, "\x00\x14\x01\x53", 4 }, // Partial Display Column Start Address
    // { 0x2B, "\x00\x00\x01\x67", 4 }, // Partial Display Row Start Address
    { 0x30, "\x00\x01\x01\x66", 4 }, // Partial Area Horizontal
    { 0x31, "\x00\x15\x01\x52", 4 }, // Partial Area Vertical
    { 0xC4, "\x81", 1 },             // SPI Mode Enable
    // { 0x3A, "\x75", 1 },             // Data Format 16 bit
    { 0x3A, "\xF7", 1 },             // Data Format 32 bit
    { 0x12, NULL, 0 },               // Partial Display Mode On
    // { 0x13, NULL, 0 },   // Normal Display Mode On
    { 0x11, NULL, 0 },   // Sleep Out
    { 0xFF, NULL, 100 }, // Delay > 80 ms
    { 0x29, NULL, 0 },   // Display On
};

/**
 * @brief rm69090_dbi_async_callback_enable
 *
 * @return
 */
void rm69090_dbi_async_callback_enable(bool enable)
{
    lcd_dbi_sync_callback_enable(enable);
}

/**
 * @brief rm69090_dbi_async_callback_register
 *
 * @return
 */
void rm69090_dbi_async_callback_register(void (*callback)(void))
{
    lcd_dbi_async_callback_register(callback);
}

/**
 * @brief rm69090_dbi_draw_is_busy, After the call rm69090_dbi_draw_picture_dma must check this,
 *         if rm69090_dbi_draw_is_busy() == 1, Don't allow other draw !!
 *         can run in the DMA interrupt callback function.
 *
 * @return int 0:draw end; 1:Being draw
 */
int rm69090_dbi_draw_is_busy(void)
{
    return lcd_dbi_isbusy();
}

/**
 * @brief rm69090_dbi_init
 *
 * @return int
 */
int rm69090_dbi_init()
{
    lcd_dbi_init(&dbi_para);

    for (uint16_t i = 0; i < (sizeof(rm69090_dbi_init_cmds) / sizeof(rm69090_dbi_init_cmd_t)); i++) {
        if (rm69090_dbi_init_cmds[i].cmd == 0xFF && rm69090_dbi_init_cmds[i].data == NULL && rm69090_dbi_init_cmds[i].databytes) {
            bflb_mtimer_delay_ms(rm69090_dbi_init_cmds[i].databytes);
        } else {
            lcd_dbi_transmit_cmd_para(rm69090_dbi_init_cmds[i].cmd, (void *)(rm69090_dbi_init_cmds[i].data), rm69090_dbi_init_cmds[i].databytes);
        }
    }

    printf("RM69090 init\n");
    return 0;
}

/**
 * @brief
 *
 * @param dir
 * @param mir_flag
 */
int rm69090_dbi_set_dir(uint8_t dir, uint8_t mir_flag)
{
    // uint8_t param;
    // switch (dir) {
    //     case 0:
    //         if (!mir_flag)
    //             param = 0x00;
    //         else
    //             param = 0x40;
    //         break;
    //     case 1:
    //         if (!mir_flag)
    //             param = 0x20;
    //         else
    //             param = 0xA0;
    //         break;
    //     case 2:
    //         if (!mir_flag)
    //             param = 0x80;
    //         else
    //             param = 0xC0;
    //         break;
    //     case 3:
    //         if (!mir_flag)
    //             param = 0xE0;
    //         else
    //             param = 0x60;

    //         break;
    //     default:
    //         return -1;
    //         break;
    // }
    // lcd_dbi_transmit_cmd_para(0x36, (void *)&param, 1);
    // return dir;
}

/**
 * @brief rm69090_dbi_set_draw_window
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
void rm69090_dbi_set_draw_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
#if RM69090_DBI_OFFSET_X
    x1 += RM69090_DBI_OFFSET_X;
    x2 += RM69090_DBI_OFFSET_X;
#endif
#if RM69090_DBI_OFFSET_Y
    y1 += RM69090_DBI_OFFSET_Y;
    y2 += RM69090_DBI_OFFSET_Y;
#endif

    uint8_t param[4];

    param[0] = (y1 >> 8) & 0xFF;
    param[1] = y1 & 0xFF;
    param[2] = (y2 >> 8) & 0xFF;
    param[3] = y2 & 0xFF;
    lcd_dbi_transmit_cmd_para(0x2B, (void *)param, 4); // Set Row Start Address

    param[0] = (x1 >> 8) & 0xFF;
    param[1] = x1 & 0xFF;
    param[2] = (x2 >> 8) & 0xFF;
    param[3] = x2 & 0xFF;
    lcd_dbi_transmit_cmd_para(0x2A, (void *)param, 4); // Set Column Start Address
}

/**
 * @brief rm69090_dbi_draw_point
 *
 * @param x
 * @param y
 * @param color
 */
void rm69090_dbi_draw_point(uint16_t x, uint16_t y, rm69090_dbi_color_t color)
{
    /* set window */
    rm69090_dbi_set_draw_window(x, y, x, y);

    lcd_dbi_transmit_cmd_pixel_sync(0x2C, (void *)&color, 1); // Memory Write
}

/**
 * @brief rm69090_dbi_draw_area
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param color
 */
void rm69090_dbi_draw_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, rm69090_dbi_color_t color)
{
    uint32_t pixel_num = (x2 - x1 + 1) * (y2 - y1 + 1);
    uint32_t color_src;

#if RM69090_DBI_COLOR_DEPTH == 16
    color_src = color << 16 | color;
#elif RM69090_DBI_COLOR_DEPTH == 32
    color_src = color;
#endif

    /* set window */
    rm69090_dbi_set_draw_window(x1, y1, x2, y2);

    lcd_dbi_transmit_cmd_pixel_fill_sync(0x2C, color_src, pixel_num); // Memory Write
}

/**
 * @brief rm69090_dbi_draw_picture_dma, Non-blocking! Using DMA acceleration, Not waiting for the draw end
 *  After the call, No other operations are allowed until (rm69090_dbi_draw_is_busy()==0)
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param picture
 */

void rm69090_dbi_draw_picture_nonblocking(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, rm69090_dbi_color_t *picture)
{
    uint32_t pixel_num = (x2 - x1 + 1) * (y2 - y1 + 1);

    /* set window */ 
    rm69090_dbi_set_draw_window(x1, y1, x2, y2);

    lcd_dbi_transmit_cmd_pixel_async(0x2C, (void *)picture, pixel_num); // Memory Write
}

/**
 * @brief rm69090_dbi_draw_picture,Blocking，Using DMA acceleration,Waiting for the draw end
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param picture
 */
void rm69090_dbi_draw_picture_blocking(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, rm69090_dbi_color_t *picture)
{
    uint32_t pixel_num = (x2 - x1 + 1) * (y2 - y1 + 1);

    /* set window */
    rm69090_dbi_set_draw_window(x1, y1, x2, y2);

    lcd_dbi_transmit_cmd_pixel_sync(0x2C, (void *)picture, pixel_num); // Memory Write
}

#endif
