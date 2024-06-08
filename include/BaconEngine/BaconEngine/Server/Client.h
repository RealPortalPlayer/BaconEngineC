// Purpose: The connected client.
// Created on: 10/22/22 @ 5:03 AM

// Copyright (c) 2022, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <stdint.h>
#include <BaconAPI/Internal/Boolean.h>

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef int BE_Client;

BA_Boolean BE_Client_IsConnected(void);

void BE_Client_StartConnection(const char* address, uint16_t port);
void BE_Client_Disconnect(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#define BE_CLIENT_UNCONNECTED (-1)
