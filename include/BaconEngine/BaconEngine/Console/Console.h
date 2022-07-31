// Purpose: Stores and manages commands.
// Created on: 7/30/22 @ 12:31 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "Command.h"
#include "CommandArguments.h"

CPP_GUARD_START() // TODO: Console logs.
    int GetCommandAmount(void);
    Command* GetCommands(void);
    void RegisterCommand(const char* name, const char* description, CommandArguments arguments, void (*Run)(Command this, CommandContext context));
    void ExecuteCommand(const char* name, const char* arguments[MAX_ARGUMENTS]);
CPP_GUARD_END()