#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/Debugging/Assert.h>

#include "EngineCommands.h"
#include "BaconEngine/Console/Console.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/UI.h"
#include "../Rendering/EngineLayers.h"
#include "BaconEngine/Rendering/Layer.h"
#include "BaconEngine/ClientInformation.h"

SEC_CPP_GUARD_START()
    void HelpCommand(BE_Command_Context context);
    void CheatsCommand(BE_Command_Context context);
    void StopCommand(void);
    void DebugInfoCommand(void);
    void SayCommand(BE_Command_Context context);
    void DisconnectCommand(BE_Command_Context context);
    void ConnectCommand(BE_Command_Context context);
    void WhatAmICommand(void);
    void KickCommand(BE_Command_Context context);
    void BanCommand(BE_Command_Context context);
    void SudoCommand(BE_Command_Context context);

    void InitializeEngineCommands(void) {
        static int initialized = 0;

        SEC_ASSERT(!initialized, "Engine commands are already initialized");

        initialized = 1;

        BE_Console_RegisterCommand("help", "Shows information about each command.", BE_COMMAND_FLAG_NULL, &HelpCommand);
        BE_Console_RegisterCommand("cheats", "Get information or modify if cheats are enabled.",
                                   BE_COMMAND_FLAG_SERVER_ONLY,
                                   &CheatsCommand);
        BE_Console_RegisterCommand("stop", BE_ClientInformation_IsServerModeEnabled() ? "Stops the server." : "Stops the client.",
                                   BE_COMMAND_FLAG_NULL,
                                   (void (*)(BE_Command_Context)) &StopCommand);
        BE_Console_DuplicateCommand("exit", BE_ClientInformation_IsServerModeEnabled() ? "Exits the server." : "Exits the client.");
        BE_Console_DuplicateCommand("quit", NULL);
        BE_Console_DuplicateCommand("close", BE_ClientInformation_IsServerModeEnabled() ? "Closes the server." : "Closes the client.");
        BE_Console_RegisterCommand("debuginfo", "Get debugging information.", BE_COMMAND_FLAG_NULL,
                                   (void (*)(BE_Command_Context)) &DebugInfoCommand);
        BE_Console_RegisterCommand("say", "Say something as the console.", BE_COMMAND_FLAG_NULL, &SayCommand);
        BE_Console_RegisterCommand("disconnect", "Disconnects from the server.", BE_COMMAND_FLAG_CLIENT_ONLY,
                                   &DisconnectCommand);
        BE_Console_RegisterCommand("connect", "Connects to a server.", BE_COMMAND_FLAG_CLIENT_ONLY, &ConnectCommand);
        BE_Console_RegisterCommand("whatami", "Tells your current mode.", BE_COMMAND_FLAG_NULL,
                                   (void (*)(BE_Command_Context)) &WhatAmICommand);
        BE_Console_RegisterCommand("kick", "Forcefully removes a client.", BE_COMMAND_FLAG_SERVER_ONLY, &KickCommand);
        BE_Console_RegisterCommand("ban", "Forcefully removes a client, and prevents them from rejoining.",
                                   BE_COMMAND_FLAG_SERVER_ONLY, &BanCommand);
        BE_Console_RegisterCommand("sudo", "Runs a command as the server.", BE_COMMAND_FLAG_CLIENT_ONLY, &SudoCommand);
    }

    void HelpCommand(BE_Command_Context context) {
        (void) context;
        // TODO: Concat strings together.
        SEC_ASSERT(0, "This function has not been implemented, yet");
    }

    void CheatsCommand(BE_Command_Context context) {
        if (BE_ArgumentHandler_GetString(context.arguments, "toggle", "")[0] == '\0') {
            SEC_LOGGER_INFO("Cheats are %s", BE_ClientInformation_IsCheatsEnabled() ? "enabled" : "disabled");
            return;
        }

        int value = BE_ArgumentHandler_GetBoolean(context.arguments, "toggle", 0);

        if (BE_ClientInformation_IsCheatsEnabled() == value) {
            SEC_LOGGER_WARN("Cheats are already %s", BE_ClientInformation_IsCheatsEnabled() ? "enabled" : "disabled");
            return;
        }

        BE_ClientInformation_SetCheats(value);

        SEC_LOGGER_INFO("Cheats are now %s", BE_ClientInformation_IsCheatsEnabled() ? "enabled" : "disabled");

        if (!BE_ArgumentHandler_GetBoolean(context.arguments, "silence", 0))
            return;

        // TODO: Broadcast to users.
    }

    void StopCommand(void) {
        BE_ClientInformation_StopRunning();
    }

    void DebugInfoCommand(void) {
        SEC_LOGGER_INFO("Commands: %i/%i (%i realloc)\n"
                        "Layers: %i/%i (%i realloc)\n"
                        "UIs: %i/%i (%i realloc, %i rendered%s%s)\n"
                        "Renderer: %i calls\n"
                        "Memory: %u megabytes (%u bytes) allocated by the engine", BE_Console_GetCommandAmount(),
                        BE_Console_GetAllocatedCommandsAmount(),
                        BE_Console_GetCommandReallocationAmount(),
                        BE_Layer_GetAmount(), BE_Layer_GetAllocatedLayersAmount(), BE_Layer_GetLayersReallocationAmount(),
                        BE_UI_GetWindowAmount(), BE_UI_GetAllocatedWindowAmount(), BE_UI_GetWindowReallocationAmount(), GetUIWindowRenderCount(),
                        BE_UI_GetCurrentWindow() != NULL ? ", current: " : "",
                        BE_UI_GetCurrentWindow() != NULL ? BE_UI_GetCurrentWindow()->name : "",
                        BE_Renderer_GetCalls(),
                        SEC_EngineMemory_GetAllocated() / 1000000, SEC_EngineMemory_GetAllocated());
    }

    void SayCommand(BE_Command_Context context) {
        (void) context;
        // TODO: Broadcast to everyone.
        SEC_ASSERT(0, "This function has not been implemented, yet");
    }

    void DisconnectCommand(BE_Command_Context context) {
        (void) context;
        SEC_ASSERT(0, "This function has not been implemented, yet");
    }

    void ConnectCommand(BE_Command_Context context) {
        (void) context;
        SEC_ASSERT(0, "This function has not been implemented, yet");
    }

    void WhatAmICommand(void) {
        if (BE_ClientInformation_IsServerModeEnabled()) {
            SEC_LOGGER_INFO("Server");
            return;
        }

        if (BE_Renderer_GetCurrentType() != BE_RENDERER_TYPE_TEXT) {
            SEC_LOGGER_INFO("Client");
            return;
        }

        SEC_LOGGER_INFO("Headless Client");
    }

    void KickCommand(BE_Command_Context context) {
        (void) context;
        SEC_ASSERT(0, "This function has not been implemented, yet");
    }

    void BanCommand(BE_Command_Context context) {
        (void) context;
        SEC_ASSERT(0, "This function has not been implemented, yet");
    }

    void SudoCommand(BE_Command_Context context) {
        (void) context;
        SEC_ASSERT(0, "This function has not been implemented, yet");
    }
SEC_CPP_GUARD_END()
