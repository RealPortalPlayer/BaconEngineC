#include <BaconEngine/EntryPoint.h>

int BE_EntryPoint_ClientStart(int argc, char** argv) {
    return 0;
}

int BE_EntryPoint_ClientShutdown(void) {
    return 0;
}

int BE_EntryPoint_ClientSupportsServer(void) {
    return 0;
}

const char* BE_EntryPoint_GetClientName(void) {
    return "Standalone Client";
}