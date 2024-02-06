// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <SharedEngineCode/Debugging/StrictMode.h>
#include <BaconAPI/Number.h>
#include <BaconAPI/String.h>
#include <BaconAPI/Internal/OperatingSystem.h>

#include "BaconEngine/Server/Server.h"
#include "../InterfaceFunctions.h"
#include "BaconEngine/Client/Information.h"
#include "BaconEngine/Server/Packet.h"
#include "BaconAPI/Debugging/Assert.h"

#ifndef BE_CLIENT_BINARY
#   include "../EngineMemory.h"
#   include "PrivateServer.h"
#   include "PrivateClient.h"
#   include "PrivatePacket.h"
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static BE_SERVER_SOCKET_TYPE beServerSocket = BE_SERVER_INVALID_SOCKET;
static unsigned beServerPort;
static BE_PrivateClient** beServerConnected;
static unsigned beServerMaxPlayers;
static BA_Boolean beServerConnectedCountDirty = BA_BOOLEAN_TRUE;
static BE_PrivateClient* beServerGenericUnconnected;
#endif

BA_Boolean BE_Server_IsRunning(void) {
#ifndef BE_CLIENT_BINARY
    return beServerSocket != BE_SERVER_INVALID_SOCKET;
#else
    BE_INTERFACEFUNCTION(BA_Boolean, void);
    return function();
#endif
}

unsigned BE_Server_GetPort(void) {
#ifndef BE_CLIENT_BINARY
    return beServerPort;
#else
    BE_INTERFACEFUNCTION(unsigned, void);
    return function();
#endif
}

#ifndef BE_CLIENT_BINARY
BE_SERVER_SOCKET_TYPE BE_PrivateServer_GetSocketDescriptor(void) {
    return beServerSocket;
}
#endif

void BE_Server_Start(unsigned port) {
#ifndef BE_CLIENT_BINARY
    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(beServerSocket == BE_SERVER_INVALID_SOCKET, "Server is already running\n");
    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(BE_ClientInformation_IsServerModeEnabled(), "Cannot start a server on a non-server client\n");
    BA_LOGGER_INFO("Starting server: 0.0.0.0:%d\n", port);
    
#   if BA_OPERATINGSYSTEM_WINDOWS
    {
        WSADATA data;
        
        BA_ASSERT(WSAStartup(MAKEWORD(2, 2), &data) == 0, "Failed to start Windows Socket: %i\n", GetLastError());
        BA_ASSERT(LOBYTE(data.wVersion) == 2 && HIBYTE(data.wVersion) == 2, "Incorrect Windows Socket version returned: %i.%i\n",
                  LOBYTE(data.wVersion), HIBYTE(data.wVersion));
    }
#   endif
    
    beServerMaxPlayers = 10;
    beServerSocket = socket(AF_INET, SOCK_DGRAM, 0);
    beServerPort = port;

    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(beServerSocket != BE_SERVER_INVALID_SOCKET, "Failed to create socket: %s\n", strerror(errno));

    {
        BA_ArgumentHandler_ShortResults maxPlayersResults;

        if (BA_ArgumentHandler_GetInformationWithShort(SEC_BUILTINARGUMENTS_MAX_PLAYERS, SEC_BUILTINARGUMENTS_MAX_PLAYERS_SHORT, BA_BOOLEAN_FALSE, &maxPlayersResults) != 0)
            beServerMaxPlayers = BA_Number_StringToUnsigned(*maxPlayersResults.value, NULL, NULL, "Invalid max players, defaulting with 10\n", beServerMaxPlayers);
    }
    
    BA_LOGGER_INFO("Allocating player list: %u player%s\n", beServerMaxPlayers, beServerMaxPlayers != 1 ? "s" : "");

    beServerConnected = BE_EngineMemory_AllocateMemory(sizeof(BE_PrivateClient*) * beServerMaxPlayers, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CONNECTED);
    beServerGenericUnconnected = BE_EngineMemory_AllocateMemory(sizeof(BE_PrivateClient), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CLIENT);
    beServerGenericUnconnected->publicClient = BE_CLIENT_UNCONNECTED;
    beServerGenericUnconnected->socket = NULL;
    
    for (int i = 0; i < beServerMaxPlayers; i++)
        beServerConnected[i] = beServerGenericUnconnected;
    
    struct sockaddr_in serverAddress;

    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(beServerPort);

    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(bind(beServerSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == 0,
                                        "Failed to bind to socket: %s\n", strerror(errno));
#else
    BE_INTERFACEFUNCTION(void, unsigned)(port);
#endif
}

#ifndef BE_CLIENT_BINARY
void BE_PrivateServer_AddConnection(struct sockaddr_in* clientDescriptor) {
    if (beServerMaxPlayers == BE_Server_GetConnectedAmount()) {
        BE_PrivatePacket_Send(clientDescriptor, "error server_full");
        return;
    }

    for (int i = 0; i < beServerMaxPlayers; i++) {
        if (beServerConnected[i]->publicClient != BE_CLIENT_UNCONNECTED)
            continue;

        BE_PrivateClient* client = BE_EngineMemory_AllocateMemory(sizeof(BE_PrivateClient), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CLIENT);
        
        beServerConnected[i] = client;
        beServerConnected[i]->publicClient = i;
        // TODO: Is this even necessary? I would assume so
        beServerConnected[i]->socket = BE_EngineMemory_AllocateMemory(sizeof(struct sockaddr_in), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CLIENT_SOCKET);
        beServerConnectedCountDirty = BA_BOOLEAN_TRUE;

        memcpy(beServerConnected[i]->socket, clientDescriptor, sizeof(struct sockaddr_in));
        return;
    }

    BE_PrivatePacket_Send(clientDescriptor, "error crash");
    BA_ASSERT_ALWAYS("No more free client slots while also not being full, this shouldn't happen\n");
}
#endif

void BE_Server_Stop(void) {
#ifndef BE_CLIENT_BINARY
    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(beServerSocket != BE_SERVER_INVALID_SOCKET, "Server is not running\n");
    BA_LOGGER_INFO("Closing server\n");
    
    for (int i = 0; i < beServerMaxPlayers; i++) {
        if (beServerConnected[i]->publicClient == BE_CLIENT_UNCONNECTED)
            continue;

        // TODO: Kick client
        
        if (beServerConnected[i]->socket != NULL)
            BE_EngineMemory_DeallocateMemory(beServerConnected[i]->socket, sizeof(struct sockaddr_in), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CLIENT_SOCKET);
        
        BE_EngineMemory_DeallocateMemory(beServerConnected[i], sizeof(BE_PrivateClient), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CLIENT);
    }

    BE_EngineMemory_DeallocateMemory(beServerConnected, sizeof(BE_PrivateClient*) * beServerMaxPlayers, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CONNECTED);
    BE_EngineMemory_DeallocateMemory(beServerGenericUnconnected, sizeof(BE_PrivateClient), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_SERVER_CLIENT);

#   if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    close(beServerSocket);
#   elif BA_OPERATINGSYSTEM_WINDOWS
    closesocket(beServerSocket);
    WSACleanup();
#   endif
    
    beServerSocket = BE_SERVER_INVALID_SOCKET;
#else
    BE_INTERFACEFUNCTION(void, void)();
#endif
}

#ifndef BE_CLIENT_BINARY
BE_PrivateClient* BE_PrivateServer_GetPrivateClientFromSocket(struct sockaddr_in* clientDescriptor) {
    for (int i = 0; i < beServerMaxPlayers; i++) {
        if (beServerConnected[i]->socket == NULL || memcmp(beServerConnected[i]->socket, clientDescriptor, sizeof(struct sockaddr_in)) != 0)
            continue;

        return beServerConnected[i];
    }
    
    return beServerGenericUnconnected;
}
#endif

BE_BINARYEXPORT unsigned BE_Server_GetConnectedAmount(void) {
#ifndef BE_CLIENT_BINARY
    static unsigned count = 0;

    if (!beServerConnectedCountDirty)
        return count;
    
    count = 0;
    
    for (int i = 0; i < beServerMaxPlayers; i++) {
        if (beServerConnected[i]->publicClient == BE_CLIENT_UNCONNECTED)
            continue;
        
        count++;
    }
    
    return count;
#else
    BE_INTERFACEFUNCTION(unsigned, void);
    return function();
#endif
}

#ifndef BE_CLIENT_BINARY
BE_PrivateClient* BE_PrivateServer_GetPrivateClientFromClient(BE_Client client) {
    if (client == BE_CLIENT_UNCONNECTED)
        return beServerGenericUnconnected;
    
    for (int i = 0; i < beServerMaxPlayers; i++) {
        if (beServerConnected[i]->publicClient != client)
            continue;
        
        return beServerConnected[i];
    }

    return beServerGenericUnconnected;
}
#endif
BA_CPLUSPLUS_SUPPORT_GUARD_END()
