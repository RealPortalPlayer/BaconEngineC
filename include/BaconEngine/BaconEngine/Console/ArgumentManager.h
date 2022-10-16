// Purpose: Manages the arguments a user sends.
// Created on: 8/7/22 @ 12:02 PM

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>
#include <stddef.h>

#include <BaconEngine/Storage/DynamicDictionary.h>

SEC_CPP_SUPPORT_GUARD_START()
int BE_ArgumentManager_GetInt(BE_DynamicDictionary arguments, const char* name, int defaultValue);
int BE_ArgumentManager_GetBoolean(BE_DynamicDictionary arguments, const char* name, int defaultValue);
float BE_ArgumentManager_GetFloat(BE_DynamicDictionary arguments, const char* name, float defaultValue);
const char* BE_ArgumentManager_GetString(BE_DynamicDictionary arguments, const char* name, const char* defaultValue);
SEC_CPP_SUPPORT_GUARD_END()
