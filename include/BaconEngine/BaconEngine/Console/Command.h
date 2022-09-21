// Purpose: A basic command.
// Created on: 8/7/22 @ 10:05 AM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Storage/DynamicDictionary.h"
#include "ArgumentManager.h"

SEC_CPP_GUARD_START()
    typedef enum {
        BE_COMMAND_FLAG_NULL,
        BE_COMMAND_FLAG_CHEATS_ONLY,
        BE_COMMAND_FLAG_SERVER_ONLY = (1 << 1),
        BE_COMMAND_FLAG_CLIENT_ONLY = (1 << 2)
    } BE_Command_Flags;

    typedef struct { // TODO: Client
        const char** userArguments;
        BE_DynamicDictionary arguments;
    } BE_Command_Context;

    typedef struct {
        const char* name;
        const char* description;
        BE_Command_Flags flags;
        void (*Run)(BE_Command_Context context);
    } BE_Command;
SEC_CPP_GUARD_END()
