// Purpose: Count allocated engine memory.
// Created on: 9/5/22 @ 3:53 PM

#pragma once

#include "Internal/CppHeader.h"

SEC_CPP_GUARD_START()
    unsigned SEC_EngineMemory_GetAllocated(void);
    void SEC_EngineMemory_AddAllocated(unsigned size);
    void SEC_EngineMemory_RemoveAllocated(unsigned size);
SEC_CPP_GUARD_END()
