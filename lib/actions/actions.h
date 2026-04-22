#pragma once

#include <Arduino.h>

constexpr size_t ACTION_MAX_RETRIES = 3;
constexpr size_t ACTION_RETRY_DELAY = 60000;

void retry(void (*fn)(), bool (*shouldStop)());
