// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stddef.h>

#include "BaconEngine/Server/Packets.h"
#include "BaconEngine/Server/Packet.h"
#include "../InterfaceFunctions.h"

#ifndef BE_CLIENT_BINARY
#   include "PrivatePacket.h"
#endif

void BE_Packets_SendPing(BE_Client client) {
#ifndef BE_CLIENT_BINARY
    BE_Packet_Send(client, BE_PRIVATEPACKET_OPERATION_CODE_PING, NULL);
#else
    BE_INTERFACEFUNCTION(void, BE_Client)(client);
#endif
}

void BE_Packets_SendConnect(void) {
#ifndef BE_CLIENT_BINARY
    BE_Packet_Send(BE_CLIENT_UNCONNECTED, BE_PRIVATEPACKET_OPERATION_CODE_CONNECT, NULL);
#else
    BE_INTERFACEFUNCTION(void, void)();
#endif
}
