// Purpose: Extended string functions.
// Created on: 9/14/22 @ 11:00 PM

#pragma once

#include "Internal/CppHeader.h"

SEC_CPP_GUARD_START()
    int SEC_StringExtension_CompareCaseless(const char* string1, const char* string2);
    void SEC_StringExtension_ToLowerString(char* string);
    void SEC_StringExtension_ToUpperString(char* string);
SEC_CPP_GUARD_END()
