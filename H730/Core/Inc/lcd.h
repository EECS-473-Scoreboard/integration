#ifndef LCD_H
#define LCD_H

#include "adc.h"
#include "ltdc.h"

#include "../../lvgl/lvgl.h"

#define LCD_RENDER_WIDTH 800
#define LCD_RENDER_HEIGHT 400

// take the median of this number of adc readings as the final output
#define ADC_MEDIAN_SIZE 21
#define ADC_DMA_BUF_SIZE (ADC_MEDIAN_SIZE * 2)

#define LCD_EN_PORT GPIOE
#define LCD_EN_PIN GPIO_PIN_1
#define LCD_X_R_PORT GPIOA
#define LCD_X_R_PIN GPIO_PIN_6
#define LCD_X_L_PORT GPIOD
#define LCD_X_L_PIN GPIO_PIN_0
#define LCD_Y_D_PORT GPIOA
#define LCD_Y_D_PIN GPIO_PIN_7
#define LCD_Y_U_PORT GPIOD
#define LCD_Y_U_PIN GPIO_PIN_1
#define LCD_ADC_PORT ADC1
#define LCD_X_R_ADC_RANK 0
#define LCD_Y_D_ADC_RANK 1

extern uint8_t fb[LCD_RENDER_WIDTH * LCD_RENDER_HEIGHT];

void init_display();
void flush_cb(lv_disp_drv_t *, const lv_area_t *, lv_color_t *);
void clean_dcache_cb(lv_disp_drv_t *);

void test_display();
void show_calib();

void init_touch();
void read_touch(lv_indev_drv_t *, lv_indev_data_t *);

#endif