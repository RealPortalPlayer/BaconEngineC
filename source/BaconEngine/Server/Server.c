// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <SharedEngineCode/Debugging/StrictMode.h>
#include <BaconAPI/Number.h>
#include <BaconAPI/String.h>

#include "BaconEngine/Server/Server.h"
#include "../InterfaceFunctions.h"
#include "BaconEngine/ClientInformation.h"
#include "BaconEngine/Server/Packet.h"
#include "BaconAPI/Debugging/Assert.h"

#ifndef BE_CLIENT_BINARY
#   include "../EngineMemory.h"
#   include "PrivateServer.h"
#   include "PrivateClient.h"
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static int beServerSocket = -1;
static unsigned beServerPort;
static BE_PrivateClient** beServerConnected;
static unsigned beServerMaxPlayers;
static BA_Boolean beServerConnectedCountDirty = BA_BOOLEAN_TRUE;
#endif

BA_Boolean BE_Server_IsRunning(void) {
#ifndef BE_CLIENT_BINARY
    return beServerSocket != -1;
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
int BE_PrivateServer_GetSocketDescriptor(void) {
    return beServerSocket;
}
#endif

void BE_Server_Start(unsigned port) {
#ifndef BE_CLIENT_BINARY
    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(beServerSocket == -1, "Server is already running\n");
    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(BE_ClientInformation_IsServerModeEnabled(), "Cannot start a server on a non-server client\n");
    BA_LOGGER_INFO("Starting server: 0.0.0.0:%d\n", port);
    
    beServerMaxPlayers = 10;
    beServerSocket = socket(AF_INET, SOCK_DGRAM, 0);
    beServerPort = port;

    {
        BA_ArgumentHandler_ShortResults maxPlayersResults;

        if (BA_ArgumentHandler_GetInformationWithShort(SEC_BUILTINARGUMENTS_MAX_PLAYERS, SEC_BUILTINARGUMENTS_MAX_PLAYERS_SHORT, BA_BOOLEAN_FALSE, &maxPlayersResults) != 0)
            beServerMaxPlayers = BA_Number_StringToUnsigned(*maxPlayersResults.value, NULL, NULL, "Invalid max players, defaulting with 10\n", beServerMaxPlayers);
    }
    
    BA_LOGGER_INFO("Allocating player list: %u player%s\n", beServerMaxPlayers, beServerMaxPlayers != 1 ? "s" : "");

    beServerConnected = BE_EngineMemory_AllocateMemory(sizeof(BE_PrivateClient*) * beServerMaxPlayers, BE_ENGINEMEMORY_MEMORY_TYPE_SERVER);

    for (int i = 0; i < beServerMaxPlayers; i++) {
        BE_PrivateClient* privateClient = BE_EngineMemory_AllocateMemory(sizeof(BE_PrivateClient), BE_ENGINEMEMORY_MEMORY_TYPE_SERVER);
        
        privateClient->publicClient = -1;
        privateClient->socket = NULL;
        beServerConnected[i] = privateClient;
    }
    
    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(beServerSocket != -1, "Failed to create socket: %s\n", strerror(errno));

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
        BE_Packet_Send(clientDescriptor, "error server_full");
        return;
    }

    for (int i = 0; i < beServerMaxPlayers; i++) {
        if (beServerConnected[i]->publicClient != BE_CLIENT_UNCONNECTED)
            continue;

        // TODO: Is this even necessary? I would assume so
        beServerConnected[i]->publicClient = i;
        beServerConnected[i]->socket = BE_EngineMemory_AllocateMemory(sizeof(struct sockaddr_in), BE_ENGINEMEMORY_MEMORY_TYPE_SERVER);
        beServerConnectedCountDirty = BA_BOOLEAN_TRUE;

        memcpy(beServerConnected[i]->socket, clientDescriptor, sizeof(struct sockaddr_in));
        return;
    }

    BE_Packet_Send(clientDescriptor, "error crash");
    BA_ASSERT_ALWAYS("No more free client slots while also not being full, this shouldn't happen\n");
}
#endif

void BE_Server_Stop(void) {
#ifndef BE_CLIENT_BINARY
    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(beServerSocket != -1, "Server is not running\n");
    BA_LOGGER_INFO("Closing server\n");
    
    for (int i = 0; i < beServerMaxPlayers; i++) {
        if (beServerConnected[i]->publicClient != BE_CLIENT_UNCONNECTED) {
            // TODO: Kick client
        }

        if (beServerConnected[i]->socket != NULL)
            BE_EngineMemory_DeallocateMemory(beServerConnected[i]->socket, sizeof(struct sockaddr_in), BE_ENGINEMEMORY_MEMORY_TYPE_SERVER);
        
        BE_EngineMemory_DeallocateMemory(beServerConnected[i], sizeof(BE_PrivateClient), BE_ENGINEMEMORY_MEMORY_TYPE_SERVER);
    }

    BE_EngineMemory_DeallocateMemory(beServerConnected, sizeof(BE_PrivateClient*) * beServerMaxPlayers, BE_ENGINEMEMORY_MEMORY_TYPE_SERVER);
    close(beServerSocket);
#else
    BE_INTERFACEFUNCTION(void, void)();
#endif
}

#ifndef BE_CLIENT_BINARY
BE_Client BE_PrivateServer_GetClientFromSocket(struct sockaddr_in* clientDescriptor) {
    for (int i = 0; i < beServerMaxPlayers; i++) {
        if (beServerConnected[i]->socket == NULL || memcmp(beServerConnected[i]->socket, clientDescriptor, sizeof(struct sockaddr_in)) != 0)
            continue;

        return beServerConnected[i]->publicClient;
    }
    
    return BE_CLIENT_UNCONNECTED;
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
BA_CPLUSPLUS_SUPPORT_GUARD_END()
