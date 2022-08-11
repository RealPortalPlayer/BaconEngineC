#include <BaconEngine/EntryPoint.h>
#include <BaconEngine/Rendering/UIWindows.h>

#include "Layers.h"

void TestCommand(void) {
    LOG_INFO("Hello, World!");
}

int ClientStart(int argc, char** argv) {
    (void) argc;
    (void) argv;

    RegisterUIWindow("You should not see this! You should not see this? You should not see this! You should not see this? You should not see this! You should not see this? You should not see this!", (Vector2I){100, 100}, (Vector2U){200, 70}, UI_WINDOW_FLAG_NOT_RESIZABLE | UI_WINDOW_FLAG_NO_MAXIMIZE);

    RegisterUIMessageBox("Main Menu", "You will lose unsaved progress doing this, are you sure?", (Vector2I) {5, 5}, UI_MESSAGE_BOX_FLAG_NULL, (UIMessageBoxChoices) {
        .specialButton1 = "Main Menu",
        .normalButton1  = "Cancel"
    });

//    RegisterUIWindow("Test###1", (Vector2I) {5, 5}, (Vector2U) {200, 200}, UI_WINDOW_FLAG_NULL);
//    RegisterUIWindow("Test###2", (Vector2I) {215, 5}, (Vector2U) {200, 200}, UI_WINDOW_FLAG_NO_CLOSE | UI_WINDOW_FLAG_NOT_MOVABLE);
//    RegisterUIWindow("Test###3", (Vector2I) {425, 5}, (Vector2U) {200, 200}, UI_WINDOW_FLAG_NO_MINIMIZE | UI_WINDOW_FLAG_NO_MAXIMIZE);
//    RegisterUIWindow("Test###4", (Vector2I) {635, 5}, (Vector2U) {200, 200}, UI_WINDOW_FLAG_NO_CLOSE);
//    RegisterUIWindow("Test###5", (Vector2I) {845, 5}, (Vector2U) {200, 200}, UI_WINDOW_FLAG_NO_TITLE_BAR_BUTTONS);
//    RegisterUIWindow("Test###6", (Vector2I) {5, 240}, (Vector2U) {200, 200}, UI_WINDOW_FLAG_NO_TITLE_BAR);
//    RegisterUIWindow("Test###7", (Vector2I) {215, 240}, (Vector2U) {200, 200}, UI_WINDOW_FLAG_NO_BORDER);
//    RegisterUIWindow("Test###8", (Vector2I) {425, 240}, (Vector2U) {200, 200}, UI_WINDOW_FLAG_NO_BORDER | UI_WINDOW_FLAG_NO_TITLE_BAR);
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
