#include <api_client.h>
#include <schedule.h>
#include <wifi.h>
#include <wol.h>

int hours[] = {7, 8, 11, 12, 15, 16, 19, 20};
int hoursCount = sizeof(hours) / sizeof(int);

void setup(void) {
    Serial.begin(115200);
    configTzTime(TZ, "pool.ntp.org");
}

void loop(void) {
    if (WiFi.status() != WL_CONNECTED) {
        setupWiFi();
    }

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return;
    }

    bool shouldBeOnline = isWithinSchedule(hours, hoursCount, timeinfo);
    bool isOnline = healthy();
    if (shouldBeOnline && !isOnline) {
        Serial.printf(
            "Wake-on-LAN at %02d:%02d:%02d\n",
            timeinfo.tm_hour,
            timeinfo.tm_min,
            timeinfo.tm_sec
        );

        wakeOnLan();
    } else if (!shouldBeOnline && isOnline) {
        Serial.printf(
            "Shutdown at %02d:%02d:%02d\n",
            timeinfo.tm_hour,
            timeinfo.tm_min,
            timeinfo.tm_sec
        );

        shutdown();
    }

    long sleepTime = secondsUntilNextSchedule(hours, hoursCount, timeinfo);
    Serial.printf("Sleeping for %ld seconds\n", sleepTime);
    esp_sleep_enable_timer_wakeup(sleepTime * 1000000ULL);
    esp_deep_sleep_start();
}
