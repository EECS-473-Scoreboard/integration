#include "lcd.h"
#include "common.h"
#include "main_menu.h"
#include "wearable.h"

#include "stm32h7xx_hal_def.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// framebuffer stored in 320K SRAM1
__attribute__((section(".ram_d1"), aligned(8)))
uint8_t fb[LCD_RENDER_WIDTH * LCD_RENDER_HEIGHT];
// color look-up table
static uint32_t CLUT[256];

static lv_disp_drv_t lvgl_disp_drv;
// draw buffer for lvgl
static lv_disp_draw_buf_t lvgl_buf;
static lv_color_t draw_buf[LCD_RENDER_WIDTH * LCD_RENDER_HEIGHT / 10];

static void read_wearable(lv_indev_drv_t *_, lv_indev_data_t *data) {
    static bool dummy_read = false;

    if (dummy_read) {
        // manually send a release after one event
        data->state = LV_INDEV_STATE_RELEASED;
        data->continue_reading = false;
        dummy_read = false;
    } else {
        wearable_event_t event = poll_wearable();
        if (event.bits != NO_WEARABLE_EVENT) {
            // a new event
            data->state = LV_INDEV_STATE_PRESSED;
            data->key = event.bits;
            // force one more following poll
            data->continue_reading = true;
            dummy_read = true;
        }
    }
}

static void broadcast_wearable_event(struct _lv_indev_drv_t *_, uint8_t code) {
    if (code != LV_EVENT_KEY)
        return;
    wearable_event_t event = {.bits = lv_indev_get_key(lv_indev_get_act())};

    lv_event_send(main_menu, SC_EVENT_WEARABLE, event.word);
}

void init_display() {
    // point LTDC to framebuffer
    HAL_LTDC_SetAddress(&hltdc, (uint32_t)fb, LTDC_LAYER_1);

    // create a CLUT so that every rgb332 color maps to its rgb888 value
    for (uint8_t rgb332 = 0; rgb332 != 255; rgb332++) {
        uint8_t r = (uint32_t)(((rgb332 & 0xE0) >> 5) * 255 / 7);
        uint8_t g = (uint32_t)(((rgb332 & 0x1C) >> 2) * 255 / 7);
        uint8_t b = (uint32_t)((rgb332 & 0x03) * 255 / 3);
        // LTDC_LxCLUTWR[31:0]: CLUTADD[7:0] RED[7:0] GREEN[7:0] BLUE[7:0]
        // Note on endianess: will be writing to register, not memory.
        CLUT[rgb332] = (rgb332 << 24) + (r << 16) + (g << 8) + b;
    }
    // point LTDC to CLUT
    HAL_LTDC_ConfigCLUT(&hltdc, CLUT, 256, LTDC_LAYER_1);
    // load CLUT. Each word in the array will be written to the CLUTWR register
    // of layer 1. Such write happens for 256 times.
    HAL_LTDC_EnableCLUT(&hltdc, LTDC_LAYER_1);

    HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_SET);

    lv_init();
    lv_disp_draw_buf_init(&lvgl_buf, draw_buf, NULL,
                          LCD_RENDER_WIDTH * LCD_RENDER_HEIGHT / 10);
    lv_disp_drv_init(&lvgl_disp_drv);
    lvgl_disp_drv.flush_cb = flush_cb;
    lvgl_disp_drv.draw_buf = &lvgl_buf;
    lvgl_disp_drv.hor_res = LCD_RENDER_WIDTH;
    lvgl_disp_drv.ver_res = LCD_RENDER_HEIGHT;
    lvgl_disp_drv.sw_rotate = 0;
    lvgl_disp_drv.rotated = LV_DISP_ROT_90;
    lv_disp_t *disp = lv_disp_drv_register(&lvgl_disp_drv);

    lv_theme_t *th = lv_theme_default_init(
        disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
        LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, th);

    static lv_indev_drv_t wearable_indev;
    lv_indev_drv_init(&wearable_indev);
    wearable_indev.type = LV_INDEV_TYPE_KEYPAD;
    wearable_indev.read_cb = read_wearable;
    SC_EVENT_WEARABLE = lv_event_register_id();
    wearable_indev.feedback_cb = broadcast_wearable_event;
}

void flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    // LVGL provides a top-left to bottom-right region to draw
    // we draw it in framebuffer from bottom-left to top-right
    // so that the landscape screen is used vertically
    int32_t x_mem, y_mem;
    for (x_mem = area->y1; x_mem <= area->y2; x_mem++) {
        for (y_mem = area->x2; y_mem >= area->x1; y_mem--) {
            fb[y_mem * LCD_RENDER_WIDTH + x_mem] = *(uint8_t *)color_p;
            color_p++;
        }
    }

    lv_disp_flush_ready(disp);
}

void test_display() {
    for (int line = 0; line <= LCD_RENDER_HEIGHT; line++) {
        uint8_t color;
        if (line < LCD_RENDER_HEIGHT / 4) {
            // R
            color = 0b11100000;
        } else if (line < LCD_RENDER_HEIGHT / 2) {
            // G
            color = 0b00011100;
        } else if (line < LCD_RENDER_HEIGHT / 4 * 3) {
            // B
            color = 0b00000011;
        } else {
            // Miku 39c5bb is R: 2, G: 5, B: 2 = 0b01010110
            color = 0b01010110;
        }
        memset((void *)(fb + line * LCD_RENDER_WIDTH), color, LCD_RENDER_WIDTH);
    }
}

// display three points at:
// - 10% from left, 10% from top
// - 10% from right, 10% from top
// - 50% from left. 20% from bottom
// note that screen is 800x480 but display is 800x400 from (0,0)
void show_calib() {
    fb[48 * LCD_RENDER_WIDTH + LCD_RENDER_WIDTH / 10] = 0xFE;
    fb[48 * LCD_RENDER_WIDTH + LCD_RENDER_WIDTH / 10 * 9] = 0xFE;
    fb[384 * LCD_RENDER_WIDTH + LCD_RENDER_WIDTH / 2] = 0xFE;
}

inline static void to_ground(GPIO_TypeDef *port, uint16_t pin) {
    GPIO_InitTypeDef s = {0};
    s.Pin = pin;
    s.Mode = GPIO_MODE_OUTPUT_PP;
    s.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(port, &s);
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

inline static void to_power(GPIO_TypeDef *port, uint16_t pin) {
    GPIO_InitTypeDef s = {0};
    s.Pin = pin;
    s.Mode = GPIO_MODE_OUTPUT_PP;
    s.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(port, &s);
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}

inline static void to_floating(GPIO_TypeDef *port, uint16_t pin) {
    GPIO_InitTypeDef s = {0};
    s.Pin = pin;
    s.Mode = GPIO_MODE_INPUT;
    s.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(port, &s);
}

// MUX the GPIO pin to ADC
inline static void to_adc(GPIO_TypeDef *port, uint16_t pin) {
    GPIO_InitTypeDef s = {0};
    s.Pin = pin;
    s.Mode = GPIO_MODE_ANALOG;
    s.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(port, &s);
}

static int compare_u16(const void *a, const void *b) {
    uint16_t left = *(uint16_t *)a;
    uint16_t right = *(uint16_t *)b;
    return (left > right) - (left < right);
}

// size must be an odd number
// modifies provided array
static uint16_t median(uint16_t *begin, size_t size) {
    qsort(begin, size, 2, compare_u16);
    return begin[size / 2];
}

// DMA hardware cannot touch DTCM, but can touch SRAM1
__attribute__((
    section(".ram_d1"))) volatile uint16_t adc_dma_buffer[ADC_DMA_BUF_SIZE];
volatile int adc_ready;

// called when all ADC channels have finished conversion
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *_) { adc_ready = 1; }

static uint16_t *read_adc1(uint32_t rank) {
    adc_ready = 0;

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_dma_buffer, ADC_DMA_BUF_SIZE);
    while (!adc_ready)
        ;
    HAL_ADC_Stop_DMA(&hadc1);

    static uint16_t buffer[ADC_MEDIAN_SIZE];
    for (size_t i = 0; i < ADC_MEDIAN_SIZE; ++i) {
        buffer[i] = adc_dma_buffer[2 * i + rank];
    }
    return buffer;
}

static uint16_t read_x() {
    // XL: V+, XR: GND, YU: float, YD: ADC
    to_power(LCD_X_L_PORT, LCD_X_L_PIN);
    to_ground(LCD_X_R_PORT, LCD_X_R_PIN);
    to_floating(LCD_Y_U_PORT, LCD_Y_U_PIN);
    to_adc(LCD_Y_D_PORT, LCD_Y_D_PIN);

    return median(read_adc1(LCD_Y_D_ADC_RANK), ADC_MEDIAN_SIZE);
}

static uint16_t read_y() {
    // YU: V+, YD: GND, XL: float, XR: ADC
    to_power(LCD_Y_U_PORT, LCD_Y_U_PIN);
    to_ground(LCD_Y_D_PORT, LCD_Y_D_PIN);
    to_floating(LCD_X_L_PORT, LCD_X_L_PIN);
    to_adc(LCD_X_R_PORT, LCD_X_R_PIN);

    return median(read_adc1(LCD_X_R_ADC_RANK), ADC_MEDIAN_SIZE);
}

void init_touch() {}

int read_touch(lv_disp_drv_t *_, lv_indev_data_t *data) {
    // if no touch, x and y value should persist
    static uint16_t x = 0;
    static uint16_t y = 0;

    uint16_t raw_x = read_x();
    uint16_t raw_y = read_y();

    double newX = LCD_CALIB_A * raw_x + LCD_CALIB_B * raw_y + LCD_CALIB_C;
    double newY = LCD_CALIB_D * raw_x + LCD_CALIB_E * raw_y + LCD_CALIB_F;

    if (newX < 0 || newX > LCD_RENDER_WIDTH || newY < 0 ||
        newY > LCD_RENDER_HEIGHT) {
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_REL;
    } else {
        x = newX;
        y = newY;
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PR;
    }

    return 0;
}