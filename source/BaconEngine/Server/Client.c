// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <BaconAPI/Debugging/Assert.h>
#include <BaconAPI/Internal/OperatingSystem.h>

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <unistd.h>
#   define BE_CLIENT_SLEEP(time) sleep(time)
#else
#   include <Windows.h>
#   define BE_CLIENT_SLEEP(time) Sleep(time * 1000)
#endif

#include "BaconEngine/Server/Client.h"
#include "../InterfaceFunctions.h"
#include "BaconEngine/Client/Information.h"
#include "SharedEngineCode/Debugging/StrictMode.h"

#ifndef BE_CLIENT_BINARY
#   include "PrivateServer.h"
#   include "PrivateClient.h"
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#if !defined(BE_CLIENT_BINARY) && !defined(BE_DISABLE_NETWORK)
static BE_PRIVATECLIENT_SOCKET_TYPE beClientSocket = BE_PRIVATECLIENT_INVALID_SOCKET;
static struct sockaddr_in beClientSocketAddress;
#endif

BA_Boolean BE_Client_IsConnected(void) {
#ifndef BE_CLIENT_BINARY
    return beClientSocket != BE_PRIVATECLIENT_INVALID_SOCKET;
#else
    BE_INTERFACEFUNCTION(BA_Boolean);
    return function();
#endif
}

void BE_Client_StartConnection(const char* address, uint16_t port) {
#ifndef BE_CLIENT_BINARY
#   ifndef BE_DISABLE_NETWORK
    if (beClientSocket != BE_PRIVATECLIENT_INVALID_SOCKET)
        BE_Client_Disconnect();

    BA_LOGGER_DEBUG("Connecting\n");
    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(!BE_ClientInformation_IsServerModeEnabled(), "Servers cannot connect to other servers\n");

    beClientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(beClientSocket != BE_PRIVATECLIENT_INVALID_SOCKET, "Failed to create client socket: %s", strerror(errno));
    memset(&beClientSocketAddress, 0, sizeof(struct sockaddr_in));

    beClientSocketAddress.sin_family = AF_INET;
    beClientSocketAddress.sin_addr.s_addr = inet_addr(address);
    beClientSocketAddress.sin_port = htons(port);
#   else
    BA_LOGGER_ERROR("Networking code is disabled\n");
#   endif
#else
    BE_INTERFACEFUNCTION(void, const char*, int)(address, port);
#endif
}

#ifndef BE_CLIENT_BINARY
BE_PRIVATECLIENT_SOCKET_TYPE BE_PrivateClient_GetSocketDescriptor(void) {
    return beClientSocket;
}
#endif

void BE_Client_Disconnect(void) {
#ifndef BE_CLIENT_BINARY
#   ifndef BE_DISABLE_NETWORK
    BA_LOGGER_DEBUG("Disconnecting\n");
    
    if (!BE_Client_IsConnected()) {
        BA_LOGGER_ERROR("Already disconnected\n");
        return;
    }

    close(beClientSocket);

    beClientSocket = BE_PRIVATECLIENT_INVALID_SOCKET;
#   else
    BA_LOGGER_ERROR("Networking code is disabled\n");
#   endif
#else
    BE_INTERFACEFUNCTION(void, void)();
#endif
}

#ifndef BE_CLIENT_BINARY
struct sockaddr_in* BE_PrivateClient_GetSocketAddress(void) {
    return &beClientSocketAddress;
}
#endif
BA_CPLUSPLUS_SUPPORT_GUARD_END()
