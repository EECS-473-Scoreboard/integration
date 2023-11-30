#include "premade_waves.h"

int16_t high_freq[nsamples];

int16_t low_freq[nsamples];

void load_waves() {
    int i = 0;
    int alt = 0;
    double t = 0;
    double j = 0;
    while (i < nsamples) {
        if (i == 100) {
            alt = 1;
        }
        if (i == -100) {
            alt = 0;
        }
        if (alt) {
            t -= 0.1;
            j += 0.7;
        } else {
            t += 0.1;
            j -= 0.7;
        }
        low_freq[i] = t;
        low_freq[i + 1] = low_freq[i];
        high_freq[i] = j;
        high_freq[i + 1] = high_freq[i];
        i += 2;
    }
}

// play in loop for desired time
void play_wave(int16_t *input) {
    HAL_SAI_Transmit(&hsai_BlockA4, (uint8_t *)input, nsamples, HAL_MAX_DELAY);
}