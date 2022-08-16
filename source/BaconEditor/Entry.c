#include <BaconEngine/EntryPoint.h>

int BE_ClientStart(int argc, char** argv) {
    return 0;
}

int BE_ClientShutdown(void) {
    return 0;
}

EXPOSE_FUNC int BE_ClientSupportsServer(void) {
    return 0;
}

EXPOSE_FUNC const char* BE_GetClientName(void) {
    return "BaconEditor";
}