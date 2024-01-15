// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stddef.h>

#include "EnginePackets.h"
#include "BaconEngine/Server/Packet.h"
#include "PrivateServer.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void BE_EnginePackets_Ping(BE_Client client, struct sockaddr_in* descriptor);
void BE_EnginePackets_Connect(BE_Client client, struct sockaddr_in* descriptor);

void BE_EnginePackets_Initialize(void) {
    BE_Packet_Register("ping", BA_BOOLEAN_TRUE, &BE_EnginePackets_Ping);
    BE_Packet_Register("connect", BA_BOOLEAN_TRUE, &BE_EnginePackets_Connect);
}

void BE_EnginePackets_Ping(BE_Client client, struct sockaddr_in* descriptor) {
    BE_Packet_Send(descriptor, "pong");
}

void BE_EnginePackets_Connect(BE_Client client, struct sockaddr_in* descriptor) {
    if (client != BE_CLIENT_UNCONNECTED) {
        BE_Packet_Send(descriptor, "error already_connected");
        return;
    }
    
    BE_PrivateServer_AddConnection(descriptor);
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
