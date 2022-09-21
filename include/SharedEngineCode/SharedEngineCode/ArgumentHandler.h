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
    int SEC_ArgumentHandler_GetIndex(const char* argument, int ignoreDontParse);

    /**
     * @param argumentIndex The index if the argument was found, -1 if not. Doesn't set if NULL.
     * @param shortIndex The same as argumentIndex, but for the short version.
     * @return If either one isn't -1.
     */
    int SEC_ArgumentHandler_GetIndexWithShort(const char* argument, const char* shortArgument, int ignoreDontParse, int* argumentIndex, int* shortIndex);

    /**
    * @return The value next to the argument if found, NULL if not.
    */
    char* SEC_ArgumentHandler_GetValue(const char* argument, int ignoreDontParse);

    /**
     * @param argumentValue The value if the argument was found. Doesn't set if NULL/not found.
     * @param shortValue The same as argumentValue, but for the short version.
     * @return 1 if only one was set, 2 if both was set, and 0 if none was set.
     */
    int SEC_ArgumentHandler_GetValueWithShort(const char* argument, const char* shortArgument, int ignoreDontParse, char** argumentValue, char** shortValue);
SEC_CPP_GUARD_END()
