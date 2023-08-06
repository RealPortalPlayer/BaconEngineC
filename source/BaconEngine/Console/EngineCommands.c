// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <signal.h>
#include <SharedEngineCode/Paths.h>

#include "BaconEngine/Debugging/Assert.h"
#include "EngineCommands.h"
#include "BaconEngine/Console/Console.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "../Rendering/EngineLayers.h"
#include "BaconEngine/Rendering/Layer.h"
#include "BaconEngine/ClientInformation.h"
#include "BaconEngine/EngineMemoryInformation.h"
#include "BaconEngine/Rendering/UI.h"
#include "BaconEngine/Math/Bitwise.h"
#include "BaconEngine/DeltaTime.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
void BE_EngineCommands_Help(BE_Command_Context context);
void BE_EngineCommands_Cheats(BE_Command_Context context);
void BE_EngineCommands_Stop(void);
void BE_EngineCommands_DebugInfo(void);
void BE_EngineCommands_Say(BE_Command_Context context);
void BE_EngineCommands_Disconnect(BE_Command_Context context);
void BE_EngineCommands_Connect(BE_Command_Context context);
void BE_EngineCommands_WhatAmI(void);
void BE_EngineCommands_Kick(BE_Command_Context context);
void BE_EngineCommands_Ban(BE_Command_Context context);
void BE_EngineCommands_Sudo(BE_Command_Context context);
void BE_EngineCommands_Echo(BE_Command_Context context);
void BE_EngineCommands_Crash(void);

void BE_EngineCommands_Initialize(void) {
    static SEC_Boolean initialized = SEC_BOOLEAN_FALSE;

    BE_ASSERT(!initialized, "Engine commands are already initialized\n");

    initialized = SEC_BOOLEAN_TRUE;

    BE_Command_Register("help", "Shows information about each command.", BE_COMMAND_FLAG_NULL, &BE_EngineCommands_Help);
    {
        BE_Command_AddArgument("command", 0);
    }

    BE_Command_Register("cheats", "Get information or modify if cheats are enabled.", BE_COMMAND_FLAG_SERVER_ONLY,
                        &BE_EngineCommands_Cheats);
    {
        BE_Command_AddArgument("toggle", 0);
    }

    BE_Command_Register("stop", BE_ClientInformation_IsServerModeEnabled() ? "Stops the server." : "Stops the client.",
                        BE_COMMAND_FLAG_NULL, (void (*)(BE_Command_Context)) &BE_EngineCommands_Stop);
    {
        BE_Command_DuplicatePrevious("exit", "Exits the client.");
        BE_Command_DuplicatePrevious("quit", NULL);
        BE_Command_DuplicatePrevious("close", "Closes the client.");
    }

    BE_Command_Register("debuginfo", "Get debugging information.", BE_COMMAND_FLAG_NULL,
                        (void (*)(BE_Command_Context)) &BE_EngineCommands_DebugInfo);

    BE_Command_Register("say", "Say something as the server.", BE_COMMAND_FLAG_SERVER_ONLY, &BE_EngineCommands_Say);
    {
        BE_Command_AddArgument( "message", 1);
    }

    BE_Command_Register("disconnect", "Disconnects from the server.", BE_COMMAND_FLAG_CLIENT_ONLY,
                        &BE_EngineCommands_Disconnect);
    BE_Command_Register("connect", "Connects to a server.", BE_COMMAND_FLAG_CLIENT_ONLY, &BE_EngineCommands_Connect);
    BE_Command_Register("whatami", "Tells your current mode.", BE_COMMAND_FLAG_NULL,
                        (void (*)(BE_Command_Context)) &BE_EngineCommands_WhatAmI);

    BE_Command_Register("kick", "Forcefully removes a client.", BE_COMMAND_FLAG_SERVER_ONLY, &BE_EngineCommands_Kick);
    {
        BE_Command_AddArgument("client id", 1);
        BE_Command_AddArgument("reason", 0);
    }

    BE_Command_Register("ban", "Forcefully removes a client, and prevents them from rejoining.",
                        BE_COMMAND_FLAG_SERVER_ONLY, &BE_EngineCommands_Ban);
    {
        BE_Command_AddArgument("client id", 1);
        BE_Command_AddArgument("reason", 0);
    }

    BE_Command_Register("sudo", "Runs a command as the server.", BE_COMMAND_FLAG_CLIENT_ONLY, &BE_EngineCommands_Sudo);
    {
        BE_Command_AddArgument("key", 1);
        BE_Command_AddArgument("command", 1);
    }

    BE_Command_Register("echo", "Print text into console.", BE_COMMAND_FLAG_NULL, &BE_EngineCommands_Echo);
    {
        BE_Command_AddArgument("message", 0);
    }

    BE_Command_Register("crash", "Force a segmentation fault", BE_COMMAND_FLAG_SERVER_ONLY,
                        (void (*)(BE_Command_Context)) &BE_EngineCommands_Crash);
}

void BE_EngineCommands_HelpPrint(BE_Command* command) {
    if ((BE_BITWISE_IS_BIT_SET(command->flags, BE_COMMAND_FLAG_SERVER_ONLY) && !BE_ClientInformation_IsServerModeEnabled()) ||
        (BE_BITWISE_IS_BIT_SET(command->flags, BE_COMMAND_FLAG_CLIENT_ONLY) && BE_ClientInformation_IsServerModeEnabled()))
        return;

    SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, "        %s", command->name);
    SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, " - ");
    SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, "%s",
                                 command->description);

#ifdef BE_ALLOW_DEBUG_LOGS
    if (command->flags != BE_COMMAND_FLAG_NULL)
        SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_DEBUG, " - flags: %i",
                                     command->flags);
#endif

    for (int argumentId = 0; argumentId < command->arguments.used; argumentId++) {
        BE_Command_Argument* argument = BE_DYNAMICARRAY_GET_ELEMENT(BE_Command_Argument, command->arguments, argumentId);

        SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, "%s", argumentId != 0 ? " " : " - args: ");

        if (argument->required)
            SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, "<%s>", argument->name);
        else
            SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, "[%s]", argument->name);
    }

    SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, "\n");
}

void BE_EngineCommands_Help(BE_Command_Context context) {
    // TODO: Ignore commands you cannot run.

    const char* commandName = BE_ArgumentManager_GetString(context.arguments, "command", NULL);

    if (commandName == NULL) {
        int commandId = 0;
        int showClient = 0;
        int commandIdClient = -1;

        SEC_LOGGER_INFO("Help:\n"
                        "    Engine Commands:\n");

        for (; commandId < BE_Console_GetCommandAmount(); commandId++) {
            BE_Command* command = BE_Console_GetCommands()[commandId];

            if (!BE_Console_IsEngineCommand(*command)) {
                if (commandIdClient == -1)
                    commandIdClient = commandId;

                if ((BE_BITWISE_IS_BIT_SET(command->flags, BE_COMMAND_FLAG_SERVER_ONLY) && !BE_ClientInformation_IsServerModeEnabled()) ||
                    (BE_BITWISE_IS_BIT_SET(command->flags, BE_COMMAND_FLAG_CLIENT_ONLY) && BE_ClientInformation_IsServerModeEnabled()))
                    continue;

                showClient++;
                continue;
            }

            BE_EngineCommands_HelpPrint(command);
        }

        if (commandIdClient == -1 || showClient == 0)
            return;

        SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, "    Client Commands:\n");

        for (; commandIdClient < BE_Console_GetCommandAmount(); commandIdClient++)
            BE_EngineCommands_HelpPrint(BE_Console_GetCommands()[commandIdClient]);

        return;
    }

    BE_Command* command = BE_Console_GetCommand(commandName);

    if (command == NULL) {
        SEC_LOGGER_ERROR("'%s' is not a valid command\n", commandName);
        return;
    }

    if (strcmp(command->name, "help") == 0 && rand() % 50 == 0)
        SEC_LOGGER_WARN("There is no help\n");

    SEC_LOGGER_INFO("Help:\n    %s Command:\n", BE_Console_IsEngineCommand(*command) ? "Engine" : "Client");
    BE_EngineCommands_HelpPrint(command);
}

void BE_EngineCommands_Cheats(BE_Command_Context context) {
    if (BE_ArgumentManager_GetString(context.arguments, "toggle", NULL) == NULL) {
        SEC_LOGGER_INFO("Cheats are %s\n", BE_ClientInformation_IsCheatsEnabled() ? "enabled" : "disabled");
        return;
    }

    SEC_Boolean value = BE_ArgumentManager_GetBoolean(context.arguments, "toggle", 0);

    if (BE_ClientInformation_IsCheatsEnabled() == value) {
        SEC_LOGGER_WARN("Cheats are already %s\n", BE_ClientInformation_IsCheatsEnabled() ? "enabled" : "disabled");
        return;
    }

    BE_ClientInformation_SetCheats(value);
    SEC_LOGGER_INFO("Cheats are now %s\n", BE_ClientInformation_IsCheatsEnabled() ? "enabled" : "disabled");

    if (BE_ArgumentManager_GetBoolean(context.arguments, "silence", 0))
        return;

    // TODO: Broadcast to users.
}

void BE_EngineCommands_Stop(void) {
    BE_ClientInformation_StopRunning();
}

void BE_EngineCommands_DebugInfo(void) {
    BE_EngineMemory_MemoryInformation memoryInformation = BE_EngineMemory_GetMemoryInformation();

    SEC_LOGGER_INFO("DeltaTime: %f seconds (%f milliseconds)\n"
                    "Command: %i/%i (%i realloc)\n"
                    "UI: %i rendered (%i/%i, %i realloc)\n"
                    "Layer: %i/%i (%i realloc)\n"
                    "Renderer: %i calls\n"
                    "Paths:\n"
                    "   Launcher directory: %s\n"
                    "   Engine directory: %s\n"
                    "   Client directory: %s\n"
                    "   Engine binary path: %s\n"
                    "   Client binary path: %s\n"
                    "Engine Memory: %zu bytes\n"
                    "    Command: %zu allocated, %zu bytes\n"
                    "    UI: %zu allocated, %zu bytes\n"
                    "    DynamicArray: %zu allocated, %zu bytes\n"
                    "    Layer: %zu allocated, %zu bytes\n",
                    BE_DeltaTime_GetSeconds(), BE_DeltaTime_GetMilliseconds(), // DeltaTime
                    BE_Console_GetCommandAmount(), BE_Console_GetAllocatedCommandsAmount(), BE_Console_GetCommandReallocationAmount(), // Command
                    BE_EngineLayers_GetUIWindowRenderCount(), BE_UI_GetWindowAmount(), BE_UI_GetAllocatedWindowsAmount(), BE_UI_GetWindowReallocationAmount(), // UI
                    BE_Layer_GetAmount(), BE_Layer_GetAllocatedLayersAmount(), BE_Layer_GetLayersReallocationAmount(), // Layer
                    BE_Renderer_GetCalls(), // Renderer
                    SEC_Paths_GetLauncherDirectory(), // Launcher directory
                    SEC_Paths_GetEngineDirectory(), // Engine directory
                    SEC_Paths_GetClientDirectory(), // Client directory
                    SEC_Paths_GetEngineBinaryPath(), // Engine binary path
                    SEC_Paths_GetClientBinaryPath(), // Client binary path
                    BE_EngineMemory_GetAllocatedBytes(), // Engine memory
                    memoryInformation.command.allocatedAmount, memoryInformation.command.allocatedBytes, // Command memory
                    memoryInformation.ui.allocatedAmount, memoryInformation.ui.allocatedBytes, // UI memory
                    memoryInformation.dynamicArray.allocatedAmount, memoryInformation.dynamicArray.allocatedBytes, // DynamicArray memory
                    memoryInformation.layer.allocatedAmount, memoryInformation.layer.allocatedBytes); // Layer memory
}

void BE_EngineCommands_Say(BE_Command_Context context) {
    SEC_LOGGER_INFO("Server: %s\n", context.unparsedArguments);
    // TODO: Broadcast to everyone
}

void BE_EngineCommands_Disconnect(BE_Command_Context context) {
    (void) context;
    BE_ASSERT_NOT_IMPLEMENTED();
}

void BE_EngineCommands_Connect(BE_Command_Context context) {
    (void) context;
    BE_ASSERT_NOT_IMPLEMENTED();
}

void BE_EngineCommands_WhatAmI(void) {
    if (BE_ClientInformation_IsServerModeEnabled()) {
        SEC_LOGGER_INFO("Server\n");
        return;
    }

    if (BE_Renderer_GetCurrentType() != BE_RENDERER_TYPE_TEXT) {
        SEC_LOGGER_INFO("Client\n");
        return;
    }

    SEC_LOGGER_INFO("Headless Client\n");
}

void BE_EngineCommands_Kick(BE_Command_Context context) {
    (void) context;
    BE_ASSERT_NOT_IMPLEMENTED();
}

void BE_EngineCommands_Ban(BE_Command_Context context) {
    (void) context;
    BE_ASSERT_NOT_IMPLEMENTED();
}

void BE_EngineCommands_Sudo(BE_Command_Context context) {
    (void) context;
    BE_ASSERT_NOT_IMPLEMENTED();
}

void BE_EngineCommands_Echo(BE_Command_Context context) {
    SEC_LOGGER_INFO("%s\n", context.unparsedArguments);
}

void BE_EngineCommands_Crash(void) {
    raise(SIGSEGV);
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
