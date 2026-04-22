#include <api_client.h>
#include <wifi.h>
#include <wol.h>

void setup(void) {
    Serial.begin(115200);
    setupWiFi();
    wakeOnLan();
}

void loop(void) {
}
