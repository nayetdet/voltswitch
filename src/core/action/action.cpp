#include "action.h"

void retry(void (*fn)(), bool (*shouldStop)()) {
    for (size_t i = 0; i < ACTION_MAX_RETRIES; i++) {
        Serial.printf("Action attempt %u/%u\n", i + 1, ACTION_MAX_RETRIES);
        fn();

        if (shouldStop()) {
            Serial.println("Desired state reached, stopping attempts");
            break;
        }

        if (i < ACTION_MAX_RETRIES - 1) {
            Serial.printf("State not reached yet, retrying in %u ms\n", ACTION_RETRY_DELAY);
            delay(ACTION_RETRY_DELAY);
        }
    }

    Serial.println("Action attempts finished");
}
