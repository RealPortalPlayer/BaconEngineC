// Purpose: Manages the arguments a user sends.
// Created on: 8/7/22 @ 12:02 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>
#include <stddef.h>
#include <SharedEngineCode/Storage/DynamicDictionary.h>

SEC_CPP_GUARD_START()
    int BE_ArgumentHandler_GetInt(SEC_DynamicDictionary arguments, const char* name, int defaultValue);
    int BE_ArgumentHandler_GetBoolean(SEC_DynamicDictionary arguments, const char* name, int defaultValue);
    float BE_ArgumentHandler_GetFloat(SEC_DynamicDictionary arguments, const char* name, float defaultValue);
    const char* BE_ArgumentHandler_GetString(SEC_DynamicDictionary arguments, const char* name, const char* defaultValue);
SEC_CPP_GUARD_END()
