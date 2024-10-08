// Purpose: Packet information
// Created on: 1/8/24 @ 4:11 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "../AntiClientGuard.h"
#include "PrivateClient.h"
#include "BaconEngine/Server/Packet.h"

#define BE_PRIVATEPACKET_MAGIC "PAK"
#define BE_PRIVATEPACKET_MAGIC_LENGTH (sizeof(BE_PRIVATEPACKET_MAGIC) - 1)

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_PACKET_PACKED(struct BE_PrivatePacket_Sent {
    char magic[BE_PRIVATEPACKET_MAGIC_LENGTH];
    uint64_t operationCode;
    char data[BE_PACKET_MAXIMUM_DATA];

    /**
     * Always zero
     */
    unsigned char terminator : 1;
    // TODO: Perhaps we can make data infinitely big by sending them in chunks?
});

typedef struct BE_PrivatePacket_Sent BE_PrivatePacket_Sent;

typedef struct {
    uint64_t operationCode;
    BE_Packet_Flags flags;
    BE_Packet_Run Run;
} BE_PrivatePacket_Registered;

void BE_PrivatePacket_Initialize(void);
void BE_PrivatePacket_Parse(BE_PrivateClient* client, struct sockaddr_in* descriptor, BE_PrivatePacket_Sent packet);
void BE_PrivatePacket_Destroy(void);
void BE_PrivatePacket_Send(struct sockaddr_in* socket, uint64_t operationCode, char data[BE_PACKET_MAXIMUM_DATA]);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#define BE_PRIVATEPACKET_STARTING_OPERATION_CODE 1000ul
#define BE_PRIVATEPACKET_OPERATION_CODE_PING BE_PRIVATEPACKET_STARTING_OPERATION_CODE
#define BE_PRIVATEPACKET_OPERATION_CODE_CONNECT (BE_PRIVATEPACKET_STARTING_OPERATION_CODE + 1ul)
#define BE_PRIVATEPACKET_OPERATION_CODE_PING_ACKNOWLEDGED (BE_PRIVATEPACKET_STARTING_OPERATION_CODE + 2ul)
#define BE_PRIVATEPACKET_OPERATION_CODE_DISCONNECT (BE_PRIVATEPACKET_STARTING_OPERATION_CODE + 3ul)
