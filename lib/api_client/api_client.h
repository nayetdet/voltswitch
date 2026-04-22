#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

constexpr size_t API_TIMEOUT_MS = 5000;
constexpr size_t API_URL_MAX_LENGTH = 128;

bool healthy(void);
bool shutdown(void);
