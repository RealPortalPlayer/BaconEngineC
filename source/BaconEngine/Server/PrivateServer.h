// Purpose: Private server socket getter.
// Created on: 10/22/22 @ 5:50 AM

// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <netinet/in.h>

#include "../AntiClientGuard.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
int BE_PrivateServer_GetSocketDescriptor(void);
void BE_PrivateServer_AddConnection(struct sockaddr_in* clientDescriptor);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
