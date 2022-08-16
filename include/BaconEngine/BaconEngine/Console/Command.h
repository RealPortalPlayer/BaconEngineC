// Purpose: A basic command.
// Created on: 8/7/22 @ 10:05 AM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "ArgumentManager.h"

CPP_GUARD_START()
    typedef enum {
        BE_COMMAND_FLAG_NULL,
        BE_COMMAND_FLAG_CHEATS_ONLY,
        BE_COMMAND_FLAG_SERVER_ONLY = (1 << 1),
        BE_COMMAND_FLAG_CLIENT_ONLY = (1 << 2)
    } BE_CommandFlags;

    typedef struct { // TODO: Client
        const char** userArguments;
        ArgumentsDictionary arguments;
    } BE_CommandContext;

    typedef struct {
        const char* name;
        const char* description;
        BE_CommandFlags flags;
        void (*Run)(BE_CommandContext context);
    } BE_Command;
CPP_GUARD_END()
