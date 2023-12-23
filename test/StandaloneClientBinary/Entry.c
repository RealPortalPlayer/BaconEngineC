// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <Interface/EntryPoint.h>

int I_EntryPoint_Start(int argc, char** argv) {
    return 0;
}

int I_EntryPoint_Shutdown(void) {
    return 0;
}

BA_Boolean I_EntryPoint_SupportsServer(void) {
    return BA_BOOLEAN_FALSE;
}

const char* I_EntryPoint_GetName(void) {
    return "Standalone Client";
}
