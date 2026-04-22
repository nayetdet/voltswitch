#pragma once

#include "wol.h"

bool buildMACAddress(uint8_t macAddress[WOL_MAC_ADDRESS_LENGTH]);
void buildPacket(uint8_t packet[WOL_PACKET_LENGTH], uint8_t macAddress[WOL_MAC_ADDRESS_LENGTH]);
void sendPacket(WiFiUDP udp, uint8_t packet[WOL_PACKET_LENGTH]);
