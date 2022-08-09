// Purpose: Stores and manages commands.
// Created on: 8/7/22 @ 10:03 AM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "Command.h"

CPP_GUARD_START()
    Command** GetCommands(void);
    int GetCommandsAmount(void);
    int GetAllocatedCommandsAmount(void);
    int GetCommandReallocationAmount(void);
    void InitializeConsole(void);
    void RegisterCommand(const char* name, const char* description, CommandFlags flags, void (*Run)(CommandContext context));
    void DuplicateCommand(const char* name, const char* description);
    void ExecuteCommand(char* input); // TODO: Client
CPP_GUARD_END()
