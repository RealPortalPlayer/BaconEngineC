// Purpose: Stores and manages commands.
// Created on: 8/7/22 @ 10:03 AM

// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/String.h>

#include "Command.h"
#include "BaconEngine/BinaryExport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_BINARYEXPORT BE_Command** BE_Console_GetCommands(void);
BE_BINARYEXPORT BE_Command* BE_Console_GetCommand(const char* name);

/**
 * @return Amount of registered commands.
 */
BE_BINARYEXPORT int BE_Console_GetCommandAmount(void);

/**
 * @return Maximum amount of commands until requiring a reallocation.
 */
BE_BINARYEXPORT int BE_Console_GetAllocatedCommandsAmount(void);

/**
 * @return Amount of times the maximum amount was reached.
 */
BE_BINARYEXPORT int BE_Console_GetCommandReallocationAmount(void);
BE_BINARYEXPORT void BE_Console_ExecuteCommand(const char* input, BE_Client client); // TODO: Client
BE_BINARYEXPORT BA_Boolean BE_Console_IsEngineCommand(BE_Command command);
BE_BINARYEXPORT void BE_Console_ExecuteFile(FILE* file);
BE_BINARYEXPORT void BE_Console_ExecuteListOfCommands(const char* contents);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
