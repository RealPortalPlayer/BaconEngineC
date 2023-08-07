// Purpose: String wrapper
// Created on: 7/4/2023 @ 7:17 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <stdlib.h>

#include "Internal/Boolean.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_Boolean SEC_String_Contains(const char* string, const char* compare, SEC_Boolean caseless);
SEC_Boolean SEC_String_Equals(const char* string, const char* compare, SEC_Boolean caseless);
SEC_Boolean SEC_String_StartsWith(const char* string, const char* compare, SEC_Boolean caseless);
SEC_Boolean SEC_String_EndsWith(const char* string, const char* compare, SEC_Boolean caseless);

/**
 * @note Returns NULL if it fails to allocate memory
 */
char* SEC_String_Copy(const char* duplicateFrom);

/**
 * @return The string target
 * @note Returns NULL if it fails to allocate memory
 */
char* SEC_String_Append(char** target, const char* stringToAppend);

/**
 * @return The string target
 * @note Returns NULL if it fails to allocate memory
 * @warning This function assumes you used malloc
 */
char* SEC_String_Prepend(char** target, const char* stringToPrepend);

char* SEC_String_ToLower(char* string);
char* SEC_String_ToUpper(char* string);
char* SEC_String_AppendCharacter(char** target, char character);
char* SEC_String_PrependCharacter(char** target, char character);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
