#include "wol.h"
#include "wol_utils.h"

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
