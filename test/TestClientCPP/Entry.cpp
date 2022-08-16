#include <BaconEngine/EntryPoint.h>

int BE_ClientStart(int argc, char** argv) {
    (void) argc;
    (void) argv;

    return 0;
}

int BE_ClientShutdown(void) {
    return 0;
}

int BE_ClientSupportsServer(void) {
    return 1;
}

const char* BE_GetClientName(void) {
    return "Test C++";
}
