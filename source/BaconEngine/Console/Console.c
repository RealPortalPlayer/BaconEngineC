#include <SharedEngineCode/Internal/CppHeader.h>
#include <stddef.h>
#include <string.h>

#include "BaconEngine/Console/Console.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "EngineCommands.h"
#include "BaconEngine/Storage/DynamicArray.h"

CPP_GUARD_START()
    DynamicArray commandArray;
    Command* lastCommand;
    int duplicateCommand = 0;

    Command** GetCommands(void) {
        return (Command**) commandArray.internalArray;
    }

    int GetCommandsAmount(void) {
        return commandArray.used;
    }

    int GetAllocatedCommandsAmount(void) {
        return (int) commandArray.size;
    }

    int GetCommandReallocationAmount(void) {
        return commandArray.calledRealloc;
    }

    void InitializeConsole(void) {
        static int initialized = 0;

        STRICT_CHECK_NO_RETURN_VALUE(!initialized, "Already initialized the console");
        LOG_INFO("Initializing console");

        initialized = 1;

        CreateDynamicArray(&commandArray, 100);
        LOG_INFO("Registering engine commands");
        InitializeEngineCommands();
    }

    void RegisterCommand(const char* name, const char* description, CommandFlags flags, void (*Run)(CommandContext context)) {
        for (int i = 0; i < (int) commandArray.used; i++)
            STRICT_CHECK_NO_RETURN_VALUE(strcmp(GET_ELEMENT(Command, commandArray, i)->name, name) != 0, "The command '%s' is already registered", name);

        if (!duplicateCommand)
            LOG_TRACE("Registering command\n"
                      "Name: %s\n"
                      "Description: %s\n"
                      "Flags: %i\n"
                      "Spot: %i/%i", name, description, flags, commandArray.used, commandArray.size);

        duplicateCommand = 0;

        STRICT_CHECK_NO_RETURN_VALUE((flags & COMMAND_FLAG_SERVER_ONLY) == 0 || (flags & COMMAND_FLAG_CLIENT_ONLY) == 0,
                                     "Invalid command flags, cannot be both for server and client only");
        STRICT_CHECK_NO_RETURN_VALUE((flags & COMMAND_FLAG_CHEATS_ONLY) == 0 || (flags & COMMAND_FLAG_CLIENT_ONLY) == 0,
                                     "Invalid command flags, the client cannot run any cheat commands");

        Command* command = malloc(sizeof(Command));

        command->name = name;
        command->description = description;
        command->flags = flags;
        command->Run = Run;

        ASSERT(command != NULL, "Failed to allocate %lu bytes of data for a layer", sizeof(Command));

        ArrayPushElement(&commandArray, (void*) command);

        lastCommand = command;
    }

    void DuplicateCommand(const char* name, const char* description) {
        ASSERT(commandArray.used != 0, "There is no command to duplicate");
        LOG_TRACE("Duplicating command\n"
                  "Original name: %s\n"
                  "Original description: %s\n"
                  "New name: %s\n"
                  "New description: %s", lastCommand->name, lastCommand->description, name, description != NULL ? description : lastCommand->description);

        duplicateCommand = 1;

        RegisterCommand(name, description != NULL ? description : lastCommand->description, lastCommand->flags, lastCommand->Run);
    }

    void ExecuteCommand(char* input) { // TODO: Client
        // NOTE: Do not put any logs outside of an if check.
        //       We do not want to trick users into thinking something is part of the command.
        static Command* cachedCommand = NULL;
        char* name = strtok(input, " ");
        Command* command = NULL;

        if (cachedCommand != NULL && strcmp(cachedCommand->name, name) == 0)
            command = cachedCommand;

        if (command == NULL) {
            for (int i = 0; i < (int) commandArray.used; i++) { // TODO: Find a way to replace a for loop with index.
                if (strcmp(GET_ELEMENT(Command, commandArray, i)->name, name) != 0)
                    continue;

                command = GET_ELEMENT(Command, commandArray, i);
                break;
            }
        }

        if (command == NULL) { // TODO: Tell the client.
            LOG_ERROR("'%s' is not a valid command", name);
            return;
        }

        cachedCommand = command;

        if ((command->flags & COMMAND_FLAG_SERVER_ONLY) != 0) {
            // TODO: Kick client.

            if (!IsServerModeEnabled()) {
                LOG_ERROR("This command can only be ran by the server");
                return;
            }
        }

        if ((command->flags & COMMAND_FLAG_CLIENT_ONLY) != 0 && IsServerModeEnabled()) {
            LOG_ERROR("This command can only be ran by a client"); // TODO: Kick client if the client is the one who ran it.
            return;
        }

        if ((command->flags & COMMAND_FLAG_CHEATS_ONLY) != 0 && !IsClientCheatsEnabled()) {
            // TODO: Tell client.
            LOG_ERROR("This command requires cheats to be enabled");
            return;
        }

        command->Run((CommandContext) {
            .arguments = {
                .keys = NULL,
                .values = NULL,
                .size = 0
            }
        });
    }
CPP_GUARD_END()
