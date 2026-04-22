#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

constexpr size_t WOL_MAC_ADDRESS_LENGTH = 6;
constexpr size_t WOL_PACKET_LENGTH = 102;
constexpr size_t WOL_PACKET_REPEAT_COUNT = 16;

void wakeOnLan(void);
