#include <BaconEngine/EntryPoint.h>

int BE_EntryPoint_ClientStart(int argc, char** argv) {
//    BE_UI_RegisterWindow("Clients", (BE_Vector_2I) {300, 300}, (BE_Vector_2U) {380, 450},
//                         BE_UI_WINDOW_FLAG_NO_MAXIMIZE | BE_UI_WINDOW_FLAG_NO_CLOSE);
//    BE_UI_FinishRegisteringWindow();
//    BE_UI_RegisterWindow("TestGame###1", (BE_Vector_2I) {300, 300}, (BE_Vector_2U) {380, 450},
//                         BE_UI_WINDOW_FLAG_NO_MAXIMIZE | BE_UI_WINDOW_FLAG_NO_CLOSE);
//    BE_UI_FinishRegisteringWindow();

    return 0;
}

int BE_EntryPoint_ClientShutdown(void) {
    return 0;
}

BE_ENTRYPOINT_EXPOSE_FUNC int BE_EntryPoint_ClientSupportsServer(void) {
    return 0;
}

BE_ENTRYPOINT_EXPOSE_FUNC const char* BE_EntryPoint_GetClientName(void) {
    return "BaconLauncher";
}