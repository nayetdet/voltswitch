#include "api_client.h"

bool healthy(void) {
    int code = request(API_URL, "GET");
    return code >= 200 && code < 300;
}

bool unhealthy(void) {
    return !healthy();
}

bool shutdown(void) {
    int code = request(API_URL "/shutdown", "POST");
    return code >= 200 && code < 300;
}
