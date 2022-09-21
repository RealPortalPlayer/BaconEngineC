#include <BaconEngine/EntryPoint.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <BaconEngine/Console/Console.h>

#include "Layers.h"

void TestCommand(void) {
    SEC_LOGGER_INFO("Hello, World!");
}

int BE_EntryPoint_ClientStart(int argc, char** argv) {
    (void) argc;
    (void) argv;

    int index1;
    int index2;

    if (SEC_ArgumentHandler_GetIndexWithShort(SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING_SHORT, SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING_SHORT, 1, &index1, &index2))
        SEC_LOGGER_INFO("DUR: %i %i", index1, index2);

    InitializeTestLayers();
    BE_Console_RegisterCommand("test", "Tests the command handler.", BE_COMMAND_FLAG_NULL,
                               (void (*)(BE_Command_Context)) &TestCommand);

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
