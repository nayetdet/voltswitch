#include "wifi.h"

void setupWiFi(void) {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println(".");
        delay(1000);
    }

    Serial.println("Wi-Fi connected!");
    Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
}
