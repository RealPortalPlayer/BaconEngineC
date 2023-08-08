// Purpose: A basic command.
// Created on: 8/7/22 @ 10:05 AM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>
#include <SharedEngineCode/Internal/Boolean.h>
#include <SharedEngineCode/String.h>

#include "BaconEngine/Storage/DynamicDictionary.h"
#include "ArgumentManager.h"
#include "BaconEngine/BinaryExport.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
typedef enum {
    BE_COMMAND_FLAG_NULL,
    
    /**
     * Requires cheats enabled to run this command.
     * Clients can have cheats fake enabled, so don't trust any output from client-sided cheat commands
     */
    BE_COMMAND_FLAG_CHEATS_ONLY,
    
    /**
     * Can only be ran on the server, clients cannot run this command
     */
    BE_COMMAND_FLAG_SERVER_ONLY = (1 << 1),
    
    /**
     * Servers cannot run this command
     */
    BE_COMMAND_FLAG_CLIENT_ONLY = (1 << 2),
    
    /**
     * The command is ran on the server.
     * Useful for cheats only commands
     */
    BE_COMMAND_FLAGS_RAN_ON_SERVER = (1 << 3),
    
    /**
     * Don't apply backslash, or quote parsing.
     * Also disables any parsing errors
     */
    BE_COMMAND_FLAGS_NO_FANCY_ARGUMENT_PARSING = (1 << 4)
} BE_Command_Flags;

typedef struct { // TODO: Client
    const char* fullInput;
    const char* unparsedArguments;
    BE_DynamicDictionary arguments;
} BE_Command_Context;

typedef struct {
    char* name;
    SEC_Boolean required;
} BE_Command_Argument;

typedef struct {
    const char* name;
    const char* description;
    BE_DynamicArray arguments;
    BE_Command_Flags flags;
    void (*Run)(BE_Command_Context context);
} BE_Command;

BE_BINARYEXPORT void BE_Command_Register(const char* name, const char* description, BE_Command_Flags flags, void (*Run)(BE_Command_Context context));
BE_BINARYEXPORT void BE_Command_AddArgument(const char* name, SEC_Boolean required);

/**
 * Duplicates the previously added command
 * @param name Name for the duplicated command
 * @param description If this is NULL, then it will use the description of the previous command
 */
BE_BINARYEXPORT void BE_Command_DuplicatePrevious(const char* name, const char* description);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
