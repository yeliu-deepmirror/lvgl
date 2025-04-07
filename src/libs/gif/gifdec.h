#ifndef GIFDEC_H
#define GIFDEC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../misc/lv_fs.h"

#if LV_USE_GIF
#include <stdint.h>

typedef struct _gd_Palette {
    int size;
    uint8_t colors[0x100 * 3];
} gd_Palette;

typedef struct _gd_GCE {
    uint16_t delay;
    uint8_t tindex;
    uint8_t disposal;
    int input;
    int transparency;
} gd_GCE;

typedef struct _lv_sdmmc_drv_t {
    bool (*open_cb)(const char * path, lv_fs_mode_t mode);
    lv_fs_res_t (*close_cb)();
    lv_fs_res_t (*read_cb)(void * buf, uint32_t btr);
    lv_fs_res_t (*write_cb)(const void * buf, uint32_t btw, uint32_t * bw);
    lv_fs_res_t (*seek_cb)(uint32_t pos, lv_fs_whence_t whence);
    uint32_t (*tell_cb)();
} lv_sdmmc_drv_t;

typedef struct _gd_GIF {
    lv_sdmmc_drv_t* fd_sdmmc;
    lv_fs_file_t fd;
    const char * data;
    uint8_t is_file;
    uint32_t f_rw_p;
    int32_t anim_start;
    uint16_t width, height;
    uint16_t depth;
    int32_t loop_count;
    gd_GCE gce;
    gd_Palette *palette;
    gd_Palette lct, gct;
    void (*plain_text)(
        struct _gd_GIF *gif, uint16_t tx, uint16_t ty,
        uint16_t tw, uint16_t th, uint8_t cw, uint8_t ch,
        uint8_t fg, uint8_t bg
    );
    void (*comment)(struct _gd_GIF *gif);
    void (*application)(struct _gd_GIF *gif, char id[8], char auth[3]);
    uint16_t fx, fy, fw, fh;
    uint8_t bgindex;
    uint8_t * canvas, * frame;
    #if LV_GIF_CACHE_DECODE_DATA
    uint8_t *lzw_cache;
    #endif
} gd_GIF;

gd_GIF * gd_open_gif_file(const char *fname);
gd_GIF * gd_open_gif_file_sdmmc(const char *fname, lv_sdmmc_drv_t* fd_sdmmc);

gd_GIF * gd_open_gif_data(const void *data);

void gd_render_frame(gd_GIF *gif, uint8_t *buffer);

int gd_get_frame(gd_GIF * gif);
void gd_rewind(gd_GIF * gif);
void gd_close_gif(gd_GIF * gif);

#endif /*LV_USE_GIF*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GIFDEC_H */
