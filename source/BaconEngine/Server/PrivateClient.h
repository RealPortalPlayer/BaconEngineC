// Purpose: Hidden information about the client
// Created on: 1/15/24 @ 2:11 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/OperatingSystem.h>

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <netinet/in.h>
#elif BA_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#endif

#include "../AntiClientGuard.h"
#include "BaconEngine/Server/Client.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    BE_Client publicClient;
    struct sockaddr_in* socket;
} BE_PrivateClient;
BA_CPLUSPLUS_SUPPORT_GUARD_END()
