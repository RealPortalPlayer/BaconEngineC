// Purpose: Packet information
// Created on: 1/8/24 @ 4:11 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>

#include "BaconEngine/Server/Client.h"
#include "../AntiClientGuard.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    BA_Boolean acceptUnconnected;
    char* name;

    /**
     * @param client - NULL if it's unconnected
     */
    void (*Run)(BE_Client client, struct sockaddr_in* descriptor);
} BE_PrivatePacket;

void BE_PrivatePacket_Initialize(void);
void BE_PrivatePacket_Parse(BE_Client client, struct sockaddr_in* descriptor, const char* buffer);
void BE_PrivatePacket_Destroy(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
