#include <BaconEngine/EntryPoint.h>
#include <BaconEngine/Console/Console.h>
#include <SharedEngineCode/Logger.h>

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

int BE_EntryPoint_ClientStart(int argc, char** argv) {
    (void) argc;
    (void) argv;

    int index1;
    int index2;

//    if (SEC_ArgumentHandler_GetIndexWithShort(SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING_SHORT, SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING_SHORT, 1, &index1, &index2))
//        SEC_LOGGER_INFO("DUR: %i %i\n", index1, index2);

    InitializeTestLayers();
    BE_Command_Register("test", "Tests the command handler.", BE_COMMAND_FLAG_NULL,
                        (void (*)(BE_Command_Context)) &TestCommand);
    {
        BE_Command_AddArgument("test", 0);
        BE_Command_AddArgument("test2", 0);
    }

    return 0;
}

int BE_EntryPoint_ClientShutdown(void) {
    return 0;
}

int BE_EntryPoint_ClientSupportsServer(void) {
    return 1;
}

const char* BE_EntryPoint_GetClientName(void) {
    return "Test";
}
