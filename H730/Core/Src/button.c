#include "button.h"
#include "gpio.h"

#define BUTTON_0_PORT GPIOB
#define BUTTON_0_PIN GPIO_PIN_11
#define BUTTON_1_PORT GPIOE
#define BUTTON_1_PIN GPIO_PIN_15
#define BUTTON_2_PORT GPIOE
#define BUTTON_2_PIN GPIO_PIN_13
#define BUTTON_3_PORT GPIOE
#define BUTTON_3_PIN GPIO_PIN_10
#define BUTTON_4_PORT GPIOE
#define BUTTON_4_PIN GPIO_PIN_9

static button_t last_btn = BUTTON_NONE;

button_t poll_button() {
    switch (last_btn) {
    case BUTTON_NONE:
        if (HAL_GPIO_ReadPin(BUTTON_0_PORT, BUTTON_0_PIN)) {
            last_btn = BUTTON_0;
        } else if (HAL_GPIO_ReadPin(BUTTON_1_PORT, BUTTON_1_PIN)) {
            last_btn = BUTTON_1;
        } else if (HAL_GPIO_ReadPin(BUTTON_2_PORT, BUTTON_2_PIN)) {
            last_btn = BUTTON_2;
        } else if (HAL_GPIO_ReadPin(BUTTON_3_PORT, BUTTON_3_PIN)) {
            last_btn = BUTTON_3;
        } else if (HAL_GPIO_ReadPin(BUTTON_4_PORT, BUTTON_4_PIN)) {
            last_btn = BUTTON_4;
        }
        break;
    case BUTTON_0:
        if (!HAL_GPIO_ReadPin(BUTTON_0_PORT, BUTTON_0_PIN)) {
            last_btn = BUTTON_NONE;
            return BUTTON_0;
        }
        break;
    case BUTTON_1:
        if (!HAL_GPIO_ReadPin(BUTTON_1_PORT, BUTTON_1_PIN)) {
            last_btn = BUTTON_NONE;
            return BUTTON_1;
        }
        break;
    case BUTTON_2:
        if (!HAL_GPIO_ReadPin(BUTTON_2_PORT, BUTTON_2_PIN)) {
            last_btn = BUTTON_NONE;
            return BUTTON_2;
        }
        break;
    case BUTTON_3:
        if (!HAL_GPIO_ReadPin(BUTTON_3_PORT, BUTTON_3_PIN)) {
            last_btn = BUTTON_NONE;
            return BUTTON_3;
        }
        break;
    case BUTTON_4:
        if (!HAL_GPIO_ReadPin(BUTTON_4_PORT, BUTTON_4_PIN)) {
            last_btn = BUTTON_NONE;
            return BUTTON_4;
        }
        break;
    }
    return BUTTON_NONE;
}