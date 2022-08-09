// Purpose: A basic command.
// Created on: 8/7/22 @ 10:05 AM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "ArgumentManager.h"

CPP_GUARD_START()
    typedef enum {
        COMMAND_FLAG_NULL,
        COMMAND_FLAG_CHEATS_ONLY,
        COMMAND_FLAG_SERVER_ONLY = (1 << 1),
        COMMAND_FLAG_CLIENT_ONLY = (1 << 2)
    } CommandFlags;

    typedef struct { // TODO: Client
        const char** userArguments;
        ArgumentsDictionary arguments;
    } CommandContext;

    typedef struct {
        const char* name;
        const char* description;
        CommandFlags flags;
        void (*Run)(CommandContext context);
    } Command;
CPP_GUARD_END()
