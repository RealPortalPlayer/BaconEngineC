// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <Interface/EntryPoint.h>
#include <BaconEngine/Console/Console.h>
#include <SharedEngineCode/Logger.h>
#include <BaconEngine/Package.h>
#include <stdlib.h>

#include "Layers.h"

void TestCommand(BE_Command_Context context) {
    const char* string = BE_ArgumentManager_GetString(context.arguments, "test", NULL);
    const char* string2 = BE_ArgumentManager_GetString(context.arguments, "test2", NULL);

    if (string == NULL) {
        SEC_LOGGER_INFO("No arguments\n");
        return;
    }

    if (string2 == NULL) {
        SEC_LOGGER_INFO("Argument: %s\n", string);
        return;
    }

    SEC_LOGGER_INFO("Arguments: %s, %s\n", string, string2);
}

int I_EntryPoint_Start(int argc, char** argv) {
    (void) argc;
    (void) argv;

    BE_Package package = BE_Package_Open("Test.package");

    if (package != NULL) {
        void* buffer = NULL;
        size_t size = 0;

        if (BE_Package_GetFile(package, "Test.txt", &buffer, &size)) {
            SEC_LOGGER_INFO("File contents: ");
            fwrite(buffer, 1, size, stdout);
            SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, "\n");
            free(buffer);
        } else
            SEC_LOGGER_ERROR("Failed to open file: Test.txt\n");
    } else
        SEC_LOGGER_WARN("Failed to find: Test.package\n");

    InitializeTestLayers();
    BE_Command_Register("test", "Tests the command handler.", BE_COMMAND_FLAG_NULL,
                        (void (*)(BE_Command_Context)) &TestCommand);
    {
        BE_Command_AddArgument("test", 0);
        BE_Command_AddArgument("test2", 0);
    }

    return 0;
}

SEC_Boolean I_EntryPoint_SupportsServer(void) {
    return SEC_BOOLEAN_TRUE;
}

const char* I_EntryPoint_GetName(void) {
    return "Test";
}
