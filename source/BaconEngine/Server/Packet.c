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
#ifndef BE_CLIENT_BINARY
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

BE_BINARYEXPORT void BE_Packet_Register(const char* name, BA_Boolean acceptUnconnected, void (*Run)(BE_Client client)) {
#ifndef BE_CLIENT_BINARY
    BE_PrivatePacket* packet = BE_EngineMemory_AllocateMemory(sizeof(BE_PrivatePacket), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_PACKET);
    
    packet->name = BA_String_Copy(name);
    packet->acceptUnconnected = acceptUnconnected;
    packet->Run = Run;

    BE_PrivateDynamicArray_CheckResize(&bePacketRegistered);
    BA_DynamicArray_AddElementToLast(&bePacketRegistered, packet);
#else
    BE_INTERFACEFUNCTION(void, const char*, BA_Boolean, void (*)(BE_Client))(name, acceptUnconnected, Run);
#endif
}

BE_BINARYEXPORT void BE_Packet_Send(BE_Client client, const char* buffer) {
#ifndef BE_CLIENT_BINARY
    if (client == BE_CLIENT_UNCONNECTED) {
        // TODO: Send packet to server
        return;
    }

    BE_PrivatePacket_Send(BE_PrivateServer_GetPrivateClientFromClient(client)->socket, buffer);
#else
    BE_INTERFACEFUNCTION(void, BE_Client, const char*)(client, buffer);
#endif
}

#ifndef BE_CLIENT_BINARY
void BE_PrivatePacket_Parse(BE_PrivateClient* client, struct sockaddr_in* descriptor, const char* buffer) {
    BE_PrivatePacket* packet;

    if (client->publicClient == BE_CLIENT_UNCONNECTED)
        client->socket = descriptor;
    
    for (int i = 0; i < bePacketRegistered.used; i++) {
        packet = BA_DYNAMICARRAY_GET_ELEMENT(BE_PrivatePacket, bePacketRegistered, i);

        if (!BA_String_Equals(packet->name, buffer, BA_BOOLEAN_TRUE)) {
            packet = NULL;
            continue;
        }

        break;
    }

    if (packet == NULL) {
        if (BE_ClientInformation_IsServerModeEnabled()) {
            if (client->publicClient != BE_CLIENT_UNCONNECTED) {
                // TODO: Kick the client
                return;
            }

            BE_PrivatePacket_Send(client->socket, "error invalid_packet");
            return;
        } else {
            // TODO: Leave the server
        }
        
        BA_LOGGER_FATAL("Invalid packet (%s)\n", buffer);
        return;
    }

    packet->Run(client->publicClient);
}

void BE_PrivatePacket_Destroy(void) {
    BA_ASSERT(bePacketInitialized, "The packet system is not initialized\n");
    BA_LOGGER_INFO("Destroying the packet system\n");
    
    bePacketInitialized = BA_BOOLEAN_FALSE;

    for (int i = 0; i < bePacketRegistered.used; i++) {
        free(BA_DYNAMICARRAY_GET_ELEMENT(BE_PrivatePacket, bePacketRegistered, i)->name);
        BE_EngineMemory_DeallocateMemory(bePacketRegistered.internalArray[i], sizeof(BE_PrivatePacket), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_PACKET);
    }

    BE_EngineMemory_DeallocateMemory(bePacketRegistered.internalArray, sizeof(void*) * bePacketRegistered.size, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY);
}

void BE_PrivatePacket_Send(struct sockaddr_in* socket, const char* buffer) {
    sendto(BE_PrivateServer_GetSocketDescriptor(), buffer, strlen(buffer), 0, (struct sockaddr*) socket, sizeof(struct sockaddr_in));
}
#endif
BA_CPLUSPLUS_SUPPORT_GUARD_END()
