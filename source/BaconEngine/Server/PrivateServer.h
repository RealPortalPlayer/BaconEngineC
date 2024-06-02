// Purpose: Private server socket getter.
// Created on: 10/22/22 @ 5:50 AM

// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/OperatingSystem.h>

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <netinet/in.h>
#   include <sys/socket.h>
#   define BE_SERVER_SOCKET_TYPE int
#   define BE_SERVER_INVALID_SOCKET (-1)
#elif BA_OPERATINGSYSTEM_WINDOWS
#   define BE_SERVER_SOCKET_TYPE SOCKET
#   define BE_SERVER_INVALID_SOCKET INVALID_SOCKET
#endif

#include "../AntiClientGuard.h"
#include "PrivateClient.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_SERVER_SOCKET_TYPE BE_PrivateServer_GetSocketDescriptor(void);
void BE_PrivateServer_AddConnection(struct sockaddr_in* clientDescriptor);
BE_PrivateClient* BE_PrivateServer_GetPrivateClientFromSocket(struct sockaddr_in* clientDescriptor);
BE_PrivateClient* BE_PrivateServer_GetPrivateClientFromClient(BE_Client client);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
