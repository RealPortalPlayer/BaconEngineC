#include <BaconEngine/EntryPoint.h>

int BE_ClientStart(int argc, char** argv) {
    BE_RegisterUIWindow("Clients", (BE_Vector2I) {300, 300}, (BE_Vector2U) {380, 450},
                        BE_UI_WINDOW_FLAG_NO_MAXIMIZE | BE_UI_WINDOW_FLAG_NO_CLOSE);
    BE_FinishRegisteringUIWindow();
    BE_RegisterUIWindow("TestGame###1", (BE_Vector2I) {300, 300}, (BE_Vector2U) {380, 450},
                        BE_UI_WINDOW_FLAG_NO_MAXIMIZE | BE_UI_WINDOW_FLAG_NO_CLOSE);
    BE_FinishRegisteringUIWindow();

    return 0;
}

int BE_ClientShutdown(void) {
    return 0;
}

EXPOSE_FUNC int BE_ClientSupportsServer(void) {
    return 0;
}

EXPOSE_FUNC const char* BE_GetClientName(void) {
    return "BaconLauncher";
}