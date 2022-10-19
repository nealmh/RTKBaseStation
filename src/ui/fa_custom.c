/*******************************************************************************
 * Size: 14 px
 * Bpp: 4
 * Opts: 
 ******************************************************************************/

#include "lvgl.h"


#ifndef FA_CUSTOM
#define FA_CUSTOM 1
#endif

#if FA_CUSTOM

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+F0C9 "" */
    0xcd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0x2f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf3, 0x12, 0x22, 0x22,
    0x22, 0x22, 0x22, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xde,
    0xee, 0xee, 0xee, 0xee, 0xee, 0x20, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xd, 0xee, 0xee, 0xee, 0xee, 0xee,
    0xe2, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x30,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    /* U+F7BF "" */
    0x0, 0x0, 0x0, 0x13, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x2e, 0xf6, 0x0, 0x54, 0x0, 0x0,
    0x0, 0x2e, 0xb8, 0xf5, 0x7f, 0xf5, 0x0, 0x0,
    0x1e, 0xb0, 0x7, 0xff, 0xff, 0xf1, 0x0, 0x3,
    0xf8, 0x0, 0x7f, 0xff, 0xfa, 0x0, 0x0, 0x5,
    0xf8, 0x7f, 0xff, 0xfc, 0x0, 0x0, 0x13, 0x25,
    0xff, 0xff, 0xfd, 0xf6, 0x0, 0xbf, 0xff, 0xef,
    0xff, 0xfa, 0x7, 0xf6, 0x5, 0xff, 0xff, 0xff,
    0xfd, 0x0, 0xc, 0xe0, 0x5, 0xff, 0xff, 0xfc,
    0xf8, 0x8, 0xf5, 0x0, 0x5, 0xff, 0xff, 0xa5,
    0xfc, 0xf5, 0x0, 0x2, 0xac, 0xff, 0xfc, 0x5,
    0xe5, 0x0, 0x0, 0x6f, 0x15, 0xff, 0xc0, 0x0,
    0x0, 0x0, 0x0, 0x10, 0x5, 0xf9, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x5, 0x20, 0x0, 0x0,
    0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 196, .box_w = 13, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 78, .adv_w = 224, .box_w = 15, .box_h = 15, .ofs_x = -1, .ofs_y = -2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x6f6
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 61641, .range_length = 1783, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 2, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
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
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t fa_custom = {
#else
lv_font_t fa_custom = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -5,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if FA_CUSTOM*/

