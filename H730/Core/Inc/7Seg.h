#ifndef __SEGSW_H
#define __SEGSW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
	
typedef enum { SEVEN_SEG_FREE, SEVEN_SEG_DISPLAY, SEVEN_SEG_BLINK } seven_seg_job_t ;
typedef enum { SEVEN_SEG_FINISHED, SEVEN_SEG_PENDING, SEVEN_SEG_ERROR } seven_seg_result_t;

/* TODO: Make better function names */
seven_seg_result_t seven_seg_pollMainFunction(uint32_t time);

void seven_seg_init();

uint8_t get_bitmap(uint8_t i);
seven_seg_result_t start_display_numbers(unsigned char p1num, unsigned char p2num);

seven_seg_result_t start_display_custom(uint8_t p1_tens,uint8_t p1_ones, uint8_t p2_tens, uint8_t p2_ones);

seven_seg_result_t start_display_saved();

seven_seg_result_t change_digit(uint8_t i, uint8_t bitmap);

/* duration in ms */
seven_seg_result_t start_blink(uint32_t time, uint32_t duration);


#ifdef __cpluplus
}
#endif

#endif
/* Interface with 7Seg displays */

