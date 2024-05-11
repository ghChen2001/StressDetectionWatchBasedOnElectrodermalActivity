/*******************************************************************************
 * Size: 20 px
 * Bpp: 4
 * Opts: --bpp 4 --size 20 --lcd --font FontAwesome5-Solid+Brands+Regular.woff --range 61475 --format lvgl -o ui_font_lock.c
 ******************************************************************************/

#include "../ui.h"

#ifndef UI_FONT_LOCK
#define UI_FONT_LOCK 1
#endif

#if UI_FONT_LOCK

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+F023 "" */
    0x0, 0xff, 0xe6, 0x9, 0x19, 0x91, 0x6, 0x62,
    0x10, 0xf, 0xfe, 0xe9, 0x2c, 0x5e, 0xff, 0x6e,
    0x62, 0xed, 0x98, 0xde, 0xfe, 0xca, 0x73, 0x10,
    0xf, 0xfe, 0x9a, 0x46, 0x6d, 0x3a, 0x10, 0x7,
    0xff, 0x0, 0x4d, 0x63, 0x3e, 0xd8, 0x80, 0x3f,
    0xf9, 0xa2, 0xb5, 0x47, 0x30, 0xf, 0xf0, 0x8c,
    0x1, 0xfc, 0x29, 0x3f, 0x6, 0x1, 0xff, 0xca,
    0x29, 0x85, 0x10, 0xf, 0x13, 0x56, 0xff, 0xbb,
    0xaf, 0xeb, 0x84, 0x10, 0xf, 0x13, 0xca, 0x80,
    0x7f, 0xf2, 0x5, 0x10, 0x40, 0x1e, 0x27, 0xf9,
    0x52, 0x0, 0xf0, 0xa3, 0xdd, 0x94, 0x3, 0xe4,
    0xc4, 0x0, 0xff, 0xe3, 0x9f, 0x8, 0x7, 0xcf,
    0x84, 0x1, 0xff, 0xc1, 0x7d, 0x30, 0xf, 0x9,
    0x88, 0x7, 0xff, 0x1c, 0x40, 0x3f, 0x8c, 0x3,
    0xff, 0x86, 0x60, 0x1f, 0xc2, 0x1, 0xff, 0xfa,
    0x14, 0x9c, 0xde, 0xfc, 0x72, 0x0, 0xf9, 0xaf,
    0xff, 0xff, 0x85, 0xd0, 0x60, 0x1f, 0x25, 0x7f,
    0xbb, 0x69, 0xcc, 0x3, 0x1c, 0x53, 0x19, 0x8,
    0x7, 0xff, 0x74, 0x49, 0x62, 0xc, 0x20, 0x6,
    0x31, 0x0, 0xff, 0xf2, 0xb7, 0x18, 0x7, 0xff,
    0xfc, 0x3, 0xff, 0xfe, 0x1, 0xff, 0xff, 0x0,
    0xff, 0xff, 0x80, 0x7f, 0xf8, 0x4, 0x3, 0xff,
    0xce, 0x3e, 0x1, 0x27, 0x31, 0x0, 0x7f, 0xf8,
    0xce, 0x94, 0x80, 0x24, 0x9d, 0xd6, 0x5d, 0xff,
    0xf7, 0xf7, 0xb1, 0x88, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 280, .box_w = 57, .box_h = 21, .ofs_x = -1, .ofs_y = -3}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 61475, .range_length = 1, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = 1,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_lock = {
#else
lv_font_t ui_font_lock = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 21,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_HOR,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -7,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_LOCK*/

