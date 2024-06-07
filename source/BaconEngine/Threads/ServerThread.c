// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/Internal/OperatingSystem.h>

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <unistd.h>
#   include <fcntl.h>
#elif BA_OPERATINGSYSTEM_WINDOWS
#   define fileno _fileno
#   include <Windows.h>
#endif

#include <errno.h>
#include <string.h>

#include "ServerThread.h"
#include "../Server/PrivateServer.h"
#include "BaconEngine/Client/Information.h"
#include "../Server/PrivatePacket.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_BASETHREAD_CREATE_IMPLEMENTATION(BE_ServerThread, beServerThread, "server") {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    fcntl(BE_PrivateServer_GetSocketDescriptor(), F_SETFL, O_NONBLOCK);
#elif BA_OPERATINGSYSTEM_WINDOWS
    {
        ULONG enable = 1;
        
        ioctlsocket(BE_PrivateServer_GetSocketDescriptor(), FIONBIO, &enable);
    }
#endif
    
    while (BE_ClientInformation_IsRunning()) {
        struct sockaddr_in clientInterface;
        socklen_t clientSize = sizeof(clientInterface);
        BE_PrivatePacket_Sent packet = {0};
        ssize_t packetLength = recvfrom(BE_PrivateServer_GetSocketDescriptor(), &packet, sizeof(BE_PrivatePacket_Sent), 0, (struct sockaddr*) &clientInterface, &clientSize);

        if (packetLength == -1) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
            if (errno == EWOULDBLOCK)
                continue;
#elif BA_OPERATINGSYSTEM_WINDOWS
            if (WSAGetLastError() == WSAEWOULDBLOCK)
                continue;
#endif
            
            BA_LOGGER_ERROR("Errored while getting packet from client: %s\n", strerror(errno));
            continue;
        }

        if (packetLength != sizeof(BE_PrivatePacket_Sent)) {
            BA_LOGGER_ERROR("Invalid packet: size mismatch (%zu != %zu)\n", packetLength, sizeof(BE_PrivatePacket_Sent));
            continue; // TODO: Disconnect
        }
        
        BE_PrivatePacket_Parse(BE_PrivateServer_GetPrivateClientFromSocket(&clientInterface), &clientInterface, packet);
    }

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    return NULL;
#else
    return 0;
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
