#include <SharedEngineCode/Internal/CppHeader.h>
#include <stddef.h>
#include <string.h>

#include "BaconEngine/Console/Console.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"

CPP_GUARD_START()
    int commandAmount = 0;
    Command registeredCommands[MAX_COMMANDS];

    int GetCommandAmount(void) {
        return commandAmount;
    }

    Command* GetCommands(void) {
        return registeredCommands;
    }

    void RegisterCommand(const char* name, const char* description, CommandArguments arguments, void (*Run)(Command this, CommandContext context)) {
        ASSERT(commandAmount + 1 < MAX_COMMANDS, "Ran out of free command slots");

        for (int i = 0; i < commandAmount; i++)
            STRICT_CHECK_NO_RETURN_VALUE(registeredCommands[i].name != name, "%s is already a registered command", name);

        registeredCommands[commandAmount++] = (Command) {name, description, arguments, Run};
    }

    void ExecuteCommand(const char* name, const char* arguments[MAX_ARGUMENTS]) {
        Command command;
        int found = 0;

        for (int i = 0; i < commandAmount; i++) {
            if (strcmp(registeredCommands[i].name, name) != 0)
                continue;

            command = registeredCommands[i];
            found = 1;
            break;
        }

        if (!found) {
            LOG_ERROR("Command %s not found", name);
            return;
        }

        int argumentCount = 0;
        int requiredArguments = 0;
        Arguments parsedArguments = {arguments, {""}, {0}};

        for (int i = 0; arguments[i]; i++)
            argumentCount++;

        for (int i = 0; i < MAX_ARGUMENTS; i++) {
            if (command.arguments.keys[i] == NULL)
                break;

            if (i < argumentCount) {
                parsedArguments.keys[i] = command.arguments.keys[i];
                parsedArguments.values[i] = arguments[i];
            }

            if (command.arguments.values[i])
                requiredArguments++;
        }

        if (requiredArguments > argumentCount) {
            LOG_ERROR("Not enough arguments");
            return;
        }

        command.Run(command, (CommandContext) {NULL, parsedArguments});
    }
CPP_GUARD_END()