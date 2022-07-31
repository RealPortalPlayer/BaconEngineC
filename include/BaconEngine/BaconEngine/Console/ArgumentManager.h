// Purpose: Manages the arguments for a command.
// Created on: 7/30/22 @ 10:31 AM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "CommandArguments.h"

CPP_GUARD_START()
    typedef struct {
        const char** userArguments;
        const char* keys[MAX_ARGUMENTS];
        const char* values[MAX_ARGUMENTS];
    } Arguments;

    int ContainsArgument(Arguments arguments, const char* name);
    short GetArgumentShort(Arguments arguments, const char* name, short defaultValue);
    unsigned short GetArgumentUnsignedShort(Arguments arguments, const char* name, unsigned short defaultValue);
    int GetArgumentInt(Arguments arguments, const char* name, int defaultValue);
    unsigned GetArgumentUnsigned(Arguments arguments, const char* name, unsigned defaultValue);
    long GetArgumentLong(Arguments arguments, const char* name, long defaultValue);
    unsigned long GetArgumentUnsignedLong(Arguments arguments, const char* name, unsigned long defaultValue);
    long long GetArgumentLongLong(Arguments arguments, const char* name, long long defaultValue);
    unsigned long long GetArgumentUnsignedLongLong(Arguments arguments, const char* name, unsigned long long defaultValue);
    float GetArgumentFloat(Arguments arguments, const char* name, float defaultValue);
    double GetArgumentDouble(Arguments arguments, const char* name, double defaultValue);
    long double GetArgumentLongDouble(Arguments arguments, const char* name, long double defaultValue);
    const char* GetArgumentString(Arguments arguments, const char* name, const char* defaultValue);
    int GetArgumentBoolean(Arguments arguments, const char* name, int defaultValue);
CPP_GUARD_END()