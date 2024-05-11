/**
 * @file sh8501a_disp_qspi.h
 * @brief
 *
 *
 */

#ifndef _SH8501A_QSPI_H_
#define _SH8501A_QSPI_H_

#include "../lcd_conf.h"

#if defined LCD_DISP_QSPI_SH8501A

/* Do not modify the following */

#if (SH8501A_QSPI_PIXEL_FORMAT == 1)
#define SH8501A_DISP_QSPI_COLOR_DEPTH 16
typedef uint16_t sh8501a_qspi_color_t;
#elif (SH8501A_QSPI_PIXEL_FORMAT == 2)
#define SH8501A_DISP_QSPI_COLOR_DEPTH 32
typedef uint32_t sh8501a_qspi_color_t;
#endif

typedef struct {
    uint8_t cmd;       /* 0xFF : delay(databytes)ms */
    const char *data;
    uint8_t databytes; /* Num of data in data; or delay time */
} sh8501a_qspi_init_cmd_t;

int sh8501a_qspi_init();
void sh8501a_qspi_async_callback_enable(bool enable);
void sh8501a_qspi_async_callback_register(void (*callback)(void));
int sh8501a_qspi_set_dir(uint8_t dir, uint8_t mir_flag);
void sh8501a_qspi_set_draw_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void sh8501a_qspi_draw_point(uint16_t x, uint16_t y, sh8501a_qspi_color_t color);
void sh8501a_qspi_draw_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, sh8501a_qspi_color_t color);
void sh8501a_qspi_draw_picture_nonblocking(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, sh8501a_qspi_color_t *picture);
void sh8501a_qspi_draw_picture_blocking(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, sh8501a_qspi_color_t *picture);
int sh8501a_qspi_draw_is_busy(void);
// void sh8501a_qspi_set_brightness(uint8_t brightness);

#endif
#endif