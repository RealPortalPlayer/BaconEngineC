// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
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
    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(beConsoleCommands.used != 0, "There is no command to add arguments to\n");

    BE_PrivateConsole_Command* command = BA_DYNAMICARRAY_GET_LAST_ELEMENT(BE_PrivateConsole_Command, beConsolePrivateCommands);

    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(command->publicCommand.arguments.used == 0 ||
                                         BA_DYNAMICARRAY_GET_LAST_ELEMENT(BE_Command_Argument, command->publicCommand.arguments)->required ||
                                         !required, "Required arguments cannot be added after an optional argument\n");

    BE_Command_Argument* argument = BE_EngineMemory_AllocateMemory(sizeof(BE_Command_Argument), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

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

void BE_Console_ExecuteCommand(const char* input) { // TODO: Client
#ifndef BE_CLIENT_BINARY
    // Do not put any logs outside an if check.
    // We do not want to trick users into thinking something is part of the command.

    if (input == NULL || input[0] == '\0')
        return;

    char* name = BE_EngineMemory_AllocateMemory(sizeof(char), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
    BE_PrivateConsole_Command* command = NULL;
    BA_DynamicDictionary arguments;
    size_t inputLength = strlen(input);
    
    name[0] = 0;
    
    BE_PrivateDynamicDictionary_Create(&arguments, 20);

    int index;
    int argumentStartingIndex;

    {
        BA_Boolean trimmed = BA_BOOLEAN_FALSE;

        for (index = 0; index < (int) inputLength; index++) {
            if (input[index] == ' ') {
                if (!trimmed)
                    continue;

                index++;
                break;
            }
            
            trimmed = BA_BOOLEAN_TRUE;

            BA_String_AppendCharacter(&name, input[index]);
            BE_EngineMemory_AddSize(sizeof(char), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
        }
    }

    command = BE_Console_GetPrivateCommand(name);
    argumentStartingIndex = index;

    if (command == NULL) { // TODO: Tell the client.
        BA_LOGGER_ERROR("'%s' is not a valid command\n", name);
        goto destroy;
    }

    if (command->publicCommand.arguments.used != 0 && !BE_BITWISE_IS_BIT_SET(command->publicCommand.flags, BE_COMMAND_FLAGS_NO_ARGUMENT_PARSING)) {
        char* argument = (char*) BE_EngineMemory_AllocateMemory(sizeof(char), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
        int current = 0;
        int quotePosition = -1;
        BA_Boolean doubleQuote = BA_BOOLEAN_FALSE;
        BA_Boolean escaped = BA_BOOLEAN_FALSE;
        BA_Boolean added = BA_BOOLEAN_FALSE;
        BA_Boolean trimmed = BA_BOOLEAN_FALSE;
        BA_Boolean quoteAdded = BA_BOOLEAN_FALSE;

        argument[0] = 0;

        for (; index < (int) inputLength && current < command->publicCommand.arguments.used; index++) {
            BA_Boolean validEscapeCharacter = BA_BOOLEAN_FALSE;
            
            if (added) {
                argument = (char*) BE_EngineMemory_AllocateMemory(sizeof(char), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

                argument[0] = 0;
            }

            added = BA_BOOLEAN_FALSE;

            if (input[index] == ' ' && (quoteAdded || !trimmed))
                continue;

            quoteAdded = BA_BOOLEAN_FALSE;

            if (input[index] == ' ' && quotePosition == -1 && !escaped) {
                publish_argument:
                BE_PrivateDynamicArray_CheckResize(&arguments.keys);
                BE_PrivateDynamicArray_CheckResize(&arguments.values);
                BA_DynamicDictionary_AddElementToLast(&arguments,
                                                       (void*) BA_DYNAMICARRAY_GET_ELEMENT(BE_Command_Argument,
                                                                                            command->publicCommand.arguments,
                                                                                            current++)->name, argument);

                added = BA_BOOLEAN_TRUE;
                trimmed = BA_BOOLEAN_FALSE;
                continue;
            }

            if (input[index] == '\\' && !BE_BITWISE_IS_BIT_SET(command->publicCommand.flags, BE_COMMAND_FLAGS_NO_FANCY_ARGUMENT_PARSING)) {
                if (!escaped) {
                    escaped = BA_BOOLEAN_TRUE;
                    continue;
                }
                
                validEscapeCharacter = BA_BOOLEAN_TRUE;
            }
            
            if (input[index] == '\'' || input[index] == '"') {
                if (!escaped) {
                    if (quotePosition == -1) {
                        if (argument[0] != 0) {
                            index--;
                            goto publish_argument;
                        }

                        quotePosition = index;
                        doubleQuote = input[index] == '"';
                        continue;
                    }

                    if (doubleQuote == (input[index] == '"')) {
                        quotePosition = -1;
                        doubleQuote = BA_BOOLEAN_FALSE;
                        quoteAdded = BA_BOOLEAN_TRUE;
                        goto publish_argument;
                    }
                }
                
                validEscapeCharacter = BA_BOOLEAN_TRUE;
            }

            trimmed = BA_BOOLEAN_TRUE;

            if (escaped && !validEscapeCharacter && !BE_BITWISE_IS_BIT_SET(command->publicCommand.flags, BE_COMMAND_FLAGS_NO_FANCY_ARGUMENT_PARSING)) {
                BA_LOGGER_ERROR("Parsing error: invalid escape character '%c', use double backslashes instead of one\n", input[index]);
                BE_EngineMemory_DeallocateMemory(argument, sizeof(char) * (strlen(argument) + 1), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
                goto destroy;
            }

            BA_String_AppendCharacter(&argument, input[index]);
            BE_EngineMemory_AddSize(sizeof(char), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
            
            escaped = BA_BOOLEAN_FALSE;
        }

        if (quotePosition != -1 && !BE_BITWISE_IS_BIT_SET(command->publicCommand.flags, BE_COMMAND_FLAGS_NO_FANCY_ARGUMENT_PARSING)) {
            BA_LOGGER_ERROR("Parsing error: unescaped %s quote at %i\n", doubleQuote ? "double" : "single", quotePosition);
            goto destroy;
        }

        if (escaped && !BE_BITWISE_IS_BIT_SET(command->publicCommand.flags, BE_COMMAND_FLAGS_NO_FANCY_ARGUMENT_PARSING)) {
            BA_LOGGER_ERROR("Parsing error: stray escape character\n");
            goto destroy;
        }

        if (!added) {
            if (argument[0] != '\0') {
                BE_PrivateDynamicArray_CheckResize(&arguments.keys);
                BE_PrivateDynamicArray_CheckResize(&arguments.values);
                BA_DynamicDictionary_AddElementToLast(&arguments,
                                                       (void*) BA_DYNAMICARRAY_GET_ELEMENT(BE_Command_Argument,
                                                                                            command->publicCommand.arguments,
                                                                                            current)->name, argument);

                added = BA_BOOLEAN_TRUE;
            }

            if (!added)
                BE_EngineMemory_DeallocateMemory(argument, sizeof(char) * (strlen(argument) + 1), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
        }
    }

    if (BE_BITWISE_IS_BIT_SET(command->publicCommand.flags, BE_COMMAND_FLAG_SERVER_ONLY)) {
        // TODO: Kick client.

        if (!BE_ClientInformation_IsServerModeEnabled()) {
            BA_LOGGER_ERROR("This command can only be ran by the server\n");
            goto destroy;
        }
    }

    if (BE_BITWISE_IS_BIT_SET(command->publicCommand.flags, BE_COMMAND_FLAG_CLIENT_ONLY) && BE_ClientInformation_IsServerModeEnabled()) {
        BA_LOGGER_ERROR("This command can only be ran by a client\n"); // TODO: Kick client if the client is the one who ran it.
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
    BE_EngineMemory_DeallocateMemory(name, sizeof(char) * (strlen(name) + 1), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
    
    for (int argumentId = 0; argumentId < arguments.keys.used; argumentId++)
        BE_EngineMemory_DeallocateMemory(arguments.values.internalArray[argumentId], sizeof(char) * (strlen(arguments.values.internalArray[argumentId]) + 1), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

    BE_EngineMemory_DeallocateMemory(arguments.keys.internalArray, sizeof(void*) * arguments.keys.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
    BE_EngineMemory_DeallocateMemory(arguments.values.internalArray, sizeof(void*) * arguments.keys.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
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
    BA_ASSERT(beConsoleInitialized, "Console are not initialized\n");
    BA_LOGGER_INFO("Destroying console\n");

    beConsoleInitialized = BA_BOOLEAN_FALSE;

    for (int commandId = 0; commandId < beConsoleCommands.used; commandId++) {
        BE_PrivateConsole_Command* command = BA_DYNAMICARRAY_GET_ELEMENT(BE_PrivateConsole_Command, beConsoleCommands, commandId);

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

BE_BINARYEXPORT void BE_Console_ExecuteFile(FILE* file) {
#ifndef BE_CLIENT_BINARY
    SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(file != NULL, "File cannot be null\n");
    BA_LOGGER_TRACE("Executing list of commands\n");
    
    char* line;
    ssize_t length;
    
    while((length = BA_String_GetLine(file, &line, NULL)) != -1) {
        if (length == -2) {
            BA_LOGGER_TRACE("Failed to allocate enough memory for buffer\n");
            return;
        }

        if (length == 0) {
            free(line);
            continue;
        }

        BE_Console_ExecuteCommand(line);
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

        BE_Console_ExecuteCommand(line);
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
