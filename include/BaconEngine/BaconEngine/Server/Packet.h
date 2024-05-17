// Purpose: Main packet implementation
// Created on: 1/8/24 @ 3:20 PM

// Copyright (c) 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>
#include <BaconAPI/Internal/Compiler.h>
#include <stdint.h>

#include "Client.h"
#include "BaconEngine/BinaryExport.h"

#define BE_PACKET_MAXIMUM_DATA 1024

#if BA_COMPILER_GCC || BA_COMPILER_CLANG
#   define BE_PACKET_PACKED(structure) structure __attribute__((packed))
#elif BA_COMPILER_MSVC
#   define BE_PACKET_PACKED(structure) __pragma(pack(push, 1)) structure __pragma(pack(pop))
#else
#   define BE_PACKET_PACKED(structure) structure
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef void (*BE_Packet_Run)(BE_Client client, char data[BE_PACKET_MAXIMUM_DATA]);

// TODO: Flags

BE_BINARYEXPORT void BE_Packet_Register(uint16_t operationCode, BA_Boolean acceptUnconnected, BE_Packet_Run Run);

BE_BINARYEXPORT void BE_Packet_Send(BE_Client client, uint16_t operationCode, char data[BE_PACKET_MAXIMUM_DATA]);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

