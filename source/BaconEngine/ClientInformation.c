#include <SharedEngineCode/ArgumentHandler.h>

#include "BaconEngine/ClientInformation.h"

volatile int running = 1;

int IsServerModeEnabled(void) {
    static int enabled = -1;

    if (enabled == -1)
        enabled = GetArgumentIndex("--server") != -1 || GetArgumentIndex("-s") != -1;

    return enabled;
}

int IsStrictModeEnabled(void) {
    static int enabled = -1;

    if (enabled == -1)
        enabled = GetArgumentIndex("--no-strict") == -1 && GetArgumentIndex("-ns") == -1;

    return enabled;
}