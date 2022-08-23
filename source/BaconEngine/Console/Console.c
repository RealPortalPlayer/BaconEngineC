#include <SharedEngineCode/Internal/CppHeader.h>
#include <stddef.h>
#include <string.h>
#include <SharedEngineCode/Debugging/StrictMode.h>
#include <SharedEngineCode/Debugging/Assert.h>
#include <SharedEngineCode/Storage/DynamicArray.h>


#include "BaconEngine/Console/Console.h"
#include "EngineCommands.h"

CPP_GUARD_START()
    SEC_DynamicArray commandArray;
    BE_Command* lastCommand;
    int duplicateCommand = 0;
    int consoleInitialized = 0;

    BE_Command** GetCommands(void) {
        return (BE_Command**) commandArray.internalArray;
    }

    int BE_GetCommandAmount(void) {
        return commandArray.used;
    }

    int BE_GetAllocatedCommandsAmount(void) {
        return (int) commandArray.size;
    }

    int BE_GetCommandReallocationAmount(void) {
        return commandArray.calledRealloc;
    }

    void BE_InitializeConsole(void) {
        SEC_STRICT_CHECK_NO_RETURN_VALUE(!consoleInitialized, "Already layerInitialized the console");
        SEC_LOG_INFO("Initializing console");

        consoleInitialized = 1;

        SEC_CreateDynamicArray(&commandArray, 100);
        SEC_LOG_INFO("Registering engine commands");
        InitializeEngineCommands();
    }

    void BE_RegisterCommand(const char* name, const char* description, BE_CommandFlags flags, void (*Run)(BE_CommandContext context)) {
        for (int i = 0; i < (int) commandArray.used; i++)
            SEC_STRICT_CHECK_NO_RETURN_VALUE(strcmp(SEC_ARRAY_GET_ELEMENT(BE_Command, commandArray, i)->name, name) != 0, "The command '%s' is already registered", name);

        if (!duplicateCommand)
            SEC_LOG_TRACE("Registering command\n"
                      "Name: %s\n"
                      "Description: %s\n"
                      "Flags: %i\n"
                      "Spot: %i/%i", name, description, flags, commandArray.used, commandArray.size);

        duplicateCommand = 0;

        SEC_STRICT_CHECK_NO_RETURN_VALUE((flags & BE_COMMAND_FLAG_SERVER_ONLY) == 0 || (flags & BE_COMMAND_FLAG_CLIENT_ONLY) == 0,
                                         "Invalid command flags, cannot be both for server and client only");
        SEC_STRICT_CHECK_NO_RETURN_VALUE((flags & BE_COMMAND_FLAG_CHEATS_ONLY) == 0 || (flags & BE_COMMAND_FLAG_CLIENT_ONLY) == 0,
                                         "Invalid command flags, the client cannot run any cheat commands");

        BE_Command* command;

        SEC_ASSERT_MALLOC(command, sizeof(BE_Command), "a command");

        command->name = name;
        command->description = description;
        command->flags = flags;
        command->Run = Run;


        SEC_ArrayAddElementToLast(&commandArray, (void *) command);

        lastCommand = command;
    }

    void BE_DuplicateCommand(const char* name, const char* description) {
        SEC_ASSERT(commandArray.used != 0, "There is no command to duplicate");
        SEC_LOG_TRACE("Duplicating command\n"
                  "Original name: %s\n"
                  "Original description: %s\n"
                  "New name: %s\n"
                  "New description: %s", lastCommand->name, lastCommand->description, name, description != NULL ? description : lastCommand->description);

        duplicateCommand = 1;

        BE_RegisterCommand(name, description != NULL ? description : lastCommand->description, lastCommand->flags,
                           lastCommand->Run);
    }

    void BE_ExecuteCommand(char* input) { // TODO: Client
        // NOTE: Do not put any logs outside of an if check.
        //       We do not want to trick users into thinking something is part of the command.
        static BE_Command* cachedCommand = NULL;
        char* name = input;
        BE_Command* command = NULL;

//        name[strcspn(name, " ")] = '\0';

        if (cachedCommand != NULL && strcmp(cachedCommand->name, name) == 0)
            command = cachedCommand;

        if (command == NULL) {
            for (int i = 0; i < (int) commandArray.used; i++) { // TODO: Find a way to replace a for loop with index.
                if (strcmp(SEC_ARRAY_GET_ELEMENT(BE_Command, commandArray, i)->name, name) != 0)
                    continue;

                command = SEC_ARRAY_GET_ELEMENT(BE_Command, commandArray, i);
                break;
            }
        }

        if (command == NULL) { // TODO: Tell the client.
            SEC_LOG_ERROR("'%s' is not a valid command", name);
            return;
        }

        cachedCommand = command;

        if ((command->flags & BE_COMMAND_FLAG_SERVER_ONLY) != 0) {
            // TODO: Kick client.

            if (!BE_IsServerModeEnabled()) {
                SEC_LOG_ERROR("This command can only be ran by the server");
                return;
            }
        }

        if ((command->flags & BE_COMMAND_FLAG_CLIENT_ONLY) != 0 && BE_IsServerModeEnabled()) {
            SEC_LOG_ERROR("This command can only be ran by a client"); // TODO: Kick client if the client is the one who ran it.
            return;
        }

        if ((command->flags & BE_COMMAND_FLAG_CHEATS_ONLY) != 0 && !BE_IsClientCheatsEnabled()) {
            // TODO: Tell client.
            SEC_LOG_ERROR("This command requires cheats to be enabled");
            return;
        }

        command->Run((BE_CommandContext) {
            .arguments = {
                .keys = NULL,
                .values = NULL,
                .size = 0
            }
        });
    }

    void BE_DestroyConsole(void) {
        SEC_STRICT_CHECK_NO_RETURN_VALUE(consoleInitialized, "Console are not layerInitialized");
        SEC_LOG_INFO("Destroying console");

        consoleInitialized = 0;

        for (int i = 0; i < commandArray.used; i++) {
            BE_Command* command = SEC_ARRAY_GET_ELEMENT(BE_Command, commandArray, i);

            BE_RemoveAllocatedEngineMemory(sizeof(BE_Command));
            free(command);
        }

        BE_RemoveAllocatedEngineMemory(sizeof(void*) * commandArray.size);
        free(commandArray.internalArray);
    }
CPP_GUARD_END()
