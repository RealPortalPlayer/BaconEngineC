#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/Debugging/Assert.h>

#include "EngineCommands.h"
#include "BaconEngine/Console/Console.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/UI.h"
#include "../Rendering/EngineLayers.h"
#include "BaconEngine/Rendering/Layer.h"

CPP_GUARD_START()
    void HelpCommand(BE_CommandContext context);
    void CheatsCommand(BE_CommandContext context);
    void StopCommand(void);
    void DebugInfoCommand(void);
    void SayCommand(BE_CommandContext context);
    void DisconnectCommand(BE_CommandContext context);
    void ConnectCommand(BE_CommandContext context);
    void WhatAmICommand(void);
    void KickCommand(BE_CommandContext context);
    void BanCommand(BE_CommandContext context);
    void SudoCommand(BE_CommandContext context);

    void InitializeEngineCommands(void) {
        static int initialized = 0;

        SEC_ASSERT(!initialized, "Engine commands are already layerInitialized");

        initialized = 1;

        BE_RegisterCommand("help", "Shows information about each command.", BE_COMMAND_FLAG_NULL, &HelpCommand);
        BE_RegisterCommand("cheats", "Get information or modify if cheats are enabled.", BE_COMMAND_FLAG_SERVER_ONLY,
                           &CheatsCommand);
        BE_RegisterCommand("stop", BE_IsServerModeEnabled() ? "Stops the server." : "Stops the client.",
                           BE_COMMAND_FLAG_NULL,
                           (void (*)(BE_CommandContext)) &StopCommand);
        BE_DuplicateCommand("exit", BE_IsServerModeEnabled() ? "Exits the server." : "Exits the client.");
        BE_DuplicateCommand("quit", NULL);
        BE_DuplicateCommand("close", BE_IsServerModeEnabled() ? "Closes the server." : "Closes the client.");
        BE_RegisterCommand("debuginfo", "Get debugging information.", BE_COMMAND_FLAG_NULL,
                           (void (*)(BE_CommandContext)) &DebugInfoCommand);
        BE_RegisterCommand("say", "Say something as the console.", BE_COMMAND_FLAG_NULL, &SayCommand);
        BE_RegisterCommand("disconnect", "Disconnects from the server.", BE_COMMAND_FLAG_CLIENT_ONLY,
                           &DisconnectCommand);
        BE_RegisterCommand("connect", "Connects to a server.", BE_COMMAND_FLAG_CLIENT_ONLY, &ConnectCommand);
        BE_RegisterCommand("whatami", "Tells your current mode.", BE_COMMAND_FLAG_NULL,
                           (void (*)(BE_CommandContext)) &WhatAmICommand);
        BE_RegisterCommand("kick", "Forcefully removes a client.", BE_COMMAND_FLAG_SERVER_ONLY, &KickCommand);
        BE_RegisterCommand("ban", "Forcefully removes a client, and prevents them from rejoining.",
                           BE_COMMAND_FLAG_SERVER_ONLY, &BanCommand);
        BE_RegisterCommand("sudo", "Runs a command as the server.", BE_COMMAND_FLAG_CLIENT_ONLY, &SudoCommand);
    }

    void HelpCommand(BE_CommandContext context) {
        (void) context;
        // TODO: Concat strings together.
        SEC_ASSERT(0, "This function has not been implemented, yet");
    }

    void CheatsCommand(BE_CommandContext context) {
        if (BE_GetArgumentString(context.arguments, "toggle", "")[0] == '\0') {
            SEC_LOG_INFO("Cheats are %s", BE_IsClientCheatsEnabled() ? "enabled" : "disabled");
            return;
        }

        int value = BE_GetArgumentBoolean(context.arguments, "toggle", 0);

        if (BE_IsClientCheatsEnabled() == value) {
            SEC_LOG_WARN("Cheats are already %s", BE_IsClientCheatsEnabled() ? "enabled" : "disabled");
            return;
        }

        BE_SetClientCheats(value);

        SEC_LOG_INFO("Cheats are now %s", BE_IsClientCheatsEnabled() ? "enabled" : "disabled");

        if (!BE_GetArgumentBoolean(context.arguments, "silence", 0))
            return;

        // TODO: Broadcast to users.
    }

    void StopCommand(void) {
        BE_StopClientRunning();
    }

    void DebugInfoCommand(void) {
        SEC_LOG_INFO("Commands: %i/%i (%i realloc)\n"
                     "Layers: %i/%i (%i realloc)\n"
                     "UIs: %i/%i (%i realloc, %i rendered%s%s)\n"
                     "Renderer: %i calls\n"
                     "Memory: %u megabytes (%u bytes) allocated by the engine", BE_GetCommandAmount(), BE_GetAllocatedCommandsAmount(),
                     BE_GetCommandReallocationAmount(),
                     BE_GetLayerAmount(), BE_GetAllocatedLayersAmount(), BE_GetLayerReallocationAmount(),
                     BE_GetUIWindowAmount(), BE_GetAllocatedUIWindowAmount(), BE_GetUIWindowReallocationAmount(), GetUIWindowRenderCount(),
                     BE_GetCurrentUIWindow() != NULL ? ", current: " : "",
                     BE_GetCurrentUIWindow() != NULL ? BE_GetCurrentUIWindow()->name : "",
                     BE_GetRendererCalls(),
                     BE_GetAllocatedEngineMemory() / 1000000, BE_GetAllocatedEngineMemory());
    }

    void SayCommand(BE_CommandContext context) {
        (void) context;
        // TODO: Broadcast to everyone.
        SEC_ASSERT(0, "This function has not been implemented, yet");
    }

    void DisconnectCommand(BE_CommandContext context) {
        (void) context;
        SEC_ASSERT(0, "This function has not been implemented, yet");
    }

    void ConnectCommand(BE_CommandContext context) {
        (void) context;
        SEC_ASSERT(0, "This function has not been implemented, yet");
    }

    void WhatAmICommand(void) {
        if (BE_IsServerModeEnabled()) {
            SEC_LOG_INFO("Server");
            return;
        }

        if (BE_GetCurrentRenderer() != BE_RENDERER_TYPE_TEXT) {
            SEC_LOG_INFO("Client");
            return;
        }

        SEC_LOG_INFO("Headless Client");
    }

    void KickCommand(BE_CommandContext context) {
        (void) context;
        SEC_ASSERT(0, "This function has not been implemented, yet");
    }

    void BanCommand(BE_CommandContext context) {
        (void) context;
        SEC_ASSERT(0, "This function has not been implemented, yet");
    }

    void SudoCommand(BE_CommandContext context) {
        (void) context;
        SEC_ASSERT(0, "This function has not been implemented, yet");
    }
CPP_GUARD_END()
