#pragma once

#include "dma.h"
#include "sai.h"
#include "gpio.h"

#define nsamples 4000

extern int16_t high_freq[nsamples];
extern int16_t low_freq[nsamples];

void load_waves();
void play_wave(int16_t* input);