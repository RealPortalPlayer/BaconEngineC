// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdio.h>
#include <Interface/EntryPoint.h>
#include <SharedEngineCode/Internal/PlatformSpecific.h>

int main(int argc, char** argv) {
    printf("Hello, World!\n");
    printf("This is a custom launcher\n");
    printf("Getting engine\n");

    void* engineBinary = SEC_PLATFORMSPECIFIC_GET_BINARY("BaconEngine" SEC_PLATFORMSPECIFIC_BINARY_EXTENSION, RTLD_NOW);

    if (engineBinary == NULL) {
        fprintf(stderr, "Cannot find engine binary\n");
        return 1;
    }

    printf("Getting engine entry-point\n");

    int (*start)(void*, void*, int, char**) = (int (*)(void*, void*, int, char**)) SEC_PLATFORMSPECIFIC_GET_ADDRESS(engineBinary, "BE_EntryPoint_StartBaconEngine");

    if (start == NULL) {
        fprintf(stderr, "Could not found engine entry-point\n");
        return 1;
    }

    printf("Starting engine\n");
    return start(engineBinary, SEC_PLATFORMSPECIFIC_GET_BINARY(NULL, RTLD_NOW), argc, argv);
}

const char* I_EntryPoint_GetName(void) {
    return "Standalone Client";
}
