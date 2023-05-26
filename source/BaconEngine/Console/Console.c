// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stddef.h>
#include <string.h>

#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Console/Console.h"
#include "../InterfaceFunctions.h"

#ifndef BE_CLIENT_BINARY
#   include "EngineCommands.h"
#   include "PrivateConsole.h"
#   include "../Storage/PrivateDynamicArray.h"
#   include "../EngineMemory.h"
#   include "BaconEngine/Math/Bitwise.h"
#   include "../Storage/PrivateDynamicDictionary.h"
#endif

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static BE_DynamicArray beConsolePrivateCommands;
static BE_DynamicArray beConsoleCommands;
static BE_PrivateConsole_Command* beConsoleLastCommand;
static SEC_Boolean beConsoleDuplicateCommand = SEC_BOOLEAN_FALSE;
static SEC_Boolean beConsoleInitialized = SEC_BOOLEAN_FALSE;
static SEC_Boolean beConsoleAddingEngineCommands = SEC_BOOLEAN_TRUE;

extern void BE_EngineCommands_HelpPrint(BE_Command*);

static BE_PrivateConsole_Command* BE_Console_GetPrivateCommand(const char* name) {
    static BE_PrivateConsole_Command* cachedCommand = NULL;

    if (cachedCommand != NULL && strcmp(cachedCommand->publicCommand.name, name) == 0)
        return cachedCommand;

    for (int commandId = 0; commandId < beConsolePrivateCommands.used; commandId++) {
        BE_PrivateConsole_Command* foundCommand = BE_DYNAMICARRAY_GET_ELEMENT(BE_PrivateConsole_Command, beConsolePrivateCommands, commandId);

        if (strcmp(foundCommand->publicCommand.name, name) != 0)
            continue;

        cachedCommand = foundCommand;
        return foundCommand;
    }

    return NULL;
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
    return beConsoleCommands.calledRealloc;
#else
    BE_INTERFACEFUNCTION(int, void);
    return function();
#endif
}

#ifndef BE_CLIENT_BINARY
void BE_PrivateConsole_Initialize(void) {
    BE_ASSERT(!beConsoleInitialized, "Already initialized the console\n");
    SEC_LOGGER_INFO("Initializing console\n");

    beConsoleInitialized = SEC_BOOLEAN_TRUE;

    BE_PrivateDynamicArray_Create(&beConsoleCommands, 100);
    BE_PrivateDynamicArray_Create(&beConsolePrivateCommands, 100);

#   ifndef BE_NO_ENGINE_COMMANDS
    SEC_LOGGER_INFO("Registering engine commands\n");
    BE_EngineCommands_Initialize();
#   endif

    beConsoleAddingEngineCommands = SEC_BOOLEAN_FALSE;
}
#endif

void BE_Command_Register(const char* name, const char* description, BE_Command_Flags flags, void (*Run)(BE_Command_Context context)) {
#ifndef BE_CLIENT_BINARY
    for (int i = 0; i < (int) beConsoleCommands.used; i++)
        BE_STRICTMODE_CHECK_NO_RETURN_VALUE(strcmp(BE_DYNAMICARRAY_GET_ELEMENT(BE_Command, beConsoleCommands, i)->name, name) != 0,
                            "The command '%s' is already registered\n", name);

    if (!beConsoleDuplicateCommand)
        SEC_LOGGER_TRACE("Registering command\n"
                         "Name: %s\n"
                         "Description: %s\n"
                         "Flags: %i\n"
                         "Spot: %i/%zu\n", name, description, flags, beConsoleCommands.used, beConsoleCommands.size);

    if (flags != BE_COMMAND_FLAG_NULL) {
        BE_STRICTMODE_CHECK_NO_RETURN_VALUE(!BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAG_SERVER_ONLY) || !BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAG_CLIENT_ONLY),
                                            "Invalid command flags, cannot be both for server and client only\n");
        BE_STRICTMODE_CHECK_NO_RETURN_VALUE(!BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAG_CHEATS_ONLY) || !BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAG_CLIENT_ONLY),
                                            "Invalid command flags, the client cannot run any cheat commands\n");
        BE_STRICTMODE_CHECK_NO_RETURN_VALUE(!BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAG_CLIENT_ONLY) || !BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAGS_RAN_ON_SERVER),
                                            "Invalid command flags, cannot be both client only but also only runs on the server\n");

        if (BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAG_SERVER_ONLY) && BE_BITWISE_IS_BIT_SET(flags, BE_COMMAND_FLAGS_RAN_ON_SERVER))
            SEC_LOGGER_WARN("Redundant command flags, server commands will only run on servers\n");
    }

    BE_PrivateConsole_Command* privateConsoleCommand = (BE_PrivateConsole_Command*) BE_EngineMemory_AllocateMemory(sizeof(BE_PrivateConsole_Command), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

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

    BE_DynamicArray_AddElementToLast(&beConsolePrivateCommands, (void*) privateConsoleCommand);
    BE_DynamicArray_AddElementToLast(&beConsoleCommands, (void*) &privateConsoleCommand->publicCommand);

    beConsoleDuplicateCommand = SEC_BOOLEAN_FALSE;
#else
    BE_INTERFACEFUNCTION(void, const char*, const char*, BE_Command_Flags, void (*)(BE_Command_Context))(name, description, flags, Run);
#endif
}

void BE_Command_AddArgument(const char* name, SEC_Boolean required) {
#ifndef BE_CLIENT_BINARY
    BE_STRICTMODE_CHECK_NO_RETURN_VALUE(beConsoleCommands.used != 0, "There is no command to add arguments to\n");

    BE_Command* command = BE_DYNAMICARRAY_GET_LAST_ELEMENT(BE_Command, beConsoleCommands);

    BE_STRICTMODE_CHECK_NO_RETURN_VALUE(command->arguments.used == 0 ||
                                        BE_DYNAMICARRAY_GET_LAST_ELEMENT(BE_Command_Argument, command->arguments)->required ||
                                        !required, "Required arguments cannot be added after an optional argument\n");

    BE_Command_Argument* argument = BE_EngineMemory_AllocateMemory(sizeof(BE_Command_Argument), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

    argument->name = name;
    argument->required = required;

    BE_DynamicArray_AddElementToLast(&command->arguments, (void*) argument);
#else
    BE_INTERFACEFUNCTION(void, const char*, SEC_Boolean)(name, required);
#endif
}

void BE_Command_DuplicatePrevious(const char* name, const char* description) {
#ifndef BE_CLIENT_BINARY
    BE_STRICTMODE_CHECK_NO_RETURN_VALUE(beConsoleCommands.used != 0, "There is no command to duplicate\n");
    SEC_LOGGER_TRACE("Duplicating command\n"
                     "Original name: %s\n"
                     "Original description: %s\n"
                     "New name: %s\n"
                     "New description: %s\n", beConsoleLastCommand->publicCommand.name, beConsoleLastCommand->publicCommand.description, name,
                     description != NULL ? description : beConsoleLastCommand->publicCommand.description);

    beConsoleDuplicateCommand = SEC_BOOLEAN_TRUE;

    BE_PrivateConsole_Command* lastCommand = beConsoleLastCommand;

    BE_Command_Register(name, description != NULL ? description : lastCommand->publicCommand.description, lastCommand->publicCommand.flags,
                        lastCommand->publicCommand.Run);
#else
    BE_INTERFACEFUNCTION(void, const char*, const char*)(name, description);
#endif
}

void BE_Console_ExecuteCommand(const char* input) { // TODO: Client
#ifndef BE_CLIENT_BINARY
    // Do not put any logs outside an if check.
    // We do not want to trick users into thinking something is part of the command.

    if (input == NULL || input[0] == '\0')
        return;

    char name[BE_COMMAND_MAX_NAME_LENGTH];
    BE_Command* command = NULL;
    BE_DynamicDictionary arguments;
    size_t inputLength = strlen(input);

    BE_PrivateDynamicDictionary_Create(&arguments, 20);
    memset(name, 0, BE_COMMAND_MAX_NAME_LENGTH);

    int index;
    int argumentStartingIndex;

    {
        SEC_Boolean trimmed = SEC_BOOLEAN_FALSE;
        int writer = 0;

        for (index = 0; index < (int) inputLength; index++) {
            if (input[index] == ' ') {
                if (!trimmed)
                    continue;

                index++;
                break;
            }

            if (index >= BE_COMMAND_MAX_NAME_LENGTH)
                break;

            trimmed = SEC_BOOLEAN_TRUE;
            name[writer++] = input[index];
        }
    }

    command = BE_Console_GetCommand(name);
    argumentStartingIndex = index;

    if (command == NULL) { // TODO: Tell the client.
        SEC_LOGGER_ERROR("'%s' is not a valid command\n", name);
        goto destroy;
    }

    if (command->arguments.used != 0) {
        // TODO: This is dumb, figure out a way to do this without allocating memory.
        // TODO: Make the argument max length constant.
        char* argument = (char*) BE_EngineMemory_AllocateMemory(sizeof(char) * 1024, BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
        int current = 0;
        int quotePosition = -1;
        SEC_Boolean doubleQuote = SEC_BOOLEAN_FALSE;
        SEC_Boolean escaped = SEC_BOOLEAN_FALSE;
        SEC_Boolean added = SEC_BOOLEAN_FALSE;
        SEC_Boolean trimmed = SEC_BOOLEAN_FALSE;
        SEC_Boolean quoteAdded = SEC_BOOLEAN_FALSE;

        memset(argument, 0, 1024);

        for (int writer = 0; index < (int) inputLength && current < command->arguments.used; index++) {
            if (added) {
                argument = (char*) BE_EngineMemory_AllocateMemory(sizeof(char) * 1024, BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

                memset(argument, 0, 1024);
            }

            added = SEC_BOOLEAN_FALSE;

            if (quoteAdded && input[index] == ' ')
                continue;

            quoteAdded = SEC_BOOLEAN_FALSE;

            if (writer >= 1024) {
                publish_argument:
                writer = 0;

                BE_DynamicDictionary_AddElementToLast(&arguments, (void*) BE_DYNAMICARRAY_GET_ELEMENT(BE_Command_Argument, command->arguments, current++)->name, argument);

                added = SEC_BOOLEAN_TRUE;
                trimmed = SEC_BOOLEAN_FALSE;
                continue;
            }

            if (input[index] == ' ' && trimmed && quotePosition == -1 && !escaped)
                goto publish_argument;

            if (input[index] == '\\' && !escaped) {
                escaped = SEC_BOOLEAN_TRUE;
                continue;
            }

            if (input[index] == '\'' && !escaped) {
                if (quotePosition == -1) {
                    quotePosition = index;
                    continue;
                }

                if (!doubleQuote) {
                    quotePosition = -1;
                    quoteAdded = SEC_BOOLEAN_TRUE;

                    goto publish_argument;
                }
            }

            if (input[index] == '"' && !escaped) {
                if (quotePosition == -1) {
                    quotePosition = index;
                    doubleQuote = SEC_BOOLEAN_TRUE;
                    continue;
                }

                if (doubleQuote) {
                    quotePosition = -1;
                    doubleQuote = SEC_BOOLEAN_FALSE;
                    quoteAdded = SEC_BOOLEAN_TRUE;

                    goto publish_argument;
                }
            }

            trimmed = SEC_BOOLEAN_TRUE;
            argument[writer++] = input[index];
            // TODO: Should we throw a parse error if someone tries to escape a normal character, or should we just assume they meant to use a backslash
            escaped = SEC_BOOLEAN_FALSE;
        }

        // FIXME: This doesn't trigger at times when it probably should
        if (quotePosition != -1) {
            SEC_LOGGER_ERROR("Parsing error: unescaped %s quote at %i\n", doubleQuote ? "double" : "single", quotePosition);
            goto destroy;
        }

        if (escaped) {
            SEC_LOGGER_ERROR("Parsing error: stray escape character\n");
            goto destroy;
        }

        if (!added) {
            if (argument[0] != '\0') {
                BE_DynamicDictionary_AddElementToLast(&arguments, (void*) BE_DYNAMICARRAY_GET_ELEMENT(BE_Command_Argument, command->arguments, current)->name, argument);

                added = SEC_BOOLEAN_TRUE;
            }

            if (!added)
                BE_EngineMemory_DeallocateMemory(argument, sizeof(char) * 1024, BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
        }
    }

    if (BE_BITWISE_IS_BIT_SET(command->flags, BE_COMMAND_FLAG_SERVER_ONLY)) {
        // TODO: Kick client.

        if (!BE_ClientInformation_IsServerModeEnabled()) {
            SEC_LOGGER_ERROR("This command can only be ran by the server\n");
            goto destroy;
        }
    }

    if (BE_BITWISE_IS_BIT_SET(command->flags, BE_COMMAND_FLAG_CLIENT_ONLY) && BE_ClientInformation_IsServerModeEnabled()) {
        SEC_LOGGER_ERROR("This command can only be ran by a client\n"); // TODO: Kick client if the client is the one who ran it.
        goto destroy;
    }

    if ((command->flags & BE_COMMAND_FLAG_CHEATS_ONLY) != 0 && !BE_ClientInformation_IsCheatsEnabled()) {
        // TODO: Tell client.
        SEC_LOGGER_ERROR("This command requires cheats to be enabled\n");
        goto destroy;
    }

    if (command->arguments.used != 0) {
        int requiredArguments = 0;

        for (int i = 0; i < command->arguments.used; i++) {
            if (!BE_DYNAMICARRAY_GET_ELEMENT(BE_Command_Argument, command->arguments, i)->required)
                break;

            requiredArguments++;
        }

        if (requiredArguments > arguments.keys.used) {
            // TODO: Run help command automatically
            SEC_LOGGER_ERROR("Help: (not enough arguments)\n"
                             "    %s Command:\n", BE_Console_IsEngineCommand(*command) ? "Engine" : "Client");
            BE_EngineCommands_HelpPrint(command);
            goto destroy;
        }
    }

    command->Run(SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Command_Context, input, input + argumentStartingIndex, arguments));

    destroy:
    for (int argumentId = 0; argumentId < arguments.keys.used; argumentId++)
        BE_EngineMemory_DeallocateMemory(arguments.values.internalArray[argumentId], sizeof(char) * 1024, BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

    BE_EngineMemory_DeallocateMemory(arguments.keys.internalArray, sizeof(void*) * arguments.keys.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
    BE_EngineMemory_DeallocateMemory(arguments.values.internalArray, sizeof(void*) * arguments.keys.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
#else
    BE_INTERFACEFUNCTION(void, const char*)(input);
#endif
}

SEC_Boolean BE_Console_IsEngineCommand(BE_Command command) {
#ifndef BE_CLIENT_BINARY
    return BE_Console_GetPrivateCommand(command.name)->engineCommand;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_Command);
    return function(command);
#endif
}

#ifndef BE_CLIENT_BINARY
void BE_PrivateConsole_Destroy(void) {
    BE_ASSERT(beConsoleInitialized, "Console are not initialized\n");
    SEC_LOGGER_INFO("Destroying console\n");

    beConsoleInitialized = SEC_BOOLEAN_FALSE;

    for (int commandId = 0; commandId < beConsoleCommands.used; commandId++) {
        BE_PrivateConsole_Command* command = BE_DYNAMICARRAY_GET_ELEMENT(BE_PrivateConsole_Command, beConsoleCommands, commandId);

        for (int argumentId = 0; argumentId < command->publicCommand.arguments.used; argumentId++)
            BE_EngineMemory_DeallocateMemory(command->publicCommand.arguments.internalArray[argumentId], sizeof(BE_Command_Argument),
                                             BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

        if (!command->duplicate)
            BE_EngineMemory_DeallocateMemory(command->publicCommand.arguments.internalArray, sizeof(void*) * command->publicCommand.arguments.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);

        BE_EngineMemory_DeallocateMemory(command, sizeof(BE_PrivateConsole_Command), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
    }

    BE_EngineMemory_DeallocateMemory(beConsolePrivateCommands.internalArray, sizeof(void*) * beConsolePrivateCommands.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
    BE_EngineMemory_DeallocateMemory(beConsoleCommands.internalArray, sizeof(void*) * beConsoleCommands.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
}
#endif
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
