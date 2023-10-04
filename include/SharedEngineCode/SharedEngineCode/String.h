// Purpose: String wrapper
// Created on: 7/4/2023 @ 7:17 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "Internal/Boolean.h"
#include "SharedEngineCode/Storage/DynamicArray.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_Boolean SEC_String_Contains(const char* string, const char* compare, SEC_Boolean caseless);
SEC_Boolean SEC_String_Equals(const char* string, const char* compare, SEC_Boolean caseless);
SEC_Boolean SEC_String_StartsWith(const char* string, const char* compare, SEC_Boolean caseless);
SEC_Boolean SEC_String_EndsWith(const char* string, const char* compare, SEC_Boolean caseless);
SEC_Boolean SEC_String_ContainsCharacter(const char* string, char compare, SEC_Boolean caseless);

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
 */
char* SEC_String_Prepend(char** target, const char* stringToPrepend);

char* SEC_String_ToLower(char* string);
char* SEC_String_ToUpper(char* string);

/**
 * @return The string target
 * @note Returns NULL if it fails to allocate memory
 */
char* SEC_String_AppendCharacter(char** target, char character);

/**
 * @return The string target
 * @note Returns NULL if it fails to allocate memory
 */
char* SEC_String_PrependCharacter(char** target, char character);

/** 
 * @return The string target
 * @note Returns NULL if it fails to allocate memory
 * @warning Passing user controlled input as target will open up a uncontrolled format string attack.
 */
char* SEC_String_Format(char** target, ...);

/** 
 * @return The string target
 * @note Returns NULL if it fails to allocate memory
 * @warning Passing user controlled input as target will open up a uncontrolled format string attack.
 */
char* SEC_String_FormatPremadeList(char** target, va_list arguments);

/**
 * @return A char* DynamicArray
 * @note Make sure to free all the elements, including the DynamicArray's internalArray, and the DynamicArray itself, once done using
 */
SEC_DynamicArray* SEC_String_Split(const char* target, const char* splitBy);

/**
 * @return A char* DynamicArray
 * @note Make sure to free all the elements, including the DynamicArray's internalArray, and the DynamicArray itself, once done using
 */
SEC_DynamicArray* SEC_String_SplitCharacter(const char* target, char splitBy);

/**
 * Reads all the file contents into a buffer, this buffer can get quite big
 * @param lengthLimit Limit how long the buffer will be, zero will disable it
 * @param lineLength Tells you how long the resulting buffer is. We do nothing to it if it equals to NULL
 * @return The contents of the file, NULL if it failed to allocate memory
 */
char* SEC_String_ReadFile(FILE* file, size_t lengthLimit, size_t* lineLength);

/**
 * Gets the next line in the file
 * @param line The output string
 * @param splitString Split the file contents by this string, defaults to newline if NULL
 * @return The length of the current line, -1 if it's end of file, -2 if it failed to allocate buffer
 * @note This moves the file pointer
 * @warning Make sure to run free on the returned buffer
 */
ssize_t SEC_String_GetLine(FILE* file, char** line, const char* splitString);

/**
 * Gets the next line in the file
 * @param line The output string
 * @param splitCharacter Split the file contents by this character
 * @return The length of the current line, -1 if it's end of file, -2 if it failed to allocate buffer
 * @note This moves the file pointer
 * @warning Make sure to run free on the returned buffer
 */
ssize_t SEC_String_GetLineCharacter(FILE* file, char** line, char splitCharacter);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
