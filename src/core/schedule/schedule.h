#pragma once

#include <Arduino.h>

constexpr size_t SCHEDULE_HOURS_MAX_COUNT = 24;

void setupScheduleHours(void);
bool isWithinSchedule(struct tm timeinfo);
long secondsUntilNextSchedule(struct tm timeinfo);
