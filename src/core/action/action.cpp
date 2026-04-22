#include "action.h"

void retry(void (*fn)(), bool (*shouldStop)(), unsigned long earlyDelay, unsigned long lateDelay) {
    for (size_t i = 0; i < ACTION_MAX_RETRIES; i++) {
        Serial.printf("Action attempt %u/%u\n", i + 1, ACTION_MAX_RETRIES);
        fn();
        Serial.printf("Waiting %lu ms before checking state\n", earlyDelay);
        delay(earlyDelay);

        if (shouldStop()) {
            Serial.printf("Desired state reached on attempt %u\n", i + 1);
            break;
        }

        if (i < ACTION_MAX_RETRIES - 1) {
            Serial.printf("State not reached yet, retrying in %lu ms\n", lateDelay);
            delay(lateDelay);
        } else {
            Serial.println("Max retries reached, giving up");
        }
    }
}
