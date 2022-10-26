// Purpose: A basic command.
// Created on: 8/7/22 @ 10:05 AM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "BaconEngine/Storage/DynamicDictionary.h"
#include "ArgumentManager.h"
#include "BaconEngine/Server/Client.h"

// TODO: Validate that none of the commands go over this limit.

#define BE_COMMAND_MAX_NAME_LENGTH 1024

SEC_CPP_SUPPORT_GUARD_START()
typedef enum {
    BE_COMMAND_FLAG_NULL,
    BE_COMMAND_FLAG_CHEATS_ONLY,
    BE_COMMAND_FLAG_SERVER_ONLY = (1 << 1),
    BE_COMMAND_FLAG_CLIENT_ONLY = (1 << 2),
    BE_COMMAND_FLAGS_RAN_ON_SERVER = (1 << 3)
} BE_Command_Flags;

typedef struct { // TODO: Client
    const char* fullInput;
    const char* unparsedArguments;
    BE_DynamicDictionary arguments;

    /**
     * The client who ran the command.
     * NULL when either the command was ran by the server, or the command is client only.
     */
    BE_Client_Connected* client;
} BE_Command_Context;

typedef struct {
    const char* name;
    int required;
} BE_Command_Argument;

typedef struct {
    const char* name;
    const char* description;
    BE_DynamicArray arguments;
    BE_Command_Flags flags;
    void (*Run)(BE_Command_Context context);
} BE_Command;

/**
 * @return The ID for the command, -1 if failed to add.
 */
void BE_Command_Register(const char* name, const char* description, BE_Command_Flags flags, void (*Run)(BE_Command_Context context));
void BE_Command_AddArgument(const char* name, int required);
void BE_Command_DuplicatePrevious(const char* name, const char* description);
SEC_CPP_SUPPORT_GUARD_END()
