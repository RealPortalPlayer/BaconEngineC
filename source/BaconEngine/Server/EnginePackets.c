// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stddef.h>

#include "EnginePackets.h"
#include "BaconEngine/Server/Packet.h"
#include "PrivateServer.h"
#include "PrivatePacket.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void BE_EnginePackets_Ping(BE_Client client, char data[BE_PACKET_MAXIMUM_DATA]);
void BE_EnginePackets_Connect(BE_Client client, char data[BE_PACKET_MAXIMUM_DATA]);
void BE_EnginePackets_PingAcknowledged(BE_Client client, char data[BE_PACKET_MAXIMUM_DATA]);
void BE_EnginePackets_Disconnect(BE_Client client, char data[BE_PACKET_MAXIMUM_DATA]);

void BE_EnginePackets_Initialize(void) {
#ifndef BE_DISABLE_NETWORK
    BE_Packet_Register(BE_PRIVATEPACKET_OPERATION_CODE_PING, BE_PACKET_FLAG_ALLOW_CLIENT_TO_SERVER | BE_PACKET_FLAG_ALLOW_UNCONNECTED, &BE_EnginePackets_Ping);
    BE_Packet_Register(BE_PRIVATEPACKET_OPERATION_CODE_CONNECT, BE_PACKET_FLAG_ALLOW_CLIENT_TO_SERVER | BE_PACKET_FLAG_ALLOW_UNCONNECTED, &BE_EnginePackets_Connect);
    BE_Packet_Register(BE_PRIVATEPACKET_OPERATION_CODE_PING_ACKNOWLEDGED, BE_PACKET_FLAG_ALLOW_SERVER_TO_CLIENT | BE_PACKET_FLAG_ALLOW_UNCONNECTED, &BE_EnginePackets_PingAcknowledged);
    BE_Packet_Register(BE_PRIVATEPACKET_OPERATION_CODE_DISCONNECT, BE_PACKET_FLAG_ALLOW_CLIENT_TO_SERVER, &BE_EnginePackets_Disconnect);
#endif
}

#ifndef BE_DISABLE_NETWORK
void BE_EnginePackets_Ping(BE_Client client, char data[BE_PACKET_MAXIMUM_DATA]) {
    BE_Packet_Send(client, BE_PRIVATEPACKET_OPERATION_CODE_PING_ACKNOWLEDGED, NULL);
}

void BE_EnginePackets_Connect(BE_Client client, char data[BE_PACKET_MAXIMUM_DATA]) {
    if (client != BE_CLIENT_UNCONNECTED) {
        // BE_Packet_Send(client, "error already_connected");
        return;
    }
    
    BE_PrivateServer_AddConnection(BE_PrivateServer_GetPrivateClientFromClient(client)->socket);
}

void BE_EnginePackets_PingAcknowledged(BE_Client client, char data[BE_PACKET_MAXIMUM_DATA]) {
    // TODO: Reset disconnect watchdog
}

void BE_EnginePackets_Disconnect(BE_Client client, char data[BE_PACKET_MAXIMUM_DATA]) {
    BE_PrivateServer_RemoveConnection(BE_PrivateServer_GetPrivateClientFromClient(client)->socket);
}
#endif
BA_CPLUSPLUS_SUPPORT_GUARD_END()
