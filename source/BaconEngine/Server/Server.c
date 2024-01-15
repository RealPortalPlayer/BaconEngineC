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
#include <arpa/inet.h>

#include "BaconEngine/Server/Server.h"
#include "BaconEngine/Server/Client.h"
#include "../InterfaceFunctions.h"
#include "BaconEngine/ClientInformation.h"
#include "BaconEngine/Server/Packet.h"

#ifndef BE_CLIENT_BINARY
#   include "../EngineMemory.h"
#   include "PrivateServer.h"
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static int beServerSocket = -1;
static unsigned beServerPort;
static BE_Client_Connected** beServerConnected;
static int beServerConnectedAmount;
static unsigned beServerMaxPlayers;
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
    
    beServerConnectedAmount = 0;
    beServerMaxPlayers = 10;
    beServerConnected = BE_EngineMemory_AllocateMemory(sizeof(BE_Client_Connected) * beServerMaxPlayers, BE_ENGINEMEMORY_MEMORY_TYPE_SERVER);
    beServerSocket = socket(AF_INET, SOCK_DGRAM, 0);
    beServerPort = port;

    {
        BA_ArgumentHandler_ShortResults maxPlayersResults;

        if (BA_ArgumentHandler_GetInformationWithShort(SEC_BUILTINARGUMENTS_MAX_PLAYERS, SEC_BUILTINARGUMENTS_MAX_PLAYERS_SHORT, BA_BOOLEAN_FALSE, &maxPlayersResults) != 0)
            beServerMaxPlayers = BA_Number_StringToUnsigned(*maxPlayersResults.value, NULL, NULL, "Invalid max players, defaulting with 10\n", 10);
    }
    
    BA_LOGGER_INFO("Allocating player list: %u player%s\n", beServerMaxPlayers, beServerMaxPlayers != 1 ? "s" : "");
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
    if (beServerMaxPlayers == beServerConnectedAmount) {
        BE_Packet_Send(clientDescriptor, "error server full");
        return;
    }

    BE_Client_Connected* client = BE_EngineMemory_AllocateMemory(sizeof(BE_Client_Connected), BE_ENGINEMEMORY_MEMORY_TYPE_SERVER);
    
    client->clientId = beServerConnectedAmount;
    client->addressPort = BA_String_Copy("%s:%d");

    BA_String_Format(&client->addressPort, inet_ntoa(clientDescriptor->sin_addr), ntohs(clientDescriptor->sin_port));
    
    beServerConnected[beServerConnectedAmount++] = client;
}
#endif

void BE_Server_Stop(void) {
#ifndef BE_CLIENT_BINARY
    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(beServerSocket != -1, "Server is not running\n");
    BA_LOGGER_INFO("Closing server\n");

    // TODO: Kick clients

    for (int i = 0; i < beServerMaxPlayers; i++) {
        if (beServerConnected[i] == NULL)
            continue;

        free(beServerConnected[i]->addressPort);
        BE_EngineMemory_DeallocateMemory(beServerConnected[i], sizeof(BE_Client_Connected), BE_ENGINEMEMORY_MEMORY_TYPE_SERVER);
    }

    BE_EngineMemory_DeallocateMemory(beServerConnected, sizeof(BE_Client_Connected) * beServerMaxPlayers, BE_ENGINEMEMORY_MEMORY_TYPE_SERVER);
    close(beServerSocket);
#else
    BE_INTERFACEFUNCTION(void, void)();
#endif
}

#ifndef BE_CLIENT_BINARY
BE_Client_Connected* BE_PrivateServer_GetClientFromAddress(struct sockaddr_in* clientDescriptor) {
    for (int i = 0; i < beServerConnectedAmount; i++) {
        // FIXME: This is so stupid. This can be very time sensitive, so allocating memory can be a bad idea
        char* addressPort = BA_String_Copy("%s:%d");

        BA_String_Format(&addressPort, inet_ntoa(clientDescriptor->sin_addr), ntohs(clientDescriptor->sin_port));
        
        if (strcmp(beServerConnected[i]->addressPort, addressPort) != 0) {
            free(addressPort);
            continue;
        }

        free(addressPort);
        return beServerConnected[i];
    }
    
    return NULL;
}
#endif
BA_CPLUSPLUS_SUPPORT_GUARD_END()
