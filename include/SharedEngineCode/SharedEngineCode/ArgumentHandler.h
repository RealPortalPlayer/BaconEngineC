// Purpose: Helps with argument handling.
// Created on: 3/28/22 @ 9:21 PM

#pragma once

#include "Internal/CppHeader.h"

SEC_CPP_GUARD_START()
    void SEC_ArgumentHandler_Initialize(int argc, char** argv);
    int SEC_ArgumentHandler_GetCount(void);
    char** SEC_ArgumentHandler_GetVector(void);

    /**
    *  @return The index if the argument was found, -1 if not.
    */
    int SEC_ArgumentHandler_GetIndex(const char* argument);

    /**
    * @return The value next to the argument if found, NULL if not.
    */
    const char* SEC_ArgumentHandler_GetValue(const char* argument);
SEC_CPP_GUARD_END()
