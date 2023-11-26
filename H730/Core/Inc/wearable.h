#pragma once

#include "common.h"

// setup the driver. DMA and UART hardware are already initialized by the time of call
void init_wearable();
// return NO_WEARABLE_EVENT, or one event if there is any.
// will be polled by main.
wearable_event_t poll_wearable();