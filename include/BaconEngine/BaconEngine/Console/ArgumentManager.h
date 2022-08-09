// Purpose: Manages the arguments a user sends.
// Created on: 8/7/22 @ 12:02 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>
#include <stddef.h>

CPP_GUARD_START()
    typedef struct {
        const char** keys;
        const char** values;
        size_t size;
    } ArgumentsDictionary;

    int GetArgumentInt(ArgumentsDictionary arguments, const char* name, int defaultValue);
    int GetArgumentBoolean(ArgumentsDictionary arguments, const char* name, int defaultValue);
    float GetArgumentFloat(ArgumentsDictionary arguments, const char* name, float defaultValue);
    const char* GetArgumentString(ArgumentsDictionary arguments, const char* name, const char* defaultValue);
CPP_GUARD_END()
