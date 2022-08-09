// Purpose: Stores information about the current client.
// Created on: 3/30/22 @ 11:59 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

CPP_GUARD_START()
    int IsClientRunning(void);
    int IsClientCheatsEnabled(void);
    int IsServerModeEnabled(void);
    int IsStrictModeEnabled(void);
    void StopClientRunning(void);
    void SetClientCheats(int enable);
CPP_GUARD_END()
