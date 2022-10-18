// Purpose: Private console constructor.
// Created on: 10/12/22 @ 1:30 AM

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "BaconEngine/Console/Command.h"

SEC_CPP_SUPPORT_GUARD_START()
typedef struct {
    BE_Command publicCommand;
    int engineCommand;
    int duplicate;
} BE_PrivateConsole_Command;

void BE_PrivateConsole_Initialize(void);
void BE_PrivateConsole_Destroy(void);
SEC_CPP_SUPPORT_GUARD_END()
