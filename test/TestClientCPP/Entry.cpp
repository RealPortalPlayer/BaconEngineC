// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconEngine/EntryPoint.h>

int BE_EntryPoint_ClientStart(int argc, char** argv) {
    (void) argc;
    (void) argv;

    return 0;
}

int BE_EntryPoint_ClientShutdown() {
    return 0;
}

SEC_Boolean BE_EntryPoint_ClientSupportsServer() {
    return true;
}

const char* BE_EntryPoint_GetClientName() {
    return "Test C++";
}
