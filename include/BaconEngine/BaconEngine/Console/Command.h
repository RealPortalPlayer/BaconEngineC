// Purpose: Information for the commands.
// Created on: 7/29/22 @ 9:27 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Networking/Client.h"
#include "ArgumentManager.h"
#include "CommandArguments.h"

#define MAX_COMMANDS 1000

CPP_GUARD_START()
    struct Command;

    typedef struct {
        ConnectedClient* client;
        Arguments arguments;
    } CommandContext;

    typedef struct Command {
        const char* name;
        const char* description;
        CommandArguments arguments;
        void (*Run)(struct Command this, CommandContext context);
    } Command;
CPP_GUARD_END()