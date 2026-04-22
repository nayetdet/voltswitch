#pragma once

#include <Arduino.h>

constexpr size_t ACTION_MAX_RETRIES = 3;

void retry(void (*fn)(), bool (*shouldStop)(), unsigned long earlyDelay, unsigned long lateDelay);
