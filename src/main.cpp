#include "action.h"
#include "voltswitch_api_client.h"
#include "schedule.h"
#include "wifi.h"
#include "wol.h"

constexpr unsigned long WOL_CHECK_DELAY_MS = 45000;
constexpr unsigned long WOL_RETRY_DELAY_MS = 30000;
constexpr unsigned long SHUTDOWN_CHECK_DELAY_MS = 10000;
constexpr unsigned long SHUTDOWN_RETRY_DELAY_MS = 10000;

void setup(void) {
    Serial.begin(115200);
    configTzTime(TZ, "pool.ntp.org");
    setupScheduleHours();
}

void loop(void) {
    if (WiFi.status() != WL_CONNECTED) {
        setupWiFi();
    }

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return;
    }

    bool shouldBeOnline = isWithinSchedule(timeinfo);
    bool isOnline = healthy();
    if (shouldBeOnline && !isOnline) {
        Serial.printf(
            "Wake-on-LAN at %02d:%02d:%02d\n",
            timeinfo.tm_hour,
            timeinfo.tm_min,
            timeinfo.tm_sec
        );

        retry(wakeOnLan, healthy, WOL_CHECK_DELAY_MS, WOL_RETRY_DELAY_MS);
    } else if (!shouldBeOnline && isOnline) {
        Serial.printf(
            "Shutdown at %02d:%02d:%02d\n",
            timeinfo.tm_hour,
            timeinfo.tm_min,
            timeinfo.tm_sec
        );

        retry([]() { shutdown(); }, unhealthy, SHUTDOWN_CHECK_DELAY_MS, SHUTDOWN_RETRY_DELAY_MS);
    } else {
        Serial.printf(
            "No action at %02d:%02d:%02d (shouldBeOnline=%d, isOnline=%d)\n",
            timeinfo.tm_hour,
            timeinfo.tm_min,
            timeinfo.tm_sec,
            shouldBeOnline,
            isOnline
        );
    }

    long sleepTime = secondsUntilNextSchedule(timeinfo);
    Serial.printf("Sleeping for %ld seconds\n", sleepTime);
    esp_sleep_enable_timer_wakeup(sleepTime * 1000000ULL);
    esp_deep_sleep_start();
}
