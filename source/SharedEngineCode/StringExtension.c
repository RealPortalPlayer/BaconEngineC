// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <ctype.h>
#include <stddef.h>

#include "SharedEngineCode/Internal/CPlusPlusSupport.h"
#include "SharedEngineCode/StringExtension.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
int SEC_StringExtension_CompareCaseless(const char* string1, const char* string2) {
    if (string1 == string2)
        return 0;

    while (*string1 != '\0' && (tolower(*string1) == tolower(*string2))) {
        string1++;
        string2++;
    }
    
    // FIXME: Undefined behaviour !!
    return tolower(*(const unsigned char*) string1) - tolower(*(const unsigned char*) string2);
}

void SEC_StringExtension_ToLowerString(char* string) {
    if (string == NULL)
        return;

    for (int i = 0; string[i] != '\0'; i++)
        string[i] = (char) tolower(string[i]);
}

void SEC_StringExtension_ToUpperString(char* string) {
    if (string == NULL)
        return;

    for (int i = 0; string[i] != '\0'; i++)
        string[i] = (char) toupper(string[i]);
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
