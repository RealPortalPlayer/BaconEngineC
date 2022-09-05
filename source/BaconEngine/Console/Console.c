#include <SharedEngineCode/Internal/CppHeader.h>
#include <stddef.h>
#include <string.h>
#include <SharedEngineCode/Debugging/StrictMode.h>
#include <SharedEngineCode/Debugging/Assert.h>
#include <SharedEngineCode/Storage/DynamicArray.h>
#include <SharedEngineCode/EngineMemory.h>

#include "BaconEngine/Console/Console.h"
#include "EngineCommands.h"
#include "BaconEngine/ClientInformation.h"

SEC_CPP_GUARD_START()
    SEC_DynamicArray commandArray;
    BE_Command* lastCommand;
    int duplicateCommand = 0;
    int consoleInitialized = 0;

    BE_Command** BE_Console_GetCommands(void) {
        return (BE_Command**) commandArray.internalArray;
    }

    int BE_Console_GetCommandAmount(void) {
        return commandArray.used;
    }

    int BE_Console_GetAllocatedCommandsAmount(void) {
        return (int) commandArray.size;
    }

    int BE_Console_GetCommandReallocationAmount(void) {
        return commandArray.calledRealloc;
    }

    void BE_Console_Initialize(void) {
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(!consoleInitialized, "Already initialized the console");
        SEC_LOGGER_INFO("Initializing console");

        consoleInitialized = 1;

        SEC_DynamicArray_Create(&commandArray, 100);
        SEC_LOGGER_INFO("Registering engine commands");
        InitializeEngineCommands();
    }

    void BE_Console_RegisterCommand(const char* name, const char* description, BE_Command_Flags flags, void (*Run)(BE_Command_Context context)) {
        for (int i = 0; i < (int) commandArray.used; i++)
            SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(strcmp(SEC_DYNAMICARRAY_GET_ELEMENT(BE_Command, commandArray, i)->name, name) != 0, "The command '%s' is already registered", name);

        if (!duplicateCommand)
            SEC_LOGGER_TRACE("Registering command\n"
                      "Name: %s\n"
                      "Description: %s\n"
                      "Flags: %i\n"
                      "Spot: %i/%i", name, description, flags, commandArray.used, commandArray.size);

        duplicateCommand = 0;

        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE((flags & BE_COMMAND_FLAG_SERVER_ONLY) == 0 || (flags & BE_COMMAND_FLAG_CLIENT_ONLY) == 0,
                                             "Invalid command flags, cannot be both for server and client only");
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE((flags & BE_COMMAND_FLAG_CHEATS_ONLY) == 0 || (flags & BE_COMMAND_FLAG_CLIENT_ONLY) == 0,
                                             "Invalid command flags, the client cannot run any cheat commands");

        BE_Command* command;

        SEC_ASSERT_MALLOC(command, sizeof(BE_Command), "a command");

        command->name = name;
        command->description = description;
        command->flags = flags;
        command->Run = Run;


        SEC_DynamicArray_AddElementToLast(&commandArray, (void *) command);

        lastCommand = command;
    }

    void BE_Console_DuplicateCommand(const char* name, const char* description) {
        SEC_ASSERT(commandArray.used != 0, "There is no command to duplicate");
        SEC_LOGGER_TRACE("Duplicating command\n"
                  "Original name: %s\n"
                  "Original description: %s\n"
                  "New name: %s\n"
                  "New description: %s", lastCommand->name, lastCommand->description, name, description != NULL ? description : lastCommand->description);

        duplicateCommand = 1;

        BE_Console_RegisterCommand(name, description != NULL ? description : lastCommand->description,
                                   lastCommand->flags,
                                   lastCommand->Run);
    }

    void BE_Console_ExecuteCommand(char* input) { // TODO: Client
        // NOTE: Do not put any logs outside an if check.
        //       We do not want to trick users into thinking something is part of the command.
        // TODO: This works, but I'd rather not allocate memory just to run a command. Think of a better solution later.

        if (input[0] == '\0')
            return;

        static BE_Command* cachedCommand = NULL;
        char* name;
        const size_t inputLength = strlen(input);
        SEC_DynamicDictionary arguments;
        BE_Command* command = NULL;

        SEC_DynamicDictionary_Create(&arguments, 100);
        SEC_ASSERT_MALLOC(name, inputLength + 1, "command name");
        {
            int indexer;

            for (indexer = 0; indexer < inputLength; indexer++) {
                if (input[indexer] == ' ')
                    break;

                name[indexer] = input[indexer];
            }

            if (indexer == 0) {
                free(name);
                return;
            }

            name[++indexer] = '\0';

            if (cachedCommand != NULL && strcmp(cachedCommand->name, name) == 0)
                command = cachedCommand;

            if (command == NULL) {
                for (int i = 0; i < (int) commandArray.used; i++) { // TODO: Find a way to replace a for loop with index.
                    if (strcmp(SEC_DYNAMICARRAY_GET_ELEMENT(BE_Command, commandArray, i)->name, name) != 0)
                        continue;

                    command = SEC_DYNAMICARRAY_GET_ELEMENT(BE_Command, commandArray, i);
                    break;
                }
            }

            if (command == NULL) { // TODO: Tell the client.
                SEC_LOGGER_ERROR("'%s' is not a valid command", name);
                return;
            }

            // TODO: Arguments
        }

        cachedCommand = command;

        if ((command->flags & BE_COMMAND_FLAG_SERVER_ONLY) != 0) {
            // TODO: Kick client.

            if (!BE_ClientInformation_IsServerModeEnabled()) {
                SEC_LOGGER_ERROR("This command can only be ran by the server");
                return;
            }
        }

        if ((command->flags & BE_COMMAND_FLAG_CLIENT_ONLY) != 0 && BE_ClientInformation_IsServerModeEnabled()) {
            SEC_LOGGER_ERROR("This command can only be ran by a client"); // TODO: Kick client if the client is the one who ran it.
            return;
        }

        if ((command->flags & BE_COMMAND_FLAG_CHEATS_ONLY) != 0 && !BE_ClientInformation_IsCheatsEnabled()) {
            // TODO: Tell client.
            SEC_LOGGER_ERROR("This command requires cheats to be enabled");
            return;
        }

        command->Run((BE_Command_Context) {
            .arguments = arguments
        });
    }

    void BE_Console_Destroy(void) {
        SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(consoleInitialized, "Console are not initialized");
        SEC_LOGGER_INFO("Destroying console");

        consoleInitialized = 0;

        for (int i = 0; i < commandArray.used; i++) {
            BE_Command* command = SEC_DYNAMICARRAY_GET_ELEMENT(BE_Command, commandArray, i);

            SEC_EngineMemory_RemoveAllocated(sizeof(BE_Command));
            free(command);
        }

        SEC_EngineMemory_RemoveAllocated(sizeof(void *) * commandArray.size);
        free(commandArray.internalArray);
    }
SEC_CPP_GUARD_END()
