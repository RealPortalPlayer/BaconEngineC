// Purpose: Helps with argument handling.
// Created on: 3/28/22 @ 9:21 PM

#pragma once

#include "Internal/CppHeader.h"

CPP_GUARD_START()
    void SEC_InitializeArgumentHandler(int argc, char** argv);
    int SEC_GetArgumentCount(void);
    char** SEC_GetArgumentVector(void);

    /**
    *  @return The index if the argument was found, -1 if not.
    */
    int SEC_GetArgumentIndex(const char* argument);

    /**
    * @return The value next to the argument if found, NULL if not.
    */
    const char* SEC_GetArgumentValue(const char* argument);
CPP_GUARD_END()
