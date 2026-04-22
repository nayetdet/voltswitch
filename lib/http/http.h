#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

constexpr size_t HTTP_TIMEOUT_MS = 5000;

int request(const char *const url, const char *const method);
