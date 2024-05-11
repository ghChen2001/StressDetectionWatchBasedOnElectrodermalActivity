/**
 * @file sh8501a_disp_qspi.c
 * @brief
 *
 *
 */

#include "../lcd.h"
#include "sh8501a_disp_qspi.h"
#include "bflb_dbi.h"

#if defined(LCD_DISP_QSPI_SH8501A)

#if (LCD_DBI_INTERFACE_TYPE == 1)
/* dbi */
#include "../mipi_dbi/bl_mipi_dbi.h"


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
#if (SH8501A_QSPI_PIXEL_FORMAT == 1)
    .pixel_format = LCD_DBI_LCD_PIXEL_FORMAT_RGB565,
#elif (SH8501A_QSPI_PIXEL_FORMAT == 2)
    .pixel_format = LCD_DBI_LCD_PIXEL_FORMAT_NRGB8888,
#endif
    .cmd_wire_dual_en = 0,
    .addr_wire_dual_en = 0,
    .data_wire_dual_en = 0,
};

/* pec dbi typeB device */
static struct bflb_device_s *dbi_hd;

#elif (LCD_DBI_INTERFACE_TYPE == 2)
#else

#error "Configuration error"

#endif

const sh8501a_qspi_init_cmd_t sh8501a_qspi_init_cmds[] = {
    //     { 0x01, NULL, 0 },
    //     { 0xFF, NULL, 20 },
    //     { 0x11, NULL, 0 }, /* Exit sleep */
    //     { 0xFF, NULL, 120 },

    //     { 0xCF, "\x00\xd9\x30", 3 },
    //     { 0xED, "\x64\x03\x12\x81", 4 },
    //     { 0xE8, "\x85\x01\x78", 3 },
    //     { 0xCB, "\x39\x2C\x00\x34\x02", 5 },
    //     { 0xF7, "\x20", 1 },
    //     { 0xEA, "\x00\x00", 2 },

    //     { 0xC0, "\x23", 1 },     /*Power control*/
    //     { 0xC1, "\x12", 1 },     /*Power control */
    //     { 0xC2, "\x11", 1 },
    //     { 0xC5, "\x40\x30", 2 }, /*VCOM control 1*/
    //     { 0xC7, "\xa9", 1 },     /*VCOM control 2*/
    //     { 0x36, "\x08", 1 },     /*Memory Access Control*/

    // #if (ILI9341_DBI_PIXEL_FORMAT == 1)
    //     { 0x3A, "\x55", 1 }, /* Interface Pixel Format RGB565 */
    // #elif (ILI9341_DBI_PIXEL_FORMAT == 2)
    //     { 0x3A, "\x66", 1 }, /* Interface Pixel Format RGB666 */
    // #endif

    //     { 0xB1, "\x00\x18", 2 }, /* Frame Rate Control */
    //     { 0xB6, "\x0a\xa2", 2 }, /* Display Function Control */
    //     { 0x0C, "\xd5", 1 },     /* display pixel format,RGB 16bits,MCU 16bits  */
    //     { 0xF2, "\x00", 1 },     /* 3Gamma Function Disable */
    //     { 0xF7, "\x20", 1 },

    //     { 0x26, "\x01", 1 },                                                          /* Gamma curve selected */
    //     { 0xE0, "\x1F\x1A\x18\x0A\x0F\x06\x45\x87\x32\x0A\x07\x02\x07\x05\x00", 15 }, /* Set Gamma */
    //     { 0XE1, "\x00\x25\x27\x05\x10\x09\x3A\x78\x4D\x05\x18\x0D\x38\x3A\x1F", 15 }, /* Set Gamma */
    //     { 0xB7, "\x07", 1 },

    // #if ILI9341_DBI_COLOR_REVERSAL
    //     { 0x21, NULL, 0 }, /* Color reversal */
    // #endif

    //     { 0x29, NULL, 0 }, /* Display On */
    //     { 0xFF, NULL, 10 },

    { 0x11, NULL, 0 },   // Sleep Out
    { 0xFF, NULL, 100 }, // Delay 100 ms
    { 0x28, NULL, 0 },   // Display Off
    // { 0xC0, "\x5A\x5A", 2 },
    // { 0xBA, "\x81", 1 },
    // { 0xC1, "\x5A\x5A", 2 },
    { 0x2A, "\x00\x00\x00\xEF", 4 },
    { 0x2B, "\x00\x00\x01\x27", 4 },
    { 0x44, "\x01\x27", 2 },
    { 0x3A, "\x05", 1 }, // rgb565
    // { 0x35, "\x00", 1 },
    { 0x34, NULL, 0 }, // TE
    { 0x53, "\x20", 1 },
    { 0x51, "\xB3", 1 }, // Brightness 70%
    { 0xFF, NULL, 60 },
    { 0x29, NULL, 0 }, // Display On
    // { 0x23, NULL, 0 },
};

/**
 * @brief sh8501a_qspi_async_callback_enable
 *
 * @return
 */
void sh8501a_qspi_async_callback_enable(bool enable)
{
    lcd_dbi_sync_callback_enable(enable);
}

/**
 * @brief sh8501a_qspi_async_callback_register
 *
 * @return
 */
void sh8501a_qspi_async_callback_register(void (*callback)(void))
{
    lcd_dbi_async_callback_register(callback);
}

/**
 * @brief sh8501a_qspi_draw_is_busy, After the call sh8501a_qspi_draw_picture_dma must check this,
 *         if sh8501a_qspi_draw_is_busy() == 1, Don't allow other draw !!
 *         can run in the DMA interrupt callback function.
 *
 * @return int 0:draw end; 1:Being draw
 */
int sh8501a_qspi_draw_is_busy(void)
{
    return lcd_dbi_isbusy();
}

/**
 * @brief sh8501a_qspi_init
 *
 * @return int
 */
int sh8501a_qspi_init()
{
    dbi_hd = bflb_device_get_by_name("dbi");
    lcd_dbi_init(&dbi_para);
    int i = 0;

    for (uint16_t i = 0; i < (sizeof(sh8501a_qspi_init_cmds) / sizeof(sh8501a_qspi_init_cmd_t)); i++) {
        if (sh8501a_qspi_init_cmds[i].cmd == 0xFF && sh8501a_qspi_init_cmds[i].data == NULL && sh8501a_qspi_init_cmds[i].databytes) {
            bflb_mtimer_delay_ms(sh8501a_qspi_init_cmds[i].databytes);
        } else {
            bflb_dbi_qspi_set_addr(dbi_hd, 3, sh8501a_qspi_init_cmds[i].cmd<<8);
            lcd_dbi_transmit_cmd_para(0x02, (void *)(sh8501a_qspi_init_cmds[i].data), sh8501a_qspi_init_cmds[i].databytes);
        }
    }
    printf("SH8501A INIT\r\n");
    // while(1);
    // bflb_dbi_qspi_set_addr(dbi_hd, 3, 0x2C << 8);
    // lcd_dbi_transmit_cmd_para(0x12, NULL, 0);
    // for(i = 0; i < 1; i+=1)
    // {
    //     sh8501a_qspi_draw_area(50+i, 50, 250+i, 250, 0x1111);
    //     bflb_mtimer_delay_ms(250);
    // }

    // sh8501a_qspi_draw_area(0, 0, 320, 386, 0x1111);
    // bflb_mtimer_delay_ms(250);
    // // sh8501a_qspi_draw_area(0, 0, 50, 50, 0x1111);
    // // sh8501a_qspi_draw_area(0, 0, 320, 386, 0x0011);

    // for(i = 0; i < 1; i+=1)
    // {
    //     sh8501a_qspi_draw_point(55+i,55,0xffff);
    //     bflb_mtimer_delay_ms(10);
    // }
    // bflb_mtimer_delay_ms(1000);
    return 0;
}

/**
 * @brief
 *
 * @param dir
 * @param mir_flag
 */
int sh8501a_qspi_set_dir(uint8_t dir, uint8_t mir_flag)
{
    uint8_t param;
    switch (dir) {
        case 0:
            if (!mir_flag)
                param = 0x00;
            else
                param = 0x01;
            break;
        case 1:
            if (!mir_flag)
                param = 0x60;
            else
                param = 0x20;
            break;
        case 2:
            if (!mir_flag)
                param = 0xC0;
            else
                param = 0x80;
            break;
        case 3:
            if (!mir_flag)
                param = 0xA0;
            else
                param = 0xE0;

            break;
        default:
            return -1;
            break;
    }
    // bflb_dbi_qspi_set_addr(dbi_hd, 3, 0x36 << 8);
    // lcd_dbi_transmit_cmd_para(0x12, (void *)&param, 1);
    return dir;
}

/**
 * @brief sh8501a_qspi_set_draw_window
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
void sh8501a_qspi_set_draw_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
#if SH8501A_QSPI_OFFSET_X
    x1 += SH8501A_QSPI_OFFSET_X;
    x2 += SH8501A_QSPI_OFFSET_X;
#endif
#if SH8501A_QSPI_OFFSET_Y
    y1 += SH8501A_QSPI_OFFSET_Y;
    y2 += SH8501A_QSPI_OFFSET_Y;
#endif

    uint8_t param[4];

    // bflb_mtimer_delay_us(1000);
    param[0] = (x1 >> 8) & 0xFF;
    param[1] = x1 & 0xFF;
    param[2] = (x2 >> 8) & 0xFF;
    param[3] = x2 & 0xFF;
    // printf("x1x2 %x%x_%x%x\r\n", param[0], param[1], param[2], param[3]);

    bflb_dbi_qspi_set_addr(dbi_hd, 3, 0x2a << 8);
    lcd_dbi_transmit_cmd_para(0x02, (void *)param, 4);
    param[0] = (y1 >> 8) & 0xFF;
    param[1] = y1 & 0xFF;
    param[2] = (y2 >> 8) & 0xFF;
    param[3] = y2 & 0xFF;
    // printf("y1y2 %x%x_%x%x\r\n", param[0], param[1], param[2], param[3]);

    bflb_dbi_qspi_set_addr(dbi_hd, 3, 0x2b << 8);
    lcd_dbi_transmit_cmd_para(0x02, (void *)param, 4);

    // bflb_mtimer_delay_us(1000);
}
/**
 * @brief sh8501a_qspi_draw_point
 *
 * @param x
 * @param y
 * @param color
 */
void sh8501a_qspi_draw_point(uint16_t x, uint16_t y, sh8501a_qspi_color_t color)
{
    /* set window */
    sh8501a_qspi_set_draw_window(x-1, y-1, x+1, y+1);

    // bflb_dbi_qspi_set_addr(dbi_hd, 3, 0x2C << 8);
    // lcd_dbi_transmit_cmd_pixel_sync(0x12, NULL, 0);
    bflb_dbi_qspi_set_addr(dbi_hd, 3, 0x2C << 8);
    lcd_dbi_transmit_cmd_pixel_sync(0x02, (void *)&color, 4);
}

/**
 * @brief sh8501a_qspi_draw_area
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param color
 */
void sh8501a_qspi_draw_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, sh8501a_qspi_color_t color)
{
    uint32_t pixel_num = (x2 - x1 + 1) * (y2 - y1 + 1);
    uint32_t color_src;

#if SH8501A_DISP_QSPI_COLOR_DEPTH == 16
    color_src = color << 16 | color;
    // color_src = 0x0000 <<16 | color;
#elif SH8501A_DISP_QSPI_COLOR_DEPTH == 32
    color_src = color;
#endif

    /* set window */
    sh8501a_qspi_set_draw_window(x1, y1, x2, y2);

    // bflb_dbi_qspi_set_addr(dbi_hd, 3, 0x2C << 8);
    // lcd_dbi_transmit_cmd_pixel_fill_sync(0x12, NULL, 0);
    bflb_dbi_qspi_set_addr(dbi_hd, 3, 0x2C << 8);
    lcd_dbi_transmit_cmd_pixel_fill_sync(0x02, color_src, pixel_num);
}

/**
 * @brief sh8501a_qspi_draw_picture_dma, Non-blocking! Using DMA acceleration, Not waiting for the draw end
 *  After the call, No other operations are allowed until (sh8501a_qspi_draw_is_busy()==0)
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param picture
 */

void sh8501a_qspi_draw_picture_nonblocking(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, sh8501a_qspi_color_t *picture)
{
    uint32_t pixel_num = (x2 - x1 + 1) * (y2 - y1 + 1);

    /* set window */
    sh8501a_qspi_set_draw_window(x1, y1, x2, y2);

    // bflb_dbi_qspi_set_addr(dbi_hd, 3, 0x2C << 8);
    // lcd_dbi_transmit_cmd_pixel_async(0x12, NULL, 0);
    bflb_dbi_qspi_set_addr(dbi_hd, 3, 0x2C << 8);
    lcd_dbi_transmit_cmd_pixel_async(0x02, (void *)picture, pixel_num);
}

/**
 * @brief sh8501a_qspi_draw_picture,Blocking，Using DMA acceleration,Waiting for the draw end
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param picture
 */
void sh8501a_qspi_draw_picture_blocking(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, sh8501a_qspi_color_t *picture)
{
    uint32_t pixel_num = (x2 - x1 + 1) * (y2 - y1 + 1);

    /* set window */
    sh8501a_qspi_set_draw_window(x1, y1, x2, y2);

    // bflb_dbi_qspi_set_addr(dbi_hd, 3, 0x2C << 8);
    // lcd_dbi_transmit_cmd_pixel_sync(0x12, NULL, 0);
    bflb_dbi_qspi_set_addr(dbi_hd, 3, 0x2C << 8);
    lcd_dbi_transmit_cmd_pixel_sync(0x02, (void *)picture, pixel_num);
}

// /**
//  * @brief sh8501a_qspi_set_brightness, Blocking
//  *
//  * @param brightness
//  */
void sh8501a_qspi_set_brightness(uint8_t brightness)
{
    uint8_t b = brightness;
    bflb_dbi_qspi_set_addr(dbi_hd, 3, 0x51 << 8); // CMD 0x51

    lcd_dbi_transmit_cmd_para(0x02, (void *)&b, 1);
}

#endif
