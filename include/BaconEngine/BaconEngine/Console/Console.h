// Purpose: Stores and manages commands.
// Created on: 8/7/22 @ 10:03 AM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "Command.h"
#include "BaconEngine/DllExport.h"

SEC_CPP_SUPPORT_GUARD_START()
BE_DLLEXPORT BE_Command** BE_Console_GetCommands(void);
BE_DLLEXPORT BE_Command* BE_Console_GetCommand(const char* name);

/**
 * @return Amount of registered commands.
 */
BE_DLLEXPORT int BE_Console_GetCommandAmount(void);

/**
 * @return Maximum amount of commands until requiring a reallocation.
 */
BE_DLLEXPORT int BE_Console_GetAllocatedCommandsAmount(void);

/**
 * @return Amount of times the maximum amount was reached.
 */
BE_DLLEXPORT int BE_Console_GetCommandReallocationAmount(void);
BE_DLLEXPORT void BE_Console_ExecuteCommand(const char* input); // TODO: Client
BE_DLLEXPORT SEC_Boolean BE_Console_IsEngineCommand(BE_Command command);
SEC_CPP_SUPPORT_GUARD_END()
