// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/String.h>
#include <string.h>
#include <BaconAPI/Internal/Boolean.h>
#include <BaconAPI/Debugging/Assert.h>
#include <errno.h>
#include <BaconAPI/Math/Bitwise.h>
#include <SharedEngineCode/Debugging/StrictMode.h>

#include "BaconEngine/Server/Packet.h"
#include "../InterfaceFunctions.h"
#include "BaconEngine/Client/Information.h"

#ifndef BE_CLIENT_BINARY
#   include "PrivatePacket.h"
#   include "EnginePackets.h"
#   include "PrivateServer.h"
#   include "../Storage/PrivateDynamicArray.h"
#   include "../EngineMemory.h"
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#if !defined(BE_CLIENT_BINARY) && !defined(BE_DISABLE_NETWORK)
static BA_Boolean bePacketInitialized;
static BA_DynamicArray bePacketRegistered;

void BE_PrivatePacket_Initialize(void) {
    BA_ASSERT(!bePacketInitialized, "Already initialized the packet system\n");
    BA_LOGGER_INFO("Initializing the packet system\n");
    
    bePacketInitialized = BA_BOOLEAN_TRUE;

    BE_PrivateDynamicArray_Create(&bePacketRegistered, 100);
    BA_LOGGER_INFO("Registering engine packets\n");
    BE_EnginePackets_Initialize();
}
#endif

BE_BINARYEXPORT void BE_Packet_Register(uint64_t operationCode, BE_Packet_Flags flags, BE_Packet_Run Run) {
#ifndef BE_CLIENT_BINARY
#   ifndef BE_DISABLE_NETWORK
    BE_PrivatePacket_Registered* packet = BA_Memory_Allocate(sizeof(BE_PrivatePacket_Registered), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_PACKET);
    
    packet->operationCode = operationCode;
    packet->flags = flags;
    packet->Run = Run;

    BE_PrivateDynamicArray_CheckResize(&bePacketRegistered);
    BA_DynamicArray_AddElementToLast(&bePacketRegistered, packet);
#   endif
#else
    BE_INTERFACEFUNCTION(void, uint64_t, BE_Packet_Flags, BE_Packet_Run)(operationCode, flags, Run);
#endif
}

BE_BINARYEXPORT void BE_Packet_Send(BE_Client client, uint64_t operationCode, char data[BE_PACKET_MAXIMUM_DATA]) {
#ifndef BE_CLIENT_BINARY
#   ifndef BE_DISABLE_NETWORK
    if (client == BE_CLIENT_UNCONNECTED) {
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(!BE_ClientInformation_IsServerModeEnabled(), "Servers cannot send packets to self\n");
        BE_PrivatePacket_Send(BE_PrivateClient_GetSocketAddress(), operationCode, data);
        return;
    }

    BE_PrivatePacket_Send(BE_PrivateServer_GetPrivateClientFromClient(client)->socket, operationCode, data);
#   endif
#else
    BE_INTERFACEFUNCTION(void, BE_Client, uint64_t, char[BE_PACKET_MAXIMUM_DATA])(client, operationCode, data);
#endif
}

#if !defined(BE_CLIENT_BINARY) && !defined(BE_DISABLE_NETWORK)
void BE_PrivatePacket_Parse(BE_PrivateClient* client, struct sockaddr_in* descriptor, BE_PrivatePacket_Sent packet) {
    packet.terminator = '\0';

    BE_PrivatePacket_Registered* foundPacket = NULL;

    if (client->publicClient == BE_CLIENT_UNCONNECTED)
        client->socket = descriptor;

    if (strncmp(packet.magic, BE_PRIVATEPACKET_MAGIC, BE_PRIVATEPACKET_MAGIC_LENGTH) != 0) {
        BA_LOGGER_FATAL("Invalid packet: incorrect/missing magic\n");
        // TODO: Disconnect
        return;
    }
    
    for (int i = 0; i < bePacketRegistered.used; i++) {
        foundPacket = BA_DYNAMICARRAY_GET_ELEMENT(BE_PrivatePacket_Registered, bePacketRegistered, i);

        if (foundPacket->operationCode != packet.operationCode) {
            foundPacket = NULL;
            continue;
        }

        break;
    }

    if (foundPacket == NULL) {
        // TODO: Disconnect
        BA_LOGGER_FATAL("Invalid packet: invalid operation code (%lu)\n", packet.operationCode);
        return;
    }

    if (!BA_BITWISE_IS_BIT_SET(foundPacket->flags, BE_PACKET_FLAG_ALLOW_UNCONNECTED) && client->publicClient == BE_PACKET_FLAG_ALLOW_UNCONNECTED) {
        BA_LOGGER_ERROR("Packet doesn't allow unconnected\n");
        return;
    }

    if (!BA_BITWISE_IS_BIT_SET(foundPacket->flags, BE_PACKET_FLAG_ALLOW_SERVER_TO_CLIENT) && !BE_ClientInformation_IsServerModeEnabled()) {
        BA_LOGGER_ERROR("Packet is server only\n");
        return;
    }

    if (!BA_BITWISE_IS_BIT_SET(foundPacket->flags, BE_PACKET_FLAG_ALLOW_CLIENT_TO_SERVER) && BE_ClientInformation_IsServerModeEnabled()) {
        BA_LOGGER_ERROR("Packet is client only\n");
        return;
    }

    foundPacket->Run(client->publicClient, packet.data);
}

void BE_PrivatePacket_Destroy(void) {
    BA_ASSERT(bePacketInitialized, "The packet system is not initialized\n");
    BA_LOGGER_INFO("Destroying the packet system\n");
    
    bePacketInitialized = BA_BOOLEAN_FALSE;

    for (int i = 0; i < bePacketRegistered.used; i++)
        BA_Memory_Deallocate(bePacketRegistered.internalArray[i], sizeof(BE_PrivatePacket_Registered), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_PACKET);

    BA_Memory_Deallocate(bePacketRegistered.internalArray, sizeof(void*) * bePacketRegistered.size, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY);
}

void BE_PrivatePacket_Send(struct sockaddr_in* socket, uint64_t operationCode, char data[BE_PACKET_MAXIMUM_DATA]) {
    BE_PrivatePacket_Sent packet = {0};

    memcpy(&packet.magic, BE_PRIVATEPACKET_MAGIC, BE_PRIVATEPACKET_MAGIC_LENGTH);

    packet.operationCode = operationCode; // TODO: Endianess

    if (data != NULL)
        memcpy(&packet.data, data, sizeof(char) * BE_PACKET_MAXIMUM_DATA);
    
    if (sendto(BE_ClientInformation_IsServerModeEnabled() ? BE_PrivateServer_GetSocketDescriptor() : BE_PrivateClient_GetSocketDescriptor(), &packet, sizeof(BE_PrivatePacket_Sent), 0, (struct sockaddr*) socket, sizeof(struct sockaddr_in)) >= 0)
        return;

    BA_LOGGER_ERROR("Dropped packet due to error: %s (%i)\n", strerror(errno), errno);
}
#endif
BA_CPLUSPLUS_SUPPORT_GUARD_END()
