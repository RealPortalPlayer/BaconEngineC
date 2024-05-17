// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/String.h>
#include <string.h>
#include <BaconAPI/Internal/Boolean.h>
#include <BaconAPI/Debugging/Assert.h>

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

BE_BINARYEXPORT void BE_Packet_Register(uint16_t operationCode, BA_Boolean acceptUnconnected, BE_Packet_Run Run) {
#ifndef BE_CLIENT_BINARY
#   ifndef BE_DISABLE_NETWORK
    BE_PrivatePacket_Registered* packet = BE_EngineMemory_AllocateMemory(sizeof(BE_PrivatePacket_Registered), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_PACKET);
    
    packet->operationCode = operationCode;
    packet->acceptUnconnected = acceptUnconnected;
    packet->Run = Run;

    BE_PrivateDynamicArray_CheckResize(&bePacketRegistered);
    BA_DynamicArray_AddElementToLast(&bePacketRegistered, packet);
#   endif
#else
    BE_INTERFACEFUNCTION(void, uint16_t, BA_Boolean, BE_Packet_Run)(operationCode, acceptUnconnected, Run);
#endif
}

BE_BINARYEXPORT void BE_Packet_Send(BE_Client client, uint16_t operationCode, char data[BE_PACKET_MAXIMUM_DATA]) {
#ifndef BE_CLIENT_BINARY
#   ifndef BE_DISABLE_NETWORK
    if (client == BE_CLIENT_UNCONNECTED) {
        // TODO: Send packet to server
        return;
    }

    BE_PrivatePacket_Send(BE_PrivateServer_GetPrivateClientFromClient(client)->socket, operationCode, data);
#   endif
#else
    BE_INTERFACEFUNCTION(void, BE_Client, uint16_t, char[BE_PACKET_MAXIMUM_DATA])(client, operationCode, data);
#endif
}

#if !defined(BE_CLIENT_BINARY) && !defined(BE_DISABLE_NETWORK)
void BE_PrivatePacket_Parse(BE_PrivateClient* client, struct sockaddr_in* descriptor, BE_PrivatePacket_Sent packet) {
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
        BA_LOGGER_FATAL("Invalid packet: invalid operation code (%u)\n", packet.operationCode);
        return;
    }

    foundPacket->Run(client->publicClient, packet.data);
}

void BE_PrivatePacket_Destroy(void) {
    BA_ASSERT(bePacketInitialized, "The packet system is not initialized\n");
    BA_LOGGER_INFO("Destroying the packet system\n");
    
    bePacketInitialized = BA_BOOLEAN_FALSE;

    for (int i = 0; i < bePacketRegistered.used; i++)
        BE_EngineMemory_DeallocateMemory(bePacketRegistered.internalArray[i], sizeof(BE_PrivatePacket_Registered), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_PACKET);

    BE_EngineMemory_DeallocateMemory(bePacketRegistered.internalArray, sizeof(void*) * bePacketRegistered.size, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY);
}

void BE_PrivatePacket_Send(struct sockaddr_in* socket, uint16_t operationCode, char data[BE_PACKET_MAXIMUM_DATA]) {
    BE_PrivatePacket_Sent packet = {0};

    memcpy(&packet.magic, BE_PRIVATEPACKET_MAGIC, BE_PRIVATEPACKET_MAGIC_LENGTH);

    packet.operationCode = operationCode; // TODO: Endianess

    if (data != NULL)
        memcpy(&packet.data, data, sizeof(char) * BE_PACKET_MAXIMUM_DATA);
    
    sendto(BE_PrivateServer_GetSocketDescriptor(), &packet, sizeof(BE_PrivatePacket_Sent), 0, (struct sockaddr*) socket, sizeof(struct sockaddr_in));
}
#endif
BA_CPLUSPLUS_SUPPORT_GUARD_END()
