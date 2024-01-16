// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stddef.h>
#include <string.h>
#include <SharedEngineCode/Debugging/StrictMode.h>
#include <BaconAPI/Debugging/Assert.h>

#include "BaconEngine/Console/Console.h"
#include "../InterfaceFunctions.h"
#include "BaconEngine/Math/Bitwise.h"
#include "BaconEngine/ClientInformation.h"

#ifndef BE_CLIENT_BINARY
#   include "EngineCommands.h"
#   include "PrivateConsole.h"
#   include "../Storage/PrivateDynamicArray.h"
#   include "../EngineMemory.h"
#   include "../Storage/PrivateDynamicDictionary.h"
#   include "../PrivateArgumentManager.h"
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static BA_DynamicArray beConsolePrivateCommands;
static BA_DynamicArray beConsoleCommands;
static BE_PrivateConsole_Command* beConsoleLastCommand;
static BA_Boolean beConsoleDuplicateCommand = BA_BOOLEAN_FALSE;
static BA_Boolean beConsoleInitialized = BA_BOOLEAN_FALSE;
static BA_Boolean beConsoleAddingEngineCommands = BA_BOOLEAN_TRUE;

extern void BE_EngineCommands_HelpPrint(BE_Command*);

static BE_PrivateConsole_Command* BE_Console_GetPrivateCommand(const char* name) {
    static BE_PrivateConsole_Command* cachedCommand = NULL;

    if (cachedCommand != NULL && strcmp(cachedCommand->publicCommand.name, name) == 0)
        return cachedCommand;

    for (int commandId = 0; commandId < beConsolePrivateCommands.used; commandId++) {
        BE_PrivateConsole_Command* foundCommand = BA_DYNAMICARRAY_GET_ELEMENT(BE_PrivateConsole_Command, beConsolePrivateCommands, commandId);

        if (strcmp(foundCommand->publicCommand.name, name) != 0)
            continue;

        cachedCommand = foundCommand;
        return foundCommand;
    }

    return NULL;
}

BA_Boolean BE_Console_CheckRegisterLogsEnabled(void) {
#   if BA_ALLOW_DEBUG_LOGS
    static int enabled = -1;

    if (enabled == -1)
        enabled = !BA_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_DONT_PRINT_COMMAND_REGISTER, SEC_BUILTINARGUMENTS_DONT_PRINT_COMMAND_REGISTER_SHORT, BA_BOOLEAN_FALSE);
    
    return enabled;
#   else
    return BA_BOOLEAN_FALSE;
#   endif
}
#endif

BE_Command** BE_Console_GetCommands(void) {
#ifndef BE_CLIENT_BINARY
    return (BE_Command**) beConsoleCommands.internalArray;
#else
    BE_INTERFACEFUNCTION(BE_Command**, void);
    return function();
#endif
}

BE_Command* BE_Console_GetCommand(const char* name) {
#ifndef BE_CLIENT_BINARY
    BE_PrivateConsole_Command* privateCommand = BE_Console_GetPrivateCommand(name);

    return privateCommand != NULL ? &privateCommand->publicCommand : NULL;
#else
    BE_INTERFACEFUNCTION(BE_Command*, const char*);
    return function(name);
#endif
}

int BE_Console_GetCommandAmount(void) {
#ifndef BE_CLIENT_BINARY
    return beConsoleCommands.used;
#else
    BE_INTERFACEFUNCTION(int, void);
    return function();
#endif
}

int BE_Console_GetAllocatedCommandsAmount(void) {
#ifndef BE_CLIENT_BINARY
    return (int) beConsoleCommands.size;
#else
    BE_INTERFACEFUNCTION(int, void);
    return function();
#endif
}

int BE_Console_GetCommandReallocationAmount(void) {
#ifndef BE_CLIENT_BINARY
    return beConsoleCommands.calledReallocate;
#else
    BE_INTERFACEFUNCTION(int, void);
    return function();
#endif
}

#ifndef BE_CLIENT_BINARY
void BE_PrivateConsole_Initialize(void) {
    BA_ASSERT(!beConsoleInitialized, "Already initialized the console\n");
    BA_LOGGER_INFO("Initializing console\n");

    beConsoleInitialized = BA_BOOLEAN_TRUE;

    BE_PrivateDynamicArray_Create(&beConsoleCommands, 100);
    BE_PrivateDynamicArray_Create(&beConsolePrivateCommands, 100);

#   ifndef BE_NO_ENGINE_COMMANDS
    BA_LOGGER_INFO("Registering engine commands\n");
    BE_EngineCommands_Initialize();
#   endif

    beConsoleAddingEngineCommands = BA_BOOLEAN_FALSE;
}
#endif

void BE_Command_Register(const char* name, const char* description, BE_Command_Flags flags, void (*Run)(BE_Command_Context context)) {
#ifndef BE_CLIENT_BINARY
    for (int i = 0; i < (int) beConsoleCommands.used; i++)
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(strcmp(BA_DYNAMICARRAY_GET_ELEMENT(BE_Command, beConsoleCommands, i)->name, name) != 0,
                                             "The command '%s' is already registered\n", name);

    if (!beConsoleDuplicateCommand && BE_Console_CheckRegisterLogsEnabled())
        BA_LOGGER_TRACE("Registering command\n"
                         "Name: %s\n"
                         "Description: %s\n"
                         "Flags: %i\n"
                         "Spot: %i/%zu\n", name, description, flags, beConsoleCommands.used, beConsoleCommands.size);

    if (flags != BE_COMMAND_FLAG_NULL) {
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(!BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAG_SERVER_ONLY) || !BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAG_CLIENT_ONLY),
                                             "Invalid command flags, cannot be both for server and client only\n");
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(!BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAG_CHEATS_ONLY) || !BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAG_CLIENT_ONLY),
                                             "Invalid command flags, the client cannot run any cheat commands\n");
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(!BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAG_CLIENT_ONLY) || !BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAGS_RAN_ON_SERVER),
                                             "Invalid command flags, cannot be both client only but also only runs on the server\n");

        if (BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAG_SERVER_ONLY) && BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAGS_RAN_ON_SERVER))
            BA_LOGGER_WARN("Redundant command flags, server commands will only run on servers\n");
    }

    BE_PrivateConsole_Command* privateConsoleCommand = (BE_PrivateConsole_Command*) BE_EngineMemory_AllocateMemory(sizeof(BE_PrivateConsole_Command), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_COMMAND);

    if (!beConsoleDuplicateCommand)
        BE_PrivateDynamicArray_Create(&privateConsoleCommand->publicCommand.arguments, 10);
    else
        privateConsoleCommand->publicCommand.arguments = beConsoleLastCommand->publicCommand.arguments;

    privateConsoleCommand->engineCommand = beConsoleAddingEngineCommands;
    privateConsoleCommand->duplicate = beConsoleDuplicateCommand;
    privateConsoleCommand->publicCommand.name = name;
    privateConsoleCommand->publicCommand.description = description;
    privateConsoleCommand->publicCommand.flags = flags;
    privateConsoleCommand->publicCommand.Run = Run;
    beConsoleLastCommand = privateConsoleCommand;

    BE_PrivateDynamicArray_CheckResize(&beConsolePrivateCommands);
    BA_DynamicArray_AddElementToLast(&beConsolePrivateCommands, (void*) privateConsoleCommand);
    BE_PrivateDynamicArray_CheckResize(&beConsolePrivateCommands);
    BA_DynamicArray_AddElementToLast(&beConsoleCommands, (void*) &privateConsoleCommand->publicCommand);

    beConsoleDuplicateCommand = BA_BOOLEAN_FALSE;
#else
    BE_INTERFACEFUNCTION(void, const char*, const char*, BE_Command_Flags, void (*)(BE_Command_Context))(name, description, flags, Run);
#endif
}

void BE_Command_AddArgument(const char* name, BA_Boolean required) {
#ifndef BE_CLIENT_BINARY
    // TODO: Packets can use this code, too. Perhaps it can be moved to its own function
    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(beConsoleCommands.used != 0, "There is no command to add arguments to\n");

    BE_PrivateConsole_Command* command = BA_DYNAMICARRAY_GET_LAST_ELEMENT(BE_PrivateConsole_Command, beConsolePrivateCommands);

    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(command->publicCommand.arguments.used == 0 ||
                                         BA_DYNAMICARRAY_GET_LAST_ELEMENT(BE_ArgumentManager_Argument, command->publicCommand.arguments)->required ||
                                         !required, "Required arguments cannot be added after an optional argument\n");

    BE_ArgumentManager_Argument* argument = BE_EngineMemory_AllocateMemory(sizeof(BE_ArgumentManager_Argument), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_ARGUMENT_MANAGER_ARGUMENT);

    argument->name = BA_String_Copy(name);
    argument->required = required;

    if (required)
        command->requiredArgumentCount++;

    BE_PrivateDynamicArray_CheckResize(&command->publicCommand.arguments);
    BA_DynamicArray_AddElementToLast(&command->publicCommand.arguments, (void*) argument);
#else
    BE_INTERFACEFUNCTION(void, const char*, BA_Boolean)(name, required);
#endif
}

void BE_Command_DuplicatePrevious(const char* name, const char* description) {
#ifndef BE_CLIENT_BINARY
    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(beConsoleCommands.used != 0, "There is no command to duplicate\n");

    if (BE_Console_CheckRegisterLogsEnabled())
        BA_LOGGER_TRACE("Duplicating command\n"
                         "Original name: %s\n"
                         "Original description: %s\n"
                         "New name: %s\n"
                         "New description: %s\n", beConsoleLastCommand->publicCommand.name, beConsoleLastCommand->publicCommand.description, name,
                         description != NULL ? description : beConsoleLastCommand->publicCommand.description);

    beConsoleDuplicateCommand = BA_BOOLEAN_TRUE;

    BE_PrivateConsole_Command* lastCommand = beConsoleLastCommand;

    BE_Command_Register(name, description != NULL ? description : lastCommand->publicCommand.description, lastCommand->publicCommand.flags,
                        lastCommand->publicCommand.Run);
#else
    BE_INTERFACEFUNCTION(void, const char*, const char*)(name, description);
#endif
}

void BE_Console_ExecuteCommand(const char* input, BE_Client client) {
#ifndef BE_CLIENT_BINARY
    // Do not put any logs outside an if check.
    // We do not want to trick users into thinking something is part of the command.

    if (input == NULL || input[0] == '\0')
        return;

    char* name;
    BA_DynamicDictionary arguments;
    int argumentStartingIndex;

    BE_PrivateArgumentManager_ParseName(input, &name, &argumentStartingIndex);

    BE_PrivateConsole_Command* command = BE_Console_GetPrivateCommand(name);
    
    if (command == NULL) { // TODO: Tell the client.
        BA_LOGGER_ERROR("'%s' is not a valid command\n", name);
        goto destroy;
    }

    BE_PrivateDynamicDictionary_Create(&arguments, 20);

    if (command->publicCommand.arguments.used != 0 && !BE_BITWISE_IS_BIT_SET(command->publicCommand.flags, BE_COMMAND_FLAGS_NO_ARGUMENT_PARSING) && !BE_PrivateArgumentManager_ParseArguments(input, argumentStartingIndex, !BE_BITWISE_IS_BIT_SET(command->publicCommand.flags, BE_COMMAND_FLAGS_NO_FANCY_ARGUMENT_PARSING), command->publicCommand.arguments, &arguments))
        goto destroy;

    if (BE_BITWISE_IS_BIT_SET(command->publicCommand.flags, BE_COMMAND_FLAGS_RAN_ON_SERVER) && !BE_ClientInformation_IsServerModeEnabled()) {
        // TODO: Send command packet to server
        goto destroy;
    }

    if (BE_BITWISE_IS_BIT_SET(command->publicCommand.flags, BE_COMMAND_FLAG_SERVER_ONLY)) {
        // TODO: Kick client.

        if (!BE_ClientInformation_IsServerModeEnabled()) {
            BA_LOGGER_ERROR("This command can only be ran by the server\n");
            goto destroy;
        } else if (client != BE_CLIENT_UNCONNECTED) {
            // TODO: Kick client: invalid packet (client doesn't send a command packet if command is server only)
            goto destroy;
        }
    }

    if (BE_BITWISE_IS_BIT_SET(command->publicCommand.flags, BE_COMMAND_FLAG_CLIENT_ONLY) && BE_ClientInformation_IsServerModeEnabled()) {
        if (client != BE_CLIENT_UNCONNECTED) {
            // TODO: Kick client: invalid packet (client should never **ever** send a command packet if command is client only)
            goto destroy;
        }

        BA_LOGGER_ERROR("This command can only be ran by a client\n");
        goto destroy;
    }

    if ((command->publicCommand.flags & BE_COMMAND_FLAG_CHEATS_ONLY) != 0 && !BE_ClientInformation_IsCheatsEnabled()) {
        // TODO: Tell client.
        BA_LOGGER_ERROR("This command requires cheats to be enabled\n");
        goto destroy;
    }

    if (command->requiredArgumentCount > arguments.keys.used) {
        BA_LOGGER_ERROR("Help: (not enough arguments)\n"
                         "    %s Command:\n", command->engineCommand ? "Engine" : "Client");
        BE_EngineCommands_HelpPrint(&command->publicCommand);
        goto destroy;
    }

    command->publicCommand.Run(BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Command_Context, input, input + argumentStartingIndex, arguments));

    destroy:
    BE_EngineMemory_DeallocateMemory(name, sizeof(char) * (strlen(name) + 1), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_ARGUMENT_MANAGER_NAME);
    
    for (int argumentId = 0; argumentId < arguments.keys.used; argumentId++)
        BE_EngineMemory_DeallocateMemory(arguments.values.internalArray[argumentId], sizeof(char) * (strlen(arguments.values.internalArray[argumentId]) + 1), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_ARGUMENT_MANAGER_TEMPORARY_ARGUMENT);

    BE_EngineMemory_DeallocateMemory(arguments.keys.internalArray, sizeof(void*) * arguments.keys.size, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY);
    BE_EngineMemory_DeallocateMemory(arguments.values.internalArray, sizeof(void*) * arguments.keys.size, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY);
#else
    BE_INTERFACEFUNCTION(void, const char*)(input);
#endif
}

BA_Boolean BE_Console_IsEngineCommand(BE_Command command) {
#ifndef BE_CLIENT_BINARY
    return BE_Console_GetPrivateCommand(command.name)->engineCommand;
#else
    BE_INTERFACEFUNCTION(BA_Boolean, BE_Command);
    return function(command);
#endif
}

#ifndef BE_CLIENT_BINARY
void BE_PrivateConsole_Destroy(void) {
    BA_ASSERT(beConsoleInitialized, "Console is not initialized\n");
    BA_LOGGER_INFO("Destroying console\n");

    beConsoleInitialized = BA_BOOLEAN_FALSE;

    for (int commandId = 0; commandId < beConsoleCommands.used; commandId++) {
        BE_PrivateConsole_Command* command = BA_DYNAMICARRAY_GET_ELEMENT(BE_PrivateConsole_Command, beConsoleCommands, commandId);

        for (int argumentId = 0; argumentId < command->publicCommand.arguments.used; argumentId++)
            BE_EngineMemory_DeallocateMemory(command->publicCommand.arguments.internalArray[argumentId], sizeof(BE_ArgumentManager_Argument),
                                             BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_ARGUMENT_MANAGER_ARGUMENT);

        if (!command->duplicate)
            BE_EngineMemory_DeallocateMemory(command->publicCommand.arguments.internalArray, sizeof(void*) * command->publicCommand.arguments.size, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY);

        BE_EngineMemory_DeallocateMemory(command, sizeof(BE_PrivateConsole_Command), BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_COMMAND);
    }

    BE_EngineMemory_DeallocateMemory(beConsolePrivateCommands.internalArray, sizeof(void*) * beConsolePrivateCommands.size, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY);
    BE_EngineMemory_DeallocateMemory(beConsoleCommands.internalArray, sizeof(void*) * beConsoleCommands.size, BE_ENGINEMEMORYINFORMATION_MEMORY_TYPE_DYNAMIC_ARRAY);
}
#endif

BE_BINARYEXPORT void BE_Console_ExecuteFile(FILE* file) {
#ifndef BE_CLIENT_BINARY
    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(file != NULL, "File cannot be null\n");
    BA_LOGGER_TRACE("Executing list of commands\n");
    
    char* line;
    ssize_t length;
    
    while ((length = BA_String_GetLine(file, &line, NULL)) != -1) {
        if (length == -2) {
            BA_LOGGER_TRACE("Failed to allocate enough memory for buffer\n");
            return;
        }

        if (length == 0) {
            free(line);
            continue;
        }

        BE_Console_ExecuteCommand(line, BE_CLIENT_UNCONNECTED);
        free(line);
    }
#else
    BE_INTERFACEFUNCTION(void, FILE*);
    function(file);
#endif
}

BE_BINARYEXPORT void BE_Console_ExecuteFileContents(const char* contents) {
#ifndef BE_CLIENT_BINARY
    BA_LOGGER_TRACE("Executing list of commands\n");
    
    BA_DynamicArray* commandLines = BA_String_Split(contents, "\n");
    
    if (commandLines == NULL) {
        BA_LOGGER_TRACE("Failed to allocate enough memory for split buffer\n");
        return;
    }
    
    for (int i = 0; i < commandLines->used; i++) {
        char* line = BA_DYNAMICARRAY_GET_ELEMENT_POINTER(char, commandLines, i);

        if (strlen(line) == 0) {
            free(commandLines->internalArray[i]);
            continue;
        }

        BE_Console_ExecuteCommand(line, BE_CLIENT_UNCONNECTED);
        free(commandLines->internalArray[i]);
    }

    free(commandLines->internalArray);
    free(commandLines);
#else
    BE_INTERFACEFUNCTION(void, const char*);
    function(contents);
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
