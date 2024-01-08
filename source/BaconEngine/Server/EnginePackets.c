// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "EnginePackets.h"
#include "BaconEngine/Server/Packet.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void BE_EnginePackets_Ping(BE_Client_Connected* client, struct sockaddr_in* descriptor) {
    BE_Packet_Send(descriptor, "pong");
}

void BE_EnginePackets_Initialize(void) {
    BE_Packet_Register("ping", BA_BOOLEAN_TRUE, &BE_EnginePackets_Ping);
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
