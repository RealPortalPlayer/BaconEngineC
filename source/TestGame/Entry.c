#include <BaconEngine/EntryPoint.h>

#include "Layers.h"

void TestCommand(void) {
    LOG_INFO("Hello, World!");
}

int ClientStart(int argc, char** argv) {
    (void) argc;
    (void) argv;

    InitializeTestLayers();
    RegisterCommand("test", "Tests the command handler.", COMMAND_FLAG_NULL, (void (*)(CommandContext)) &TestCommand);

    return 0;
}

int ClientShutdown(void) {
    return 0;
}

int ClientSupportsServer(void) {
    return 1;
}

const char* GetClientName(void) {
    return "Test";
}
