// Purpose: Stores and manages commands.
// Created on: 8/7/22 @ 10:03 AM

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "Command.h"

SEC_CPP_SUPPORT_GUARD_START()
BE_Command** BE_Console_GetCommands(void);
BE_Command* BE_Console_GetCommand(const char* name);

/**
 * @return Amount of registered commands.
 */
int BE_Console_GetCommandAmount(void);

/**
 * @return Maximum amount of commands until requiring a reallocation.
 */
int BE_Console_GetAllocatedCommandsAmount(void);

/**
 * @return Amount of times the maximum amount was reached.
 */
int BE_Console_GetCommandReallocationAmount(void);
void BE_Console_ExecuteCommand(const char* input); // TODO: Client
int BE_Console_IsEngineCommand(BE_Command command);
SEC_CPP_SUPPORT_GUARD_END()
