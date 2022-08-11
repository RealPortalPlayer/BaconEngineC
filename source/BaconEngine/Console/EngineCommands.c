#include <SharedEngineCode/Internal/CppHeader.h>

#include "EngineCommands.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Console/Console.h"
#include "BaconEngine/ClientInformation.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/UI.h"
#include "../Rendering/EngineLayers.h"
#include "BaconEngine/Rendering/Layer.h"

CPP_GUARD_START()
    void HelpCommand(CommandContext context);
    void CheatsCommand(CommandContext context);
    void StopCommand(void);
    void DebugInfoCommand(void);
    void SayCommand(CommandContext context);
    void DisconnectCommand(CommandContext context);
    void ConnectCommand(CommandContext context);
    void WhatAmICommand(void);
    void KickCommand(CommandContext context);
    void BanCommand(CommandContext context);
    void SudoCommand(CommandContext context);

    void InitializeEngineCommands(void) {
        static int initialized = 0;

        ASSERT(!initialized, "Engine commands are already initialized");

        initialized = 1;

        RegisterCommand("help", "Shows information about each command.", COMMAND_FLAG_NULL, &HelpCommand);
        RegisterCommand("cheats", "Get information or modify if cheats are enabled.", COMMAND_FLAG_SERVER_ONLY, &CheatsCommand);
        RegisterCommand("stop", IsServerModeEnabled() ? "Stops the server." : "Stops the client.", COMMAND_FLAG_NULL,
                        (void (*)(CommandContext)) &StopCommand);
        DuplicateCommand("exit", IsServerModeEnabled() ? "Exits the server." : "Exits the client.");
        DuplicateCommand("quit", NULL);
        DuplicateCommand("close", IsServerModeEnabled() ? "Closes the server." : "Closes the client.");
        RegisterCommand("debuginfo", "Get debugging information.", COMMAND_FLAG_NULL,
                        (void (*)(CommandContext)) &DebugInfoCommand);
        RegisterCommand("say", "Say something as the console.", COMMAND_FLAG_NULL, &SayCommand);
        RegisterCommand("disconnect", "Disconnects from the server.", COMMAND_FLAG_CLIENT_ONLY, &DisconnectCommand);
        RegisterCommand("connect", "Connects to a server.", COMMAND_FLAG_CLIENT_ONLY, &ConnectCommand);
        RegisterCommand("whatami", "Tells your current mode.", COMMAND_FLAG_NULL,
                        (void (*)(CommandContext)) &WhatAmICommand);
        RegisterCommand("kick", "Forcefully removes a client.", COMMAND_FLAG_SERVER_ONLY, &KickCommand);
        RegisterCommand("ban", "Forcefully removes a client, and prevents them from rejoining.", COMMAND_FLAG_SERVER_ONLY, &BanCommand);
        RegisterCommand("sudo", "Runs a command as the server.", COMMAND_FLAG_CLIENT_ONLY, &SudoCommand);
    }

    void HelpCommand(CommandContext context) {
        (void) context;
        // TODO: Concat strings together.
        ASSERT(0, "This function has not been implemented, yet");
    }

    void CheatsCommand(CommandContext context) {
        if (GetArgumentString(context.arguments, "toggle", "")[0] == '\0') {
            LOG_INFO("Cheats are %s", IsClientCheatsEnabled() ? "enabled" : "disabled");
            return;
        }

        int value = GetArgumentBoolean(context.arguments, "toggle", 0);

        if (IsClientCheatsEnabled() == value) {
            LOG_WARN("Cheats are already %s", IsClientCheatsEnabled() ? "enabled" : "disabled");
            return;
        }

        SetClientCheats(value);

        LOG_INFO("Cheats are now %s", IsClientCheatsEnabled() ? "enabled" : "disabled");

        if (!GetArgumentBoolean(context.arguments, "silence", 0))
            return;

        // TODO: Broadcast to users.
    }

    void StopCommand(void) {
        StopClientRunning();
    }

    void DebugInfoCommand(void) {
        LOG_INFO("Commands: %i/%i (%i realloc)\n"
                 "Layers: %i/%i (%i realloc)\n"
                 "UIs: %i/%i (%i realloc, %i rendering, %i total render calls%s%s)", GetCommandAmount(), GetAllocatedCommandsAmount(), GetCommandReallocationAmount(),
                                                                                     GetLayerAmount(), GetAllocatedLayersAmount(), GetLayerReallocationAmount(),
                                                                                     GetUIWindowAmount(), GetAllocatedUIWindowAmount(), GetUIWindowReallocationAmount(), GetUIWindowRenderCount(), GetUIWindowTotalRenderCallCount(), GetCurrentUIWindow() != NULL ? ", current: " : "", GetCurrentUIWindow() != NULL ? GetCurrentUIWindow()->name : "");
    }

    void SayCommand(CommandContext context) {
        (void) context;
        // TODO: Broadcast to everyone.
        ASSERT(0, "This function has not been implemented, yet");
    }

    void DisconnectCommand(CommandContext context) {
        (void) context;
        ASSERT(0, "This function has not been implemented, yet");
    }

    void ConnectCommand(CommandContext context) {
        (void) context;
        ASSERT(0, "This function has not been implemented, yet");
    }

    void WhatAmICommand(void) {
        if (IsServerModeEnabled()) {
            LOG_INFO("Server");
            return;
        }

        if (GetCurrentRenderer() != RENDERER_TYPE_TEXT) {
            LOG_INFO("Client");
            return;
        }

        LOG_INFO("Headless Client");
    }

    void KickCommand(CommandContext context) {
        (void) context;
        ASSERT(0, "This function has not been implemented, yet");
    }

    void BanCommand(CommandContext context) {
        (void) context;
        ASSERT(0, "This function has not been implemented, yet");
    }

    void SudoCommand(CommandContext context) {
        (void) context;
        ASSERT(0, "This function has not been implemented, yet");
    }
CPP_GUARD_END()
