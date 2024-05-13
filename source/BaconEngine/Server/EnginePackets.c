// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stddef.h>

#include "EnginePackets.h"
#include "BaconEngine/Server/Packet.h"
#include "PrivateServer.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void BE_EnginePackets_Ping(BE_Client client);
void BE_EnginePackets_Connect(BE_Client client);

void BE_EnginePackets_Initialize(void) {
#ifndef BE_DISABLE_NETWORK
    BE_Packet_Register("ping", BA_BOOLEAN_TRUE, &BE_EnginePackets_Ping);
    BE_Packet_Register("connect", BA_BOOLEAN_TRUE, &BE_EnginePackets_Connect);
#endif
}

#ifndef BE_DISABLE_NETWORK
void BE_EnginePackets_Ping(BE_Client client) {
    BE_Packet_Send(client, "pong");
}

void BE_EnginePackets_Connect(BE_Client client) {
    if (client != BE_CLIENT_UNCONNECTED) {
        BE_Packet_Send(client, "error already_connected");
        return;
    }
    
    BE_PrivateServer_AddConnection(BE_PrivateServer_GetPrivateClientFromClient(client)->socket);
}
#endif
BA_CPLUSPLUS_SUPPORT_GUARD_END()
