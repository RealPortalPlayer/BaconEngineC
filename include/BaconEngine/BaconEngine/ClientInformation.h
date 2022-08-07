// Purpose: Stores information about the current client.
// Created on: 3/30/22 @ 11:59 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

CPP_GUARD_START()
    volatile int running;
    volatile int cheats;

    int IsServerModeEnabled(void);
    int IsStrictModeEnabled(void);
CPP_GUARD_END()
