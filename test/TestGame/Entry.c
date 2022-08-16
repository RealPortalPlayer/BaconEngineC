#include <BaconEngine/EntryPoint.h>

#include "Layers.h"

void TestCommand(void) {
    SEC_LOG_INFO("Hello, World!");
}

void ButtonOnClickEvent(void) {

}

int BE_ClientStart(int argc, char** argv) {
    (void) argc;
    (void) argv;

    BE_RegisterUIWindow("Hello, World!", (BE_Vector2I) {100, 100}, (BE_Vector2U) {200, 70},
                        BE_UI_WINDOW_FLAG_INVISIBLE_WINDOW);
    {
        BE_RegisterUIElement(
                BE_CreateUIBox((BE_Vector2I) {0, 0}, (BE_Vector2U) {100, 50}, (BE_Color4U) {255, 255, 255, 255}));
    }
    BE_FinishRegisteringUIWindow();
    InitializeTestLayers();
    BE_RegisterCommand("test", "Tests the command handler.", BE_COMMAND_FLAG_NULL,
                       (void (*)(BE_CommandContext)) &TestCommand);

    return 0;
}

int BE_ClientShutdown(void) {
    return 0;
}

int BE_ClientSupportsServer(void) {
    return 1;
}

const char* BE_GetClientName(void) {
    return "Test";
}
