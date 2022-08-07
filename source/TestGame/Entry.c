#include <BaconEngine/EntryPoint.h>

int ClientStart(int argc, char** argv) {
    (void) argc;
    (void) argv;
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
