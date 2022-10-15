#include <stddef.h>
#include <string.h>

#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Storage/DynamicArray.h"
#include "../EngineMemory.h"
#include "BaconEngine/Console/Console.h"
#include "EngineCommands.h"

SEC_CPP_SUPPORT_GUARD_START()
BE_DynamicArray commandArray;
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

void BE_PrivateConsole_Initialize(void) {
    BE_ASSERT(!consoleInitialized, "Already initialized the console");
    SEC_LOGGER_INFO("Initializing console");

    consoleInitialized = 1;

    BE_DynamicArray_Create(&commandArray, 100);
    SEC_LOGGER_INFO("Registering engine commands");
    BE_EngineCommands_Initialize();
}

// TODO: Verify arguments
void BE_Console_RegisterCommand(const char* name, const char* description, BE_Command_Flags flags, void (*Run)(BE_Command_Context context)) {
    for (int i = 0; i < (int) commandArray.used; i++)
        BE_STRICTMODE_CHECK_NO_RETURN_VALUE(strcmp(BE_DYNAMICARRAY_GET_ELEMENT(BE_Command, commandArray, i)->name, name) != 0,
                                            "The command '%s' is already registered", name);

    if (!duplicateCommand)
        SEC_LOGGER_TRACE("Registering command\n"
                         "Name: %s\n"
                         "Description: %s\n"
                         "Flags: %i\n"
                         "Spot: %i/%i", name, description, flags, commandArray.used, commandArray.size);

    duplicateCommand = 0;

    BE_STRICTMODE_CHECK_NO_RETURN_VALUE((flags & BE_COMMAND_FLAG_SERVER_ONLY) == 0 || (flags & BE_COMMAND_FLAG_CLIENT_ONLY) == 0,
                                        "Invalid command flags, cannot be both for server and client only");
    BE_STRICTMODE_CHECK_NO_RETURN_VALUE((flags & BE_COMMAND_FLAG_CHEATS_ONLY) == 0 || (flags & BE_COMMAND_FLAG_CLIENT_ONLY) == 0,
                                        "Invalid command flags, the client cannot run any cheat commands");

    BE_Command* command = (BE_Command*) BE_EngineMemory_AllocateMemory(sizeof(BE_Command), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

    command->name = name;
    command->description = description;
    command->flags = flags;
//    command->arguments = arguments;
    command->Run = Run;
    lastCommand = command;

    BE_DynamicArray_AddElementToLast(&commandArray, (void*) command);
}

void BE_Console_DuplicateCommand(const char* name, const char* description) {
    BE_ASSERT(commandArray.used != 0, "There is no command to duplicate");
    SEC_LOGGER_TRACE("Duplicating command\n"
                     "Original name: %s\n"
                     "Original description: %s\n"
                     "New name: %s\n"
                     "New description: %s", lastCommand->name, lastCommand->description, name,
                     description != NULL ? description : lastCommand->description);

    duplicateCommand = 1;

    BE_Console_RegisterCommand(name, description != NULL ? description : lastCommand->description, lastCommand->flags, lastCommand->Run);
}

void BE_Console_ExecuteCommand(const char* input) { // TODO: Client
    // TODO: Arguments
    // Do not put any logs outside an if check.
    // We do not want to trick users into thinking something is part of the command.

    if (input == NULL || input[0] == '\0')
        return;

    static BE_Command* cachedCommand = NULL;
    char* name = (char*) input; // FIXME: Casting a const to non-const is undefined behavior.
    BE_Command* command = NULL;

    if (cachedCommand != NULL && strcmp(cachedCommand->name, name) == 0)
        command = cachedCommand;

    if (command == NULL) {
        for (int i = 0; i < (int) commandArray.used; i++) { // TODO: Find a way to replace a for loop with index.
            if (strcmp(BE_DYNAMICARRAY_GET_ELEMENT(BE_Command, commandArray, i)->name, name) != 0)
                continue;

            command = BE_DYNAMICARRAY_GET_ELEMENT(BE_Command, commandArray, i);
            break;
        }
    }

    if (command == NULL) { // TODO: Tell the client.
        SEC_LOGGER_ERROR("'%s' is not a valid command", name);
        return;
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
        SEC_LOGGER_ERROR("This command requires beClientInformationCheats to be enabled");
        return;
    }

    command->Run((BE_Command_Context) {
        .arguments = {.frozen = 0}
    });
}

void BE_PrivateConsole_Destroy(void) {
    BE_ASSERT(consoleInitialized, "Console are not initialized");
    SEC_LOGGER_INFO("Destroying console");

    consoleInitialized = 0;

    for (int i = 0; i < commandArray.used; i++)
        BE_EngineMemory_DeallocateMemory(BE_DYNAMICARRAY_GET_ELEMENT(BE_Command, commandArray, i), sizeof(BE_Command), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

    BE_EngineMemory_DeallocateMemory(commandArray.internalArray, sizeof(void*) * commandArray.size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
}
SEC_CPP_SUPPORT_GUARD_END()
