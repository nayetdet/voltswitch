#include "http.h"

int request(const char *const url, const char *const method) {
    HTTPClient http;
    if (!http.begin(url)) {
        Serial.printf("[%s] Invalid URL: %s\n", method, url);
        return -1;
    }

    http.setTimeout(HTTP_TIMEOUT_MS);

    int code = -1;
    if (strcmp(method, "GET") == 0) {
        code = http.GET();
    } else if (strcmp(method, "POST") == 0) {
        code = http.POST("");
    }
    
    Serial.printf("[%s] HTTP code: %d\n", method, code);
    http.end();
    return code;
}
