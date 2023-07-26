#ifndef __LVGL_PORT_DISPLAY_H
#define __LVGL_PORT_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "lvgl/lvgl.h"

/*********************
 *      DEFINES
 *********************/

#define MY_DISP_HOR_RES    1024
#define MY_DISP_VER_RES    600
#define LV_VER_RES_MAX	   68

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void
lvgl_display_init (void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* __LVGL_PORT_DISPLAY_H */
