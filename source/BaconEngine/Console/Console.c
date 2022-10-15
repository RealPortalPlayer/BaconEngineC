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

BE_Command** BE_Console_GetCommands(void) {
    return (BE_Command**) beConsoleCommands.internalArray;
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
    BE_ASSERT(!beConsoleInitialized, "Already initialized the console");
    SEC_LOGGER_INFO("Initializing console");

    beConsoleInitialized = 1;

    BE_DynamicArray_Create(&beConsoleCommands, 100);
    SEC_LOGGER_INFO("Registering engine commands");
    BE_EngineCommands_Initialize();
}

// TODO: Verify arguments
int BE_Command_Register(const char* name, const char* description, BE_Command_Flags flags, void (*Run)(BE_Command_Context context)) {
    for (int i = 0; i < (int) beConsoleCommands.used; i++)
        BE_STRICTMODE_CHECK(strcmp(BE_DYNAMICARRAY_GET_ELEMENT(BE_Command, beConsoleCommands, i)->name, name) != 0, -1,
                            "The command '%s' is already registered", name);

    if (!beConsoleDuplicateCommand)
        SEC_LOGGER_TRACE("Registering command\n"
                         "Name: %s\n"
                         "Description: %s\n"
                         "Flags: %i\n"
                         "Spot: %i/%i", name, description, flags, beConsoleCommands.used, beConsoleCommands.size);

    beConsoleDuplicateCommand = 0;

    BE_STRICTMODE_CHECK((flags & BE_COMMAND_FLAG_SERVER_ONLY) == 0 || (flags & BE_COMMAND_FLAG_CLIENT_ONLY) == 0, -1,
                                        "Invalid command flags, cannot be both for server and client only");
    BE_STRICTMODE_CHECK((flags & BE_COMMAND_FLAG_CHEATS_ONLY) == 0 || (flags & BE_COMMAND_FLAG_CLIENT_ONLY) == 0, -1,
                                        "Invalid command flags, the client cannot run any cheat commands");

    BE_Command* command = (BE_Command*) BE_EngineMemory_AllocateMemory(sizeof(BE_Command), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

    BE_DynamicArray_Create(&command->arguments, 10);

    command->name = name;
    command->description = description;
    command->flags = flags;
    command->Run = Run;
    beConsoleLastCommand = command;

    BE_DynamicArray_AddElementToLast(&beConsoleCommands, (void*) command);

    return beConsoleCommands.used - 1;
}

void BE_Command_AddArgument(int id, const char* name, int required) {
    BE_STRICTMODE_CHECK_NO_RETURN_VALUE(id >= 0 && id < beConsoleCommands.used, "Invalid command ID");

    BE_Command* command = BE_DYNAMICARRAY_GET_ELEMENT(BE_Command, beConsoleCommands, id);

    BE_STRICTMODE_CHECK_NO_RETURN_VALUE(command->arguments.used == 0 ||
                                        BE_DYNAMICARRAY_GET_LAST_ELEMENT(BE_Command_Argument, command->arguments)->required ||
                                        !required, "Required arguments cannot be added after an optional argument");

    BE_Command_Argument* argument = BE_EngineMemory_AllocateMemory(sizeof(BE_Command_Argument), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

    argument->name = name;
    argument->required = required;

    BE_DynamicArray_AddElementToLast(&command->arguments, (void*) argument);
}

void BE_Command_DuplicatePrevious(const char* name, const char* description) {
    BE_ASSERT(beConsoleCommands.used != 0, "There is no command to duplicate");
    SEC_LOGGER_TRACE("Duplicating command\n"
                     "Original name: %s\n"
                     "Original description: %s\n"
                     "New name: %s\n"
                     "New description: %s", beConsoleLastCommand->name, beConsoleLastCommand->description, name,
                     description != NULL ? description : beConsoleLastCommand->description);

    beConsoleDuplicateCommand = 1;

    BE_Command* lastCommand = beConsoleLastCommand;

    BE_Command_Register(name, description != NULL ? description : lastCommand->description, lastCommand->flags,
                        lastCommand->Run);

    // FIXME: This is not thread safe.
    BE_DYNAMICARRAY_GET_LAST_ELEMENT(BE_Command, beConsoleCommands)->arguments = lastCommand->arguments;
}

void BE_Console_ExecuteCommand(const char* input) { // TODO: Client
    // TODO: Arguments
    // Do not put any logs outside an if check.
    // We do not want to trick users into thinking something is part of the command.

    if (input == NULL || input[0] == '\0')
        return;

    static BE_Command* cachedCommand = NULL;
    char name[BE_COMMAND_MAX_NAME_LENGTH];
    BE_Command* command = NULL;
    size_t inputLength = strlen(input);

    memset(name, 0, BE_COMMAND_MAX_NAME_LENGTH);

    {
        int index;

        for (index = 0; index < inputLength; index++) {
            if (index >= BE_COMMAND_MAX_NAME_LENGTH || input[index] == ' ')
                break;

            name[index] = input[index];
        }
    }

    if (cachedCommand != NULL && strcmp(cachedCommand->name, name) == 0)
        command = cachedCommand;

    if (command == NULL)
        for (int i = 0; i < (int) beConsoleCommands.used; i++) { // TODO: Find a way to replace a for loop with index.
            if (strcmp(BE_DYNAMICARRAY_GET_ELEMENT(BE_Command, beConsoleCommands, i)->name, name) != 0)
                continue;

            command = BE_DYNAMICARRAY_GET_ELEMENT(BE_Command, beConsoleCommands, i);
            break;
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
        SEC_LOGGER_ERROR("This command requires cheats to be enabled");
        return;
    }

    command->Run((BE_Command_Context) {
        .arguments = {.frozen = 0}
    });
}

void BE_PrivateConsole_Destroy(void) {
    BE_ASSERT(beConsoleInitialized, "Console are not initialized");
    SEC_LOGGER_INFO("Destroying console");

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
