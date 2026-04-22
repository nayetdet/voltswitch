#include "schedule.h"

static int scheduleHours[SCHEDULE_HOURS_MAX_COUNT];
static size_t scheduleHoursCount = 0;

void setupScheduleHours(void) {
    scheduleHoursCount = 0;
    char buffer[] = HOURS;
    for (char *token = strtok(buffer, ","); token != nullptr; token = strtok(nullptr, ",")) {
        while (*token == ' ' || *token == '\t') {
            token++;
        }

        char *tokenEnd = nullptr;
        long tokenValue = strtol(token, &tokenEnd, 10);
        while (*tokenEnd == ' ' || *tokenEnd == '\t') {
            tokenEnd++;
        }

        if (tokenEnd == token || *tokenEnd != '\0' || tokenValue < 0 || tokenValue >= 24) {
            Serial.printf("Invalid schedule hour ignored: %s\n", token);
        } else if (scheduleHoursCount >= SCHEDULE_HOURS_MAX_COUNT) {
            Serial.printf("Schedule hour ignored, too many values: %ld\n", tokenValue);
        } else {
            scheduleHours[scheduleHoursCount++] = static_cast<int>(tokenValue);
        }
    }

    if (scheduleHoursCount == 0) {
        Serial.println("No valid schedule hours configured, restarting...");
        ESP.restart();
    }

    Serial.printf("Loaded %u schedule hours\n", static_cast<unsigned int>(scheduleHoursCount));
}

bool isWithinSchedule(struct tm timeinfo) {
    for (size_t i = 0; i < scheduleHoursCount; i++) {
        if (timeinfo.tm_hour == scheduleHours[i]) {
            return true;
        }
    }

    return false;
}

long secondsUntilNextSchedule(struct tm timeinfo) {
    const int MAX_WAIT_SECONDS = 24 * 60 * 60;
    int currentDiff = MAX_WAIT_SECONDS;
    int currentSeconds = timeinfo.tm_hour * 3600 + timeinfo.tm_min * 60 + timeinfo.tm_sec;
    if (isWithinSchedule(timeinfo)) {
        return 3600 - timeinfo.tm_min * 60 - timeinfo.tm_sec;
    }

    for (size_t i = 0; i < scheduleHoursCount; i++) {
        int targetHour = scheduleHours[i];
        if (targetHour < 0 || targetHour >= 24) {
            continue;
        }

        int targetSeconds = targetHour * 60 * 60;
        int diff = targetSeconds - currentSeconds;
        if (diff <= 0) {
            diff += MAX_WAIT_SECONDS;
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
