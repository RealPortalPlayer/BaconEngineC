// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconEngine/EntryPoint.h>
#include <stdio.h>

int main(int argc, char** argv) {
    printf("Hello, World!\n");
    printf("This is a custom launcher\n");
    printf("Launching engine\n");
    BE_EntryPoint_StartBaconEngine(argc, argv);
}

int BE_EntryPoint_ClientStart(int argc, char** argv) {
    return 0;
}

int BE_EntryPoint_ClientShutdown(void) {
    return 0;
}

SEC_Boolean BE_EntryPoint_ClientSupportsServer(void) {
    return SEC_FALSE;
}

const char* BE_EntryPoint_GetClientName(void) {
    return "Standalone Client";
}