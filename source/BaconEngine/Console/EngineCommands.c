// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <signal.h>
#include <SharedEngineCode/Paths.h>
#include <BaconAPI/Debugging/Assert.h>

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

BA_CPLUSPLUS_SUPPORT_GUARD_START()
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
    static BA_Boolean initialized = BA_BOOLEAN_FALSE;

    BA_ASSERT(!initialized, "Engine commands are already initialized\n");

    initialized = BA_BOOLEAN_TRUE;

    BE_Command_Register("help", "Shows information about each command.", BE_COMMAND_FLAG_NULL, &BE_EngineCommands_Help);
    {
        BE_Command_AddArgument("command", BA_BOOLEAN_FALSE);
    }

    BE_Command_Register("cheats", "Get information or modify if cheats are enabled.", BE_COMMAND_FLAG_SERVER_ONLY,
                        &BE_EngineCommands_Cheats);
    {
        BE_Command_AddArgument("toggle", BA_BOOLEAN_FALSE);
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

    BE_Command_Register("say", "Say something as the server.", BE_COMMAND_FLAG_SERVER_ONLY | BE_COMMAND_FLAGS_NO_FANCY_ARGUMENT_PARSING, &BE_EngineCommands_Say);
    {
        BE_Command_AddArgument( "message", BA_BOOLEAN_TRUE);
    }

    BE_Command_Register("disconnect", "Disconnects from the server.", BE_COMMAND_FLAG_CLIENT_ONLY,
                        &BE_EngineCommands_Disconnect);
    BE_Command_Register("connect", "Connects to a server.", BE_COMMAND_FLAG_CLIENT_ONLY, &BE_EngineCommands_Connect);
    BE_Command_Register("whatami", "Tells your current mode.", BE_COMMAND_FLAG_NULL,
                        (void (*)(BE_Command_Context)) &BE_EngineCommands_WhatAmI);

    BE_Command_Register("kick", "Forcefully removes a client.", BE_COMMAND_FLAG_SERVER_ONLY, &BE_EngineCommands_Kick);
    {
        BE_Command_AddArgument("client id", BA_BOOLEAN_TRUE);
        BE_Command_AddArgument("reason", BA_BOOLEAN_FALSE);
    }

    BE_Command_Register("ban", "Forcefully removes a client, and prevents them from rejoining.",
                        BE_COMMAND_FLAG_SERVER_ONLY, &BE_EngineCommands_Ban);
    {
        BE_Command_AddArgument("client id", BA_BOOLEAN_TRUE);
        BE_Command_AddArgument("reason", BA_BOOLEAN_FALSE);
    }

    BE_Command_Register("sudo", "Runs a command as the server.", BE_COMMAND_FLAG_CLIENT_ONLY, &BE_EngineCommands_Sudo);
    {
        BE_Command_AddArgument("key", BA_BOOLEAN_TRUE);
        BE_Command_AddArgument("command", BA_BOOLEAN_TRUE);
    }

    BE_Command_Register("echo", "Print text into console.", BE_COMMAND_FLAGS_NO_ARGUMENT_PARSING, &BE_EngineCommands_Echo);
    {
        BE_Command_AddArgument("message", BA_BOOLEAN_FALSE);
    }

    BE_Command_Register("crash", "Force a segmentation fault", BE_COMMAND_FLAG_SERVER_ONLY,
                        (void (*)(BE_Command_Context)) &BE_EngineCommands_Crash);
}

void BE_EngineCommands_HelpPrint(BE_Command* command) {
    if ((BE_BITWISE_IS_BIT_SET(command->flags, BE_COMMAND_FLAG_SERVER_ONLY) && !BE_ClientInformation_IsServerModeEnabled()) ||
        (BE_BITWISE_IS_BIT_SET(command->flags, BE_COMMAND_FLAG_CLIENT_ONLY) && BE_ClientInformation_IsServerModeEnabled()))
        return;

    BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, "        %s", command->name);
    BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, " - ");
    BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, "%s",
                                 command->description);

#ifdef BA_ALLOW_DEBUG_LOGS
    if (command->flags != BE_COMMAND_FLAG_NULL)
        BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_DEBUG, " - flags: %i",
                                     command->flags);
#endif

    for (int argumentId = 0; argumentId < command->arguments.used; argumentId++) {
        BE_Command_Argument* argument = BA_DYNAMICARRAY_GET_ELEMENT(BE_Command_Argument, command->arguments, argumentId);

        BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, "%s", argumentId != 0 ? " " : " - args: ");

        if (argument->required)
            BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, "<%s>", argument->name);
        else
            BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, "[%s]", argument->name);
    }

    BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, "\n");
}

void BE_EngineCommands_Help(BE_Command_Context context) {
    // TODO: Ignore commands you cannot run.

    const char* commandName = BE_ArgumentManager_GetString(context.arguments, "command", NULL);

    if (commandName == NULL) {
        int commandId = 0;
        int showClient = 0;
        int commandIdClient = -1;

        BA_LOGGER_INFO("Help:\n"
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

        BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, "    Client Commands:\n");

        for (; commandIdClient < BE_Console_GetCommandAmount(); commandIdClient++)
            BE_EngineCommands_HelpPrint(BE_Console_GetCommands()[commandIdClient]);

        return;
    }

    BE_Command* command = BE_Console_GetCommand(commandName);

    if (command == NULL) {
        BA_LOGGER_ERROR("'%s' is not a valid command\n", commandName);
        return;
    }

    if (strcmp(command->name, "help") == 0 && rand() % 50 == 0)
        BA_LOGGER_WARN("There is no help\n");

    BA_LOGGER_INFO("Help:\n    %s Command:\n", BE_Console_IsEngineCommand(*command) ? "Engine" : "Client");
    BE_EngineCommands_HelpPrint(command);
}

void BE_EngineCommands_Cheats(BE_Command_Context context) {
    if (BE_ArgumentManager_GetString(context.arguments, "toggle", NULL) == NULL) {
        BA_LOGGER_INFO("Cheats are %s\n", BE_ClientInformation_IsCheatsEnabled() ? "enabled" : "disabled");
        return;
    }

    BA_Boolean value = BE_ArgumentManager_GetBoolean(context.arguments, "toggle", 0);

    if (BE_ClientInformation_IsCheatsEnabled() == value) {
        BA_LOGGER_WARN("Cheats are already %s\n", BE_ClientInformation_IsCheatsEnabled() ? "enabled" : "disabled");
        return;
    }

    BE_ClientInformation_SetCheats(value);
    BA_LOGGER_INFO("Cheats are now %s\n", BE_ClientInformation_IsCheatsEnabled() ? "enabled" : "disabled");

    if (BE_ArgumentManager_GetBoolean(context.arguments, "silence", 0))
        return;

    // TODO: Broadcast to users.
}

void BE_EngineCommands_Stop(void) {
    BE_ClientInformation_StopRunning();
}

void BE_EngineCommands_DebugInfo(void) {
    BE_EngineMemoryInformation memoryInformation = BE_EngineMemoryInformation_Get();

    BA_LOGGER_INFO("DeltaTime: %f seconds (%f milliseconds)\n"
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
                    "    Layer: %zu allocated, %zu bytes\n"
                    "    Server: %zu allocated, %zu bytes\n",
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
                    BE_EngineMemoryInformation_GetAllocatedBytes(), // Engine memory
                    memoryInformation.command.allocatedAmount, memoryInformation.command.allocatedBytes, // Command memory
                    memoryInformation.ui.allocatedAmount, memoryInformation.ui.allocatedBytes, // UI memory
                    memoryInformation.dynamicArray.allocatedAmount, memoryInformation.dynamicArray.allocatedBytes, // DynamicArray memory
                    memoryInformation.layer.allocatedAmount, memoryInformation.layer.allocatedBytes,
                    memoryInformation.server.allocatedAmount, memoryInformation.server.allocatedBytes); // Layer memory
}

void BE_EngineCommands_Say(BE_Command_Context context) {
    BA_LOGGER_INFO("Server: %s\n", context.unparsedArguments);
    // TODO: Broadcast to everyone
}

void BE_EngineCommands_Disconnect(BE_Command_Context context) {
    (void) context;
    BA_ASSERT_NOT_IMPLEMENTED();
}

void BE_EngineCommands_Connect(BE_Command_Context context) {
    (void) context;
    BA_ASSERT_NOT_IMPLEMENTED();
}

void BE_EngineCommands_WhatAmI(void) {
    if (BE_ClientInformation_IsServerModeEnabled()) {
        BA_LOGGER_INFO("Server\n");
        return;
    }

    switch (BE_Renderer_GetCurrentType()) {
        case BE_RENDERER_TYPE_OPENGL: BA_LOGGER_INFO("OpenGL Client\n"); return;
        case BE_RENDERER_TYPE_VULKAN: BA_LOGGER_INFO("Vulkan Client\n"); return;
        case BE_RENDERER_TYPE_METAL: BA_LOGGER_INFO("Metal Client\n"); return;
        case BE_RENDERER_TYPE_DIRECTX: BA_LOGGER_INFO("DirectX Client\n"); return;
        case BE_RENDERER_TYPE_SOFTWARE: BA_LOGGER_INFO("Client\n"); return;
        case BE_RENDERER_TYPE_TEXT: BA_LOGGER_INFO("Headless Client\n"); return;
        default: BA_ASSERT_ALWAYS("This shouldn't be reached\n");
    }
}

void BE_EngineCommands_Kick(BE_Command_Context context) {
    (void) context;
    BA_ASSERT_NOT_IMPLEMENTED();
}

void BE_EngineCommands_Ban(BE_Command_Context context) {
    (void) context;
    BA_ASSERT_NOT_IMPLEMENTED();
}

void BE_EngineCommands_Sudo(BE_Command_Context context) {
    (void) context;
    BA_ASSERT_NOT_IMPLEMENTED();
}

void BE_EngineCommands_Echo(BE_Command_Context context) {
    BA_LOGGER_INFO("%s\n", context.unparsedArguments);
}

void BE_EngineCommands_Crash(void) {
    raise(SIGSEGV);
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
