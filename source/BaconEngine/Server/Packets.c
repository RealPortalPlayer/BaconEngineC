// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stddef.h>

#include "BaconEngine/Server/Packets.h"

#include "BaconEngine/Server/Packet.h"

void BE_Packets_SendPing(BE_Client client) {
    BE_Packet_Send(client, 1000, NULL);
}

void BE_Packets_SendConnect(void) {
    BE_Packet_Send(BE_CLIENT_UNCONNECTED, 1001, NULL);
}
