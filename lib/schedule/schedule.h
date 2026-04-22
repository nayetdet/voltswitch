#pragma once

#include <Arduino.h>

bool isWithinSchedule(const int *const allowedHours, size_t size, struct tm timeinfo);
long secondsUntilNextSchedule(const int *const allowedHours, size_t size, struct tm timeinfo);
