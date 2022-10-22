#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>

#include "BaconEngine/Server/Server.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Server/Client.h"
#include "PrivateServer.h"
#include "../EngineMemory.h"

SEC_CPP_SUPPORT_GUARD_START()
int beServerSocket = -1;
unsigned beServerPort;
BE_Client_Connected** beServerConnected;
int beServerConnectedAmount;
int beServerMaxPlayers;

int BE_Server_IsRunning(void) {
    return beServerSocket != -1;
}

unsigned BE_Server_GetPort(void) {
    return beServerPort;
}

int BE_PrivateServer_GetSocketDescriptor(void) {
    return beServerSocket;
}

void BE_Server_Start(unsigned port) {
    BE_STRICTMODE_CHECK_NO_RETURN_VALUE(beServerSocket == -1, "Server is already running\n");
    BE_STRICTMODE_CHECK_NO_RETURN_VALUE(BE_ClientInformation_IsServerModeEnabled(), "Cannot start a server on a non-server client\n");
    SEC_LOGGER_INFO("Starting server\n");

    // TODO: Custom max players

    beServerConnectedAmount = 0;
    beServerMaxPlayers = 10;
    beServerConnected = BE_EngineMemory_AllocateMemory(sizeof(BE_Client_Connected) * beServerMaxPlayers, BE_ENGINEMEMORY_MEMORY_TYPE_SERVER);
    beServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    beServerPort = port;

    BE_STRICTMODE_CHECK_NO_RETURN_VALUE(beServerSocket != -1, "Failed to create socket: %s\n", strerror(errno));

    struct sockaddr_in serverAddress;

    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(beServerPort);

    BE_STRICTMODE_CHECK_NO_RETURN_VALUE(bind(beServerSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == 0,
                                        "Failed to bind to socket: %s\n", strerror(errno));
    BE_STRICTMODE_CHECK_NO_RETURN_VALUE(listen(beServerSocket, 5) == 0, "Failed to listen to socket: %s\n", strerror(errno));
}

void BE_PrivateServer_AddConnection(int clientDescriptor) {
    if (beServerMaxPlayers == beServerConnectedAmount) {
        // TODO: Kick client: server full
        return;
    }

    BE_Client_Connected* client = BE_EngineMemory_AllocateMemory(sizeof(BE_Client_Connected), BE_ENGINEMEMORY_MEMORY_TYPE_SERVER);

    client->clientId = beServerConnectedAmount;
    client->clientDescriptor = clientDescriptor;
    beServerConnected[beServerConnectedAmount++] = client;
}

void BE_Server_Stop(void) {
    BE_STRICTMODE_CHECK_NO_RETURN_VALUE(beServerSocket != -1, "Server is not running\n");
    SEC_LOGGER_INFO("Closing server\n");

    // TODO: Kick clients

    BE_EngineMemory_DeallocateMemory(beServerConnected, sizeof(BE_Client_Connected) * beServerMaxPlayers, BE_ENGINEMEMORY_MEMORY_TYPE_SERVER);

    close(beServerSocket);
}
SEC_CPP_SUPPORT_GUARD_END()
