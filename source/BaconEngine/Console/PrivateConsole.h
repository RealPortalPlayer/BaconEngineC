// Purpose: Private console constructor.
// Created on: 10/12/22 @ 1:30 AM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>

#include "../AntiClientGuard.h"
#include "BaconEngine/Console/Command.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    BE_Command publicCommand;
    SEC_Boolean engineCommand;
    SEC_Boolean duplicate;
    int requiredArgumentCount;
} BE_PrivateConsole_Command;

void BE_PrivateConsole_Initialize(void);
void BE_PrivateConsole_Destroy(void);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
