#include <stddef.h>
#include <string.h>

#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Storage/DynamicArray.h"
#include "../EngineMemory.h"
#include "BaconEngine/Console/Console.h"
#include "EngineCommands.h"

SEC_CPP_SUPPORT_GUARD_START()
BE_DynamicArray beConsoleCommands;
BE_Command* beConsoleLastCommand;
int beConsoleDuplicateCommand = 0;
int beConsoleInitialized = 0;
int beConsoleClientCommandStart = 0;

BE_Command** BE_Console_GetCommands(void) {
    return (BE_Command**) beConsoleCommands.internalArray;
}

BE_Command* BE_Console_GetCommand(const char* name) {
    static BE_Command* cachedCommand = NULL;

    if (cachedCommand != NULL && strcmp(cachedCommand->name, name) == 0)
        return cachedCommand;

    for (int i = 0; i < (int) beConsoleCommands.used; i++) {
        BE_Command* command = BE_DYNAMICARRAY_GET_ELEMENT(BE_Command, beConsoleCommands, i);

        if (strcmp(command->name, name) != 0)
            continue;

        command->index = i;
        cachedCommand = command;
        return command;
    }

    return NULL;
}

int BE_Console_GetCommandAmount(void) {
    return beConsoleCommands.used;
}

int BE_Console_GetAllocatedCommandsAmount(void) {
    return (int) beConsoleCommands.size;
}

int BE_Console_GetCommandReallocationAmount(void) {
    return beConsoleCommands.calledRealloc;
}

void BE_PrivateConsole_Initialize(void) {
    BE_ASSERT(!beConsoleInitialized, "Already initialized the console\n");
    SEC_LOGGER_INFO("Initializing console\n");

    beConsoleInitialized = 1;

    BE_DynamicArray_Create(&beConsoleCommands, 100);

#ifndef BE_NO_ENGINE_COMMANDS
    SEC_LOGGER_INFO("Registering engine commands\n");
    BE_EngineCommands_Initialize();
#endif

    beConsoleClientCommandStart = beConsoleCommands.used;
}

// TODO: Verify arguments
void BE_Command_Register(const char* name, const char* description, BE_Command_Flags flags, void (*Run)(BE_Command_Context context)) {
    for (int i = 0; i < (int) beConsoleCommands.used; i++)
        BE_STRICTMODE_CHECK_NO_RETURN_VALUE(strcmp(BE_DYNAMICARRAY_GET_ELEMENT(BE_Command, beConsoleCommands, i)->name, name) != 0,
                            "The command '%s' is already registered\n", name);

    if (!beConsoleDuplicateCommand)
        SEC_LOGGER_TRACE("Registering command\n"
                         "Name: %s\n"
                         "Description: %s\n"
                         "Flags: %i\n"
                         "Spot: %i/%i\n", name, description, flags, beConsoleCommands.used, beConsoleCommands.size);

    beConsoleDuplicateCommand = 0;

    BE_STRICTMODE_CHECK_NO_RETURN_VALUE((flags & BE_COMMAND_FLAG_SERVER_ONLY) == 0 || (flags & BE_COMMAND_FLAG_CLIENT_ONLY) == 0,
                                        "Invalid command flags, cannot be both for server and client only\n");
    BE_STRICTMODE_CHECK_NO_RETURN_VALUE((flags & BE_COMMAND_FLAG_CHEATS_ONLY) == 0 || (flags & BE_COMMAND_FLAG_CLIENT_ONLY) == 0,
                                        "Invalid command flags, the client cannot run any cheat commands\n");

    BE_Command* command = (BE_Command*) BE_EngineMemory_AllocateMemory(sizeof(BE_Command), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

    BE_DynamicArray_Create(&command->arguments, 10);

    command->name = name;
    command->description = description;
    command->index = beConsoleCommands.used;
    command->flags = flags;
    command->Run = Run;
    beConsoleLastCommand = command;

    BE_DynamicArray_AddElementToLast(&beConsoleCommands, (void*) command);
}

void BE_Command_AddArgument(const char* name, int required) {
    BE_Command* command = BE_DYNAMICARRAY_GET_LAST_ELEMENT(BE_Command, beConsoleCommands);

    BE_STRICTMODE_CHECK_NO_RETURN_VALUE(command->arguments.used == 0 ||
                                        BE_DYNAMICARRAY_GET_LAST_ELEMENT(BE_Command_Argument, command->arguments)->required ||
                                        !required, "Required arguments cannot be added after an optional argument\n");

    BE_Command_Argument* argument = BE_EngineMemory_AllocateMemory(sizeof(BE_Command_Argument), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

    argument->name = name;
    argument->required = required;

    BE_DynamicArray_AddElementToLast(&command->arguments, (void*) argument);
}

void BE_Command_DuplicatePrevious(const char* name, const char* description) {
    BE_ASSERT(beConsoleCommands.used != 0, "There is no command to duplicate\n");
    SEC_LOGGER_TRACE("Duplicating command\n"
                     "Original name: %s\n"
                     "Original description: %s\n"
                     "New name: %s\n"
                     "New description: %s\n", beConsoleLastCommand->name, beConsoleLastCommand->description, name,
                     description != NULL ? description : beConsoleLastCommand->description);

    beConsoleDuplicateCommand = 1;

    BE_Command* lastCommand = beConsoleLastCommand;

    BE_Command_Register(name, description != NULL ? description : lastCommand->description, lastCommand->flags,
                        lastCommand->Run);

    // FIXME: This is not thread safe.
    BE_DYNAMICARRAY_GET_LAST_ELEMENT(BE_Command, beConsoleCommands)->arguments = lastCommand->arguments;
}

void BE_Console_ExecuteCommand(const char* input) { // TODO: Client
    // Do not put any logs outside an if check.
    // We do not want to trick users into thinking something is part of the command.

    if (input == NULL || input[0] == '\0')
        return;

    char name[BE_COMMAND_MAX_NAME_LENGTH];
    BE_Command* command = NULL;
    BE_DynamicDictionary arguments;
    BE_DynamicArray userArguments;
    size_t inputLength = strlen(input);

    BE_DynamicDictionary_Create(&arguments, 20);
    BE_DynamicArray_Create(&userArguments, 20);
    memset(name, 0, BE_COMMAND_MAX_NAME_LENGTH);

    int index;
    int argumentStartingIndex;

    for (index = 0; index < inputLength; index++) {
        if (input[index] == ' ') {
            index++;
            break;
        }

        if (index >= BE_COMMAND_MAX_NAME_LENGTH)
            break;

        name[index] = input[index];
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
        int doubleQuote = 0;
        int escaped = 0;
        int added = 0;

        memset(argument, 0, 1024);

        for (int writer = 0; index < inputLength && current < command->arguments.used; index++) {
            added = 0;

            if (writer >= 1024) {
                publish_argument:
                writer = 0;

                BE_DynamicDictionary_AddElementToLast(&arguments, (void*) BE_DYNAMICARRAY_GET_ELEMENT(BE_Command_Argument, command->arguments, current++)->name, argument);
                BE_DynamicArray_AddElementToLast(&userArguments, (void*) argument);

                argument = (char*) BE_EngineMemory_AllocateMemory(sizeof(char) * 1024, BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
                added = 1;

                memset(argument, 0, 1024);
                continue;
            }

            if (input[index] == '\\' && !escaped) {
                escaped = 1;
                continue;
            }

            if (input[index] == '\'' && !escaped) {
                if (quotePosition == -1) {
                    quotePosition = index;
                    continue;
                }

                if (!doubleQuote) {
                    quotePosition = -1;

                    goto publish_argument;
                }
            }

            if (input[index] == '"' && !escaped) {
                if (quotePosition == -1) {
                    quotePosition = index;
                    doubleQuote = 1;
                    continue;
                }

                if (doubleQuote) {
                    quotePosition = -1;
                    doubleQuote = 0;

                    goto publish_argument;
                }
            }

            if (input[index] == ' ' && quotePosition == -1)
                goto publish_argument;

            argument[writer++] = input[index];
            escaped = 0;
        }

        if (quotePosition != -1) {
            SEC_LOGGER_ERROR("Parsing error: unescaped %s quote at %i\n", doubleQuote ? "double" : "single", quotePosition);
            goto destroy;
        }

        if (escaped) {
            SEC_LOGGER_ERROR("Parsing error: stray escape character\n");
            goto destroy;
        }

        if (argument[0] != '\0') {
            BE_DynamicDictionary_AddElementToLast(&arguments, (void*) BE_DYNAMICARRAY_GET_ELEMENT(BE_Command_Argument, command->arguments, current)->name, argument);
            BE_DynamicArray_AddElementToLast(&userArguments, (void*) argument);

            added = 1;
        }

        if (!added)
            BE_EngineMemory_DeallocateMemory(argument, sizeof(char) * 1024, BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
    }

    if ((command->flags & BE_COMMAND_FLAG_SERVER_ONLY) != 0) {
        // TODO: Kick client.

        if (!BE_ClientInformation_IsServerModeEnabled()) {
            SEC_LOGGER_ERROR("This command can only be ran by the server\n");
            goto destroy;
        }
    }

    if ((command->flags & BE_COMMAND_FLAG_CLIENT_ONLY) != 0 && BE_ClientInformation_IsServerModeEnabled()) {
        SEC_LOGGER_ERROR("This command can only be ran by a client\n"); // TODO: Kick client if the client is the one who ran it.
        goto destroy;
    }

    if ((command->flags & BE_COMMAND_FLAG_CHEATS_ONLY) != 0 && !BE_ClientInformation_IsCheatsEnabled()) {
        // TODO: Tell client.
        SEC_LOGGER_ERROR("This command requires cheats to be enabled\n");
        goto destroy;
    }

    command->Run(SEC_CPP_SUPPORT_CREATE_STRUCT(BE_Command_Context, input, input + argumentStartingIndex, userArguments, arguments));

    destroy:
    for (int argumentId = 0; argumentId < arguments.keys.used; argumentId++)
        BE_EngineMemory_DeallocateMemory(arguments.values.internalArray[argumentId], sizeof(char) * 1024, BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

    BE_EngineMemory_DeallocateMemory(arguments.keys.internalArray, sizeof(void*) * arguments.keys.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
    BE_EngineMemory_DeallocateMemory(arguments.values.internalArray, sizeof(void*) * arguments.keys.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
    BE_EngineMemory_DeallocateMemory(userArguments.internalArray, sizeof(void*) * arguments.keys.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
}

int BE_Console_GetLastEngineCommandIndex(void) {
    return beConsoleClientCommandStart - 1;
}

void BE_PrivateConsole_Destroy(void) {
    BE_ASSERT(beConsoleInitialized, "Console are not initialized\n");
    SEC_LOGGER_INFO("Destroying console\n");

    beConsoleInitialized = 0;

    for (int commandId = 0; commandId < beConsoleCommands.used; commandId++) {
        BE_Command* command = BE_DYNAMICARRAY_GET_ELEMENT(BE_Command, beConsoleCommands, commandId);

        for (int argumentId = 0; argumentId < command->arguments.used; argumentId++)
            BE_EngineMemory_DeallocateMemory(command->arguments.internalArray[argumentId], sizeof(BE_Command_Argument),
                                             BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

        // FIXME: This line causes an error due to duplicate commands sharing arguments.
//        BE_EngineMemory_DeallocateMemory(command->arguments.internalArray, sizeof(void*) * command->arguments.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
        BE_EngineMemory_DeallocateMemory(command, sizeof(BE_Command), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
    }

    BE_EngineMemory_DeallocateMemory(beConsoleCommands.internalArray, sizeof(void*) * beConsoleCommands.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
}
SEC_CPP_SUPPORT_GUARD_END()
