// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <Interface/EntryPoint.h>
#include <BaconEngine/Console/Console.h>
#include <BaconAPI/Logger.h>

#include "Layers.h"

void TestCommand(BE_Command_Context context) {
    const char* string = BE_ArgumentManager_GetString(context.arguments, "test", NULL);
    const char* string2 = BE_ArgumentManager_GetString(context.arguments, "test2", NULL);

    if (string == NULL) {
        BA_LOGGER_INFO("No arguments\n");
        return;
    }

    if (string2 == NULL) {
        BA_LOGGER_INFO("Argument: %s\n", string);
        return;
    }

    BA_LOGGER_INFO("Arguments: %s, %s\n", string, string2);
}

int I_EntryPoint_Start(int argc, char** argv) {
    (void) argc;
    (void) argv;

    InitializeTestLayers();
    BE_Command_Register("test", "Tests the command handler.", BE_COMMAND_FLAG_NULL,
                        (void (*)(BE_Command_Context)) &TestCommand);
    {
        BE_Command_AddArgument("test", 0);
        BE_Command_AddArgument("test2", 0);
    }

    return 0;
}

BA_Boolean I_EntryPoint_SupportsServer(void) {
    return BA_BOOLEAN_TRUE;
}

const char* I_EntryPoint_GetName(void) {
    return "Test";
}
