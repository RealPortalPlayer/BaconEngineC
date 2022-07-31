// Purpose: Helps with argument handling.
// Created on: 3/28/22 @ 9:21 PM

#pragma once

#include "Internal/CppHeader.h"

CPP_GUARD_START()
    int addedArgumentsCount;
    char** argumentVector;

    /**
    *  @return The index if the argument was found, -1 if not.
    */
    int GetArgumentIndex(const char* argument);

    /**
    * @return The value next to the argument if found, NULL if not.
    */
    const char* GetArgumentValue(const char* argument);
CPP_GUARD_END()