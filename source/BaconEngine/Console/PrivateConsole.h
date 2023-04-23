// Purpose: Private console constructor.
// Created on: 10/12/22 @ 1:30 AM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "BaconEngine/Console/Command.h"
#include "../AntiClientGuard.h"

SEC_CPP_SUPPORT_GUARD_START()
typedef struct {
    BE_Command publicCommand;
    SEC_Boolean engineCommand;
    SEC_Boolean duplicate;
} BE_PrivateConsole_Command;

void BE_PrivateConsole_Initialize(void);
void BE_PrivateConsole_Destroy(void);
SEC_CPP_SUPPORT_GUARD_END()
