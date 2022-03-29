#include <BaconEngine/EntryPoint.h>

int ClientStart(int argc, char** argv) {
    return 0;
}

int ClientShutdown() {
    return 0;
}

int SupportsServer() {
    return 1;
}

const char* GetName() {
    return "Test";
}