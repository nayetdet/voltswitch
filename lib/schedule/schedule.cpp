#include "schedule.h"

bool isWithinSchedule(const int *const hours, size_t hoursCount, struct tm timeinfo) {
    for (size_t i = 0; i < hoursCount; i++) {
        if (timeinfo.tm_hour == hours[i]) {
            return true;
        }
    }

    return false;
}

long secondsUntilNextSchedule(const int *const hours, size_t hoursCount, struct tm timeinfo) {
    const int BEST_DIFF = 24 * 60 * 60; // 1 day
    int currentDiff = BEST_DIFF;
    int currentSeconds = timeinfo.tm_hour * 60 * 60 + timeinfo.tm_min * 60 + timeinfo.tm_sec;
    for (size_t i = 0; i < hoursCount; i++) {
        int targetHour = hours[i];
        if (targetHour < 0 || targetHour >= 24) {
            continue;
        }

        int targetSeconds = targetHour * 60 * 60;
        int diff = targetSeconds - currentSeconds;
        if (diff <= 0) {
            diff += BEST_DIFF; // adds 1 day
        }

        if (diff < currentDiff) {
            currentDiff = diff;
        }
    }

    if (currentDiff <= 0) {
        currentDiff = 1;
    }

    return currentDiff;
}
