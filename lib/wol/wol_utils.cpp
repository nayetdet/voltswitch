#include "wol_utils.h"

bool buildMACAddress(uint8_t macAddress[WOL_MAC_ADDRESS_LENGTH]) {
    return sscanf(
        WOL_MAC_ADDRESS,
        "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
        &macAddress[0],
        &macAddress[1],
        &macAddress[2],
        &macAddress[3],
        &macAddress[4],
        &macAddress[5]
    ) == WOL_MAC_ADDRESS_LENGTH;
}

void buildPacket(uint8_t packet[WOL_PACKET_LENGTH], uint8_t macAddress[WOL_MAC_ADDRESS_LENGTH]) {
    memset(packet, 0xFF, WOL_MAC_ADDRESS_LENGTH);
    for (size_t i = 0; i < WOL_PACKET_REPEAT_COUNT; i++) {
        memcpy(
            &packet[(i + 1) * WOL_MAC_ADDRESS_LENGTH],
            macAddress,
            WOL_MAC_ADDRESS_LENGTH
        );
    }
}

void sendPacket(WiFiUDP udp, uint8_t packet[WOL_PACKET_LENGTH]) {
    const uint16_t port = static_cast<uint16_t>(strtol(WOL_PORT, nullptr, 10));
    udp.begin(port);
    udp.beginPacket(WOL_BROADCAST_IP, port);
    udp.write(packet, sizeof(packet));
    udp.endPacket();
    Serial.println("Wake-on-LAN packet sent!");
}
