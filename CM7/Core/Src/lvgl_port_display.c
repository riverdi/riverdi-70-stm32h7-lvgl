/*********************
 *      INCLUDES
 *********************/

#include "lvgl_port_display.h"
#include "main.h"
#include "ltdc.h"
#include "dma2d.h"

/*********************
 *      DEFINES
 *********************/

#define LVGL_BUFFER_1_ADDR_AT_SDRAM	(0xD0258000)
#define LVGL_BUFFER_2_ADDR_AT_SDRAM (0xD04B0000)

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void disp_flush (lv_disp_drv_t*, const lv_area_t*, lv_color_t*);
static void disp_clean_dcache (lv_disp_drv_t*);
static void disp_copy_data (void*, void*, uint32_t, uint32_t);

/**********************
 *  STATIC VARIABLES
 **********************/

static lv_disp_drv_t disp_drv;
static lv_disp_draw_buf_t disp_buf;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void
lvgl_display_init (void)
{
  /* display initialization */
  ; /* display is already initialized by cubemx-generated code */

  /* display buffer initialization */
  lv_disp_draw_buf_init (&disp_buf,
                         (void*)LVGL_BUFFER_1_ADDR_AT_SDRAM,
                         (void*)LVGL_BUFFER_2_ADDR_AT_SDRAM,
                         MY_DISP_HOR_RES * MY_DISP_VER_RES);

  /* register the display in LVGL */
  lv_disp_drv_init(&disp_drv);

  /* set the resolution of the display */
  disp_drv.hor_res = MY_DISP_HOR_RES;
  disp_drv.ver_res = MY_DISP_VER_RES;

  /* set callback for display driver */
  disp_drv.flush_cb = disp_flush;
  disp_drv.clean_dcache_cb = disp_clean_dcache;
  //disp_drv.full_refresh = 1;
  //disp_drv.direct_mode = 1;

  /* set a display buffer */
  disp_drv.draw_buf = &disp_buf;

  /* finally register the driver */
  lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void
disp_flush (lv_disp_drv_t   *drv,
            const lv_area_t *area,
            lv_color_t      *color_p)
{
  uint32_t address;

  /* return if the area is out the screen */
  if ((area->x2 < 0) || (area->y2 < 0) || \
      (area->x1 > MY_DISP_HOR_RES - 1) || \
      (area->y1 > MY_DISP_VER_RES - 1))
    {
      lv_disp_flush_ready(&disp_drv);
      return;
    }

  /* invalidate cache */
  SCB_CleanInvalidateDCache();
  SCB_InvalidateICache();

  /* calculate destination address */
  address = hltdc.LayerCfg[0].FBStartAdress + \
            (((MY_DISP_HOR_RES * area->y1) + area->x1) * 4);

  /* start dma */
  disp_copy_data ((void*) color_p, (void*) address,
                  lv_area_get_width(area), lv_area_get_height(area));
}

static void
disp_clean_dcache (lv_disp_drv_t *drv)
{
  SCB_CleanInvalidateDCache();
}

static void
disp_copy_data_complete (DMA2D_HandleTypeDef *hdma2d)
{
  lv_disp_flush_ready(&disp_drv);
}

static void
disp_copy_data (void      *pSrc,
                void      *pDst,
                uint32_t   xSize,
                uint32_t   ySize)
{
  /* configure DMA2D */
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;
  hdma2d.Init.RedBlueSwap = DMA2D_RB_REGULAR;
  hdma2d.Init.OutputOffset = MY_DISP_HOR_RES - xSize;
  hdma2d.XferCpltCallback = disp_copy_data_complete;
  hdma2d.Instance = DMA2D;

  /* DMA2D initialization & start*/
  if (HAL_DMA2D_Init(&hdma2d) == HAL_OK)
    HAL_DMA2D_Start_IT(&hdma2d, (uint32_t)pSrc, (uint32_t)pDst, xSize, ySize);
}
