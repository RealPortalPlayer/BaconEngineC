#include "BaconEngine/Debugging/Assert.h"
#include "EngineCommands.h"
#include "BaconEngine/Console/Console.h"
#include "BaconEngine/Rendering/Renderer.h"
#include "../Rendering/EngineLayers.h"
#include "BaconEngine/Rendering/Layer.h"
#include "BaconEngine/ClientInformation.h"
#include "BaconEngine/EngineMemoryInformation.h"
#include "BaconEngine/Rendering/UI.h"

SEC_CPP_SUPPORT_GUARD_START()
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

void BE_EngineCommands_Initialize(void) {
    static int initialized = 0;

    BE_ASSERT(!initialized, "Engine commands are already initialized");

    initialized = 1;

    BE_Console_RegisterCommand("help", "Shows information about each command.", BE_COMMAND_FLAG_NULL,
                               &BE_EngineCommands_Help);
    BE_Console_RegisterCommand("beClientInformationCheats", "Get information or modify if beClientInformationCheats are enabled.",
                               BE_COMMAND_FLAG_SERVER_ONLY,
                               &BE_EngineCommands_Cheats);
    BE_Console_RegisterCommand("stop", BE_ClientInformation_IsServerModeEnabled() ? "Stops the server." : "Stops the client.",
                               BE_COMMAND_FLAG_NULL,
                               (void (*)(BE_Command_Context)) &BE_EngineCommands_Stop);
    BE_Console_DuplicateCommand("exit", BE_ClientInformation_IsServerModeEnabled() ? "Exits the server." : "Exits the client.");
    BE_Console_DuplicateCommand("quit", NULL);
    BE_Console_DuplicateCommand("close", BE_ClientInformation_IsServerModeEnabled() ? "Closes the server." : "Closes the client.");
    BE_Console_RegisterCommand("debuginfo", "Get debugging information.", BE_COMMAND_FLAG_NULL,
                               (void (*)(BE_Command_Context)) &BE_EngineCommands_DebugInfo);
    BE_Console_RegisterCommand("say", "Say something as the console.", BE_COMMAND_FLAG_NULL, &BE_EngineCommands_Say);
    BE_Console_RegisterCommand("disconnect", "Disconnects from the server.", BE_COMMAND_FLAG_CLIENT_ONLY,
                               &BE_EngineCommands_Disconnect);
    BE_Console_RegisterCommand("connect", "Connects to a server.", BE_COMMAND_FLAG_CLIENT_ONLY,
                               &BE_EngineCommands_Connect);
    BE_Console_RegisterCommand("whatami", "Tells your current mode.", BE_COMMAND_FLAG_NULL,
                               (void (*)(BE_Command_Context)) &BE_EngineCommands_WhatAmI);
    BE_Console_RegisterCommand("kick", "Forcefully removes a client.", BE_COMMAND_FLAG_SERVER_ONLY,
                               &BE_EngineCommands_Kick);
    BE_Console_RegisterCommand("ban", "Forcefully removes a client, and prevents them from rejoining.",
                               BE_COMMAND_FLAG_SERVER_ONLY, &BE_EngineCommands_Ban);
    BE_Console_RegisterCommand("sudo", "Runs a command as the server.", BE_COMMAND_FLAG_CLIENT_ONLY,
                               &BE_EngineCommands_Sudo);
}

void BE_EngineCommands_Help(BE_Command_Context context) {
    // TODO: Check if the user specifies what command.
    // TODO: Ignore commands you cannot run.

    SEC_Logger_LogImplementation(1, 1, SEC_LOGGER_LOG_LEVEL_INFO, "Commands: ");

    for (int commandId = 0; commandId < BE_Console_GetCommandAmount(); commandId++) {
        BE_Command* command = BE_Console_GetCommands()[commandId];

        SEC_Logger_LogImplementation(0, 0, SEC_LOGGER_LOG_LEVEL_INFO,
                                     "%s", command->name);
        SEC_Logger_LogImplementation(0, 0, SEC_LOGGER_LOG_LEVEL_INFO, " - ");
        SEC_Logger_LogImplementation(0, 0, SEC_LOGGER_LOG_LEVEL_INFO,
                                     "%s", command->description);
        SEC_Logger_LogImplementation(0, 1, SEC_LOGGER_LOG_LEVEL_DEBUG,
                                     " - flags: %i", command->flags);
        // TODO: Arguments
    }
}

void BE_EngineCommands_Cheats(BE_Command_Context context) {
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

void BE_EngineCommands_Stop(void) {
    BE_ClientInformation_StopRunning();
}

void BE_EngineCommands_DebugInfo(void) {
    BE_EngineMemory_MemoryInformation memoryInformation = BE_EngineMemory_GetMemoryInformation();

    SEC_LOGGER_INFO("Command: %i/%i (%i realloc)\n"
                    "UI: %i rendered (%i/%i, %i realloc)\n"
                    "Layer: %i/%i (%i realloc)\n"
                    "Renderer: %i calls\n"
                    "Engine Memory: %lu bytes\n"
                    "    Command: %lu allocated, %lu bytes\n"
                    "    UI: %lu allocated, %lu bytes\n"
                    "    DynamicArray: %lu allocated, %lu bytes\n"
                    "    Layer: %lu allocated, %lu bytes",
                    BE_Console_GetCommandAmount(), BE_Console_GetAllocatedCommandsAmount(), BE_Console_GetCommandReallocationAmount(), // Command
                    BE_EngineLayers_GetUIWindowRenderCount(), BE_UI_GetWindowAmount(), BE_UI_GetAllocatedWindowsAmount(), BE_UI_GetWindowReallocationAmount(), // UI
                    BE_Layer_GetAmount(), BE_Layer_GetAllocatedLayersAmount(), BE_Layer_GetLayersReallocationAmount(), // Layer
                    BE_Renderer_GetCalls(), // Renderer
                    BE_EngineMemory_GetAllocatedBytes(), // Engine memory
                    memoryInformation.command.allocatedAmount, memoryInformation.command.allocatedBytes, // Command memory
                    memoryInformation.ui.allocatedAmount, memoryInformation.ui.allocatedBytes, // UI memory
                    memoryInformation.dynamicArray.allocatedAmount, memoryInformation.dynamicArray.allocatedBytes, // DynamicArray memory
                    memoryInformation.layer.allocatedAmount, memoryInformation.layer.allocatedBytes); // Layer memory
}

void BE_EngineCommands_Say(BE_Command_Context context) {
    (void) context;
    // TODO: Broadcast to everyone.
    BE_ASSERT_NOT_IMPLEMENTED();
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
        SEC_LOGGER_INFO("Server");
        return;
    }

    if (BE_Renderer_GetCurrentType() != BE_RENDERER_TYPE_TEXT) {
        SEC_LOGGER_INFO("Client");
        return;
    }

    SEC_LOGGER_INFO("Headless Client");
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
SEC_CPP_SUPPORT_GUARD_END()
