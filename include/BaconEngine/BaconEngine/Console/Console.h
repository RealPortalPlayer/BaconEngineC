// Purpose: Stores and manages commands.
// Created on: 8/7/22 @ 10:03 AM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "Command.h"

CPP_GUARD_START()
    BE_Command** GetCommands(void);
    int BE_GetCommandAmount(void);
    int BE_GetAllocatedCommandsAmount(void);
    int BE_GetCommandReallocationAmount(void);
    void BE_InitializeConsole(void);
    void BE_RegisterCommand(const char* name, const char* description, BE_CommandFlags flags, void (*Run)(BE_CommandContext context));
    void BE_DuplicateCommand(const char* name, const char* description);
    void BE_ExecuteCommand(char* input); // TODO: Client
    void BE_DestroyConsole(void);
CPP_GUARD_END()
