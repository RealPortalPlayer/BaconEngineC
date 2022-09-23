// Purpose: Stores and manages commands.
// Created on: 8/7/22 @ 10:03 AM

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "Command.h"

SEC_CPP_SUPPORT_GUARD_START()
BE_Command** BE_Console_GetCommands(void);
int BE_Console_GetCommandAmount(void);
int BE_Console_GetAllocatedCommandsAmount(void);
int BE_Console_GetCommandReallocationAmount(void);
void BE_Console_Initialize(void);
void BE_Console_RegisterCommand(const char* name, const char* description, BE_Command_Flags flags, void (*Run)(BE_Command_Context context));
void BE_Console_DuplicateCommand(const char* name, const char* description);
void BE_Console_ExecuteCommand(const char* input); // TODO: Client
void BE_Console_Destroy(void);
SEC_CPP_SUPPORT_GUARD_END()
