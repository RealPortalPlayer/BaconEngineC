// Purpose: Helps with argument handling.
// Created on: 3/28/22 @ 9:21 PM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "Internal/CppSupport.h"
#include "Internal/Boolean.h"

SEC_CPP_SUPPORT_GUARD_START()
typedef struct {
    /**
     * Will be the pointer to argumentValue.
     * If short value is defined, then it will be pointing to that instead,
     * no matter what.
     * Short arguments has a higher priority than normal arguments.
     */
    char** value;

    /**
     * Same for value, but for the indexes instead.
     */
    int* index;
    char* argumentValue;
    char* shortValue;
    int argumentIndex;
    int shortIndex;
} SEC_ArgumentHandler_ShortResults;

void SEC_ArgumentHandler_Initialize(int argc, char** argv);
int SEC_ArgumentHandler_GetCount(void);
char** SEC_ArgumentHandler_GetVector(void);

/**
  *  @return The index if the argument was found, -1 if not.
  */
int SEC_ArgumentHandler_GetIndex(const char* argument, SEC_Boolean ignoreDontParse);

/**
  * @return The value next to the argument if found, NULL if not.
  */
char* SEC_ArgumentHandler_GetValue(const char* argument, SEC_Boolean ignoreDontParse);

int SEC_ArgumentHandler_GetInfoWithShort(const char* argument, const char* shortArgument, SEC_Boolean ignoreDontParse,
                                         SEC_ArgumentHandler_ShortResults* results);

SEC_Boolean SEC_ArgumentHandler_ContainsArgumentOrShort(const char* argument, const char* shortArgument, SEC_Boolean ignoreDontParse);
SEC_CPP_SUPPORT_GUARD_END()
