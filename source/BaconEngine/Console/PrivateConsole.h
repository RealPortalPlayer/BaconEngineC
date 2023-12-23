// Purpose: Private console constructor.
// Created on: 10/12/22 @ 1:30 AM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>

#include "../AntiClientGuard.h"
#include "BaconEngine/Console/Command.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    BE_Command publicCommand;
    BA_Boolean engineCommand;
    BA_Boolean duplicate;
    int requiredArgumentCount;
} BE_PrivateConsole_Command;

void BE_PrivateConsole_Initialize(void);
void BE_PrivateConsole_Destroy(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
