#ifndef __LVGL_PORT_TOUCH_H
#define __LVGL_PORT_TOUCH_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "lvgl/lvgl.h"

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void
lvgl_touchscreen_init (void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* __LVGL_PORT_TOUCH_H */
