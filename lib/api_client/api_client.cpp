#include "api_client.h"

static int request(const char *const url, const char *method);

bool healthy(void) {
    int code = request(API_URL, "GET");
    return code >= 200 && code < 300;
}

bool shutdown(void) {
    int code = request(API_URL "/shutdown", "POST");
    return code >= 200 && code < 300;
}

static int request(const char *const url, const char *const method) {
    HTTPClient http;
    http.begin(url);
    http.setTimeout(API_TIMEOUT_MS);

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
