// Purpose: Extended string functions.
// Created on: 9/14/22 @ 11:00 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "Internal/CPlusPlusSupport.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
int SEC_StringExtension_CompareCaseless(const char* string1, const char* string2);
void SEC_StringExtension_ToLowerString(char* string);
void SEC_StringExtension_ToUpperString(char* string);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
