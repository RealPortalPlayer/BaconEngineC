#include <BaconEngine/EntryPoint.h>

#include "Layers.h"

void TestCommand(void) {
    SEC_LOGGER_INFO("Hello, World!");
}

void ButtonOnClickEvent(void) {

}

int BE_EntryPoint_ClientStart(int argc, char** argv) {
    (void) argc;
    (void) argv;

    BE_UI_RegisterWindow("Hello, World!", (BE_Vector_2I) {100, 100}, (BE_Vector_2U) {200, 70},
                         BE_UI_WINDOW_FLAG_INVISIBLE_WINDOW);
    {
        BE_UI_RegisterElement(
                BE_UIElement_CreateBox((BE_Vector_2I) {0, 0}, (BE_Vector_2U) {100, 50},
                                       (BE_Color_4U) {255, 255, 255, 255}));
    }
    BE_UI_FinishRegisteringWindow();
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
