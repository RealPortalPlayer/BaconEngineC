// Purpose: 
// Created on: 1/8/24 @ 3:20 PM

// Copyright (c) 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>

#include "Client.h"
#include "BaconEngine/BinaryExport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
// TODO: Packet arguments

BE_BINARYEXPORT void BE_Packet_Register(const char* name, BA_Boolean acceptUnconnected, void (*Run)(BE_Client client));

BE_BINARYEXPORT void BE_Packet_Send(BE_Client client, const char* buffer);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
