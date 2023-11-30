#include "7Seg.h"

#include <stdbool.h>

#define DELAY                                                                  \
    {                                                                          \
        for (volatile int i = 0; i < 20; i++)                                  \
            ;                                                                  \
    }

#define BLINK_INTERVAL 500

// all pins are GPIOD
#define SCLK_PIN GPIO_PIN_3
#define SCLK_PORT GPIOD

#define RCLK_PIN GPIO_PIN_4
#define RCLK_PORT GPIOD

#define SDATA_PIN GPIO_PIN_4
#define SDATA_PORT GPIOB

#define CLR_PIN GPIO_PIN_7
#define CLR_PORT GPIOD

#define OUT_EN_PIN GPIO_PIN_5
#define OUT_EN_PORT GPIOD

#define SCOREBOARD_INDICATOR 0x00

static seven_seg_job_t cur_job = SEVEN_SEG_FREE;

static uint8_t blank = 0b00000000;

static uint32_t time_of_next_blink = 0;
static uint32_t time_of_blink_stop = 0;
static bool screen_blank = false;

static const unsigned char seg_bitmaps[10] = {
    0b01111011, 0b01010000, 0b10110011, 0b11110010, 0b11011000,
    0b11101010, 0b11101011, 0b01110000, 0b11111011, 0b11111010};

/* p1_tens, p1_ones, p2_tens, p2_ones, scoreboard_indicator */
static uint8_t digits[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

static void send_data(unsigned char data) {
    for (unsigned char pos = 0x80; pos != 0; pos >>= 1) {
        /*Shift out one bit of data*/
        HAL_GPIO_WritePin(SDATA_PORT, SDATA_PIN, data & pos);

        /*Toggle the serial clock*/
        HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_SET);
        DELAY;
        HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_RESET);
        DELAY;
    }

    /*Toggle the register clock*/
    HAL_GPIO_WritePin(RCLK_PORT, RCLK_PIN, GPIO_PIN_SET);
    DELAY;
    HAL_GPIO_WritePin(RCLK_PORT, RCLK_PIN, GPIO_PIN_RESET);
    DELAY;
}

static void output_enable() {
    /*Output Enable is Active low*/
    HAL_GPIO_WritePin(GPIOD, OUT_EN_PIN, GPIO_PIN_RESET);
}

static seven_seg_result_t ll_seven_seg_writedata() {
    for (int8_t i = 3; i >= 0; i--) {
        send_data(digits[i]);
    }
    send_data(digits[4]);
    return SEVEN_SEG_FINISHED;
}

static seven_seg_result_t ll_seven_seg_blink(int time) {
    if (time > time_of_blink_stop) {
        screen_blank = false;
        cur_job = SEVEN_SEG_FREE;
        return ll_seven_seg_writedata();
    } else if (time > time_of_next_blink) {
        if (screen_blank) {
            ll_seven_seg_writedata();
            screen_blank = false;
        } else {
            send_data(blank);
            send_data(blank);
            send_data(blank);
            send_data(blank);
            send_data(SCOREBOARD_INDICATOR);
            screen_blank = true;
        }
        time_of_next_blink = time + BLINK_INTERVAL;
    }
    return SEVEN_SEG_PENDING;
}

seven_seg_result_t seven_seg_pollMainFunction(uint32_t time) {
    seven_seg_result_t retval = SEVEN_SEG_ERROR;
    switch (cur_job) {
    case SEVEN_SEG_DISPLAY:
        retval = ll_seven_seg_writedata();
        cur_job = SEVEN_SEG_FREE;
        break;
    case SEVEN_SEG_BLINK:
        retval = ll_seven_seg_blink(time);
        break;
    case SEVEN_SEG_FREE:
        retval = SEVEN_SEG_FINISHED;
        break;
    }
    return retval;
}

void seven_seg_init() {
    /*set clocks low at Power_on*/
    HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RCLK_PORT, RCLK_PIN, GPIO_PIN_RESET);

    /*Clear Shift Register Contents at Power_on*/
    HAL_GPIO_WritePin(CLR_PORT, CLR_PIN, GPIO_PIN_RESET);
    DELAY;
    HAL_GPIO_WritePin(CLR_PORT, CLR_PIN, GPIO_PIN_SET);

    /*Enable Output at Power_on*/
    output_enable();
}

/* Return bitmap entry */
uint8_t get_bitmap(uint8_t i) { return seg_bitmaps[i]; }

/* Display numbers to 7-Seg displayed */
seven_seg_result_t start_display_numbers(unsigned char p1num,
                                         unsigned char p2num) {
    if (p1num < 100 && p2num < 100) {
        digits[0] = seg_bitmaps[p2num % 10];
        digits[1] = seg_bitmaps[p2num / 10];
        digits[2] = seg_bitmaps[p1num % 10];
        digits[3] = seg_bitmaps[p1num / 10];
        ll_seven_seg_writedata();
    } else {
        return SEVEN_SEG_ERROR;
    }

    return SEVEN_SEG_FINISHED;
}

seven_seg_result_t start_display_saved() { return ll_seven_seg_writedata(); }

seven_seg_result_t change_digit(uint8_t i, uint8_t bitmap) {
    digits[i] = bitmap;
    return SEVEN_SEG_FINISHED;
}

/* Send manually defined 7-Seg bitmaps */
seven_seg_result_t start_display_custom(uint8_t p1_tens, uint8_t p1_ones,
                                        uint8_t p2_tens, uint8_t p2_ones) {
    digits[0] = p1_tens;
    digits[1] = p1_ones;
    digits[2] = p2_tens;
    digits[3] = p2_ones;

    ll_seven_seg_writedata();
    return SEVEN_SEG_FINISHED;
}

seven_seg_result_t start_blink(uint32_t time, uint32_t duration) {
    time_of_blink_stop = time + duration;
    cur_job = SEVEN_SEG_BLINK;
    return SEVEN_SEG_PENDING;
}
