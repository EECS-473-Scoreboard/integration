#pragma once

typedef enum : char {
    BUTTON_NONE,
    BUTTON_0,
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4
} button_t;

button_t poll_button();