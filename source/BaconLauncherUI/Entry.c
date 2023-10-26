// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <Interface/EntryPoint.h>

int I_EntryPoint_Start(int argc, char** argv) {
//    BE_UI_RegisterWindow("Clients", (BE_Vector2_Integer) {300, 300}, (BE_Vector2_Unsigned) {380, 450},
//                         BE_UI_WINDOW_FLAG_NO_MAXIMIZE | BE_UI_WINDOW_FLAG_NO_CLOSE);
//    BE_UI_FinishRegisteringWindow();
//    BE_UI_RegisterWindow("TestGame###1", (BE_Vector2_Integer) {300, 300}, (BE_Vector2_Unsigned) {380, 450},
//                         BE_UI_WINDOW_FLAG_NO_MAXIMIZE | BE_UI_WINDOW_FLAG_NO_CLOSE);
//    BE_UI_FinishRegisteringWindow();

    return 0;
}

int I_EntryPoint_Shutdown(void) {
    return 0;
}

BA_Boolean I_EntryPoint_SupportsServer(void) {
    return BA_BOOLEAN_FALSE;
}

const char* I_EntryPoint_GetName(void) {
    return "BaconLauncher";
}
