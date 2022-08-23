// Purpose: Stores information about the current client.
// Created on: 3/30/22 @ 11:59 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

CPP_GUARD_START()
    unsigned BE_GetAllocatedEngineMemory(void);
    void BE_AddAllocatedEngineMemory(unsigned size);
    void BE_RemoveAllocatedEngineMemory(unsigned size);
    int BE_IsClientRunning(void);
    int BE_IsClientCheatsEnabled(void);
    int BE_IsServerModeEnabled(void);
    int BE_IsStrictModeEnabled(void);
    void BE_StopClientRunning(void);
    void BE_SetClientCheats(int enable);
CPP_GUARD_END()
