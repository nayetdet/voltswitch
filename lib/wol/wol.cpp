#include "wol.h"

static bool buildMACAddress(uint8_t macAddress[WOL_MAC_ADDRESS_LENGTH]);
static void buildPacket(uint8_t packet[WOL_PACKET_LENGTH], uint8_t macAddress[WOL_MAC_ADDRESS_LENGTH]);
static void sendPacket(WiFiUDP& udp, const uint8_t packet[WOL_PACKET_LENGTH]);

WiFiUDP udp;

void wakeOnLan(void) {
    uint8_t macAddress[WOL_MAC_ADDRESS_LENGTH];
    if (!buildMACAddress(macAddress)) {
        Serial.println("Invalid MAC address format!");
        return;
    }

    uint8_t packet[WOL_PACKET_LENGTH];
    buildPacket(packet, macAddress);
    sendPacket(udp, packet);
}

static bool buildMACAddress(uint8_t macAddress[WOL_MAC_ADDRESS_LENGTH]) {
    char trailing = '\0';
    return sscanf(
        WOL_MAC_ADDRESS,
        "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx%c",
        &macAddress[0],
        &macAddress[1],
        &macAddress[2],
        &macAddress[3],
        &macAddress[4],
        &macAddress[5],
        &trailing
    ) == WOL_MAC_ADDRESS_LENGTH;
}

static void buildPacket(uint8_t packet[WOL_PACKET_LENGTH], uint8_t macAddress[WOL_MAC_ADDRESS_LENGTH]) {
    memset(packet, 0xFF, WOL_MAC_ADDRESS_LENGTH);
    for (size_t i = 0; i < WOL_PACKET_REPEAT_COUNT; i++) {
        memcpy(
            &packet[(i + 1) * WOL_MAC_ADDRESS_LENGTH],
            macAddress,
            WOL_MAC_ADDRESS_LENGTH
        );
    }
}

static void sendPacket(WiFiUDP& udp, const uint8_t packet[WOL_PACKET_LENGTH]) {
    const uint16_t port = static_cast<uint16_t>(strtol(WOL_PORT, nullptr, 10));
    udp.begin(port);
    udp.beginPacket(WOL_BROADCAST_IP, port);
    udp.write(packet, WOL_PACKET_LENGTH);
    udp.endPacket();
    Serial.println("Wake-on-LAN packet sent!");
}
