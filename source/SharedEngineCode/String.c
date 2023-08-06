// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "SharedEngineCode/String.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
char* SEC_String_Copy(const char* duplicateFrom) {
    char* string = malloc(sizeof(char) * (strlen(duplicateFrom) + 1));

    if (string == NULL)
        return NULL;
    
    strcpy(string, duplicateFrom);
    return string;
}

char* SEC_String_Append(char** target, const char* stringToAppend) {
    char* reallocatedString = realloc(*target, sizeof(char) * (strlen(*target) + strlen(stringToAppend) + 1));

    if (reallocatedString == NULL)
        return NULL;
    
    *target = reallocatedString;
    
    strcat(*target, stringToAppend);
    return *target;
}

char* SEC_String_Prepend(char** target, const char* stringToPrepend) {
    size_t finalLength = strlen(*target) + strlen(stringToPrepend);
    char* oldString = malloc(sizeof(char) * (strlen(*target) + 1));

    if (oldString == NULL)
        return NULL;

    strcpy(oldString, *target);
    free(*target);

    *target = malloc(sizeof(char) * (finalLength + 1));

    if (*target == NULL)
        return NULL;

    strcpy(*target, stringToPrepend);
    strcat(*target, oldString);
    free(oldString);
    return *target;
}

SEC_Boolean SEC_String_StartsWith(const char* string, const char* compare, SEC_Boolean caseless) {
    if (string == compare)
        return SEC_BOOLEAN_TRUE;

    size_t stringLength = strlen(string);
    size_t compareLength = strlen(compare);
    
    if (stringLength < compareLength)
        return SEC_BOOLEAN_FALSE;

    for (int i = 0; i < compareLength; i++) {
        char character1 = string[i];
        char character2 = compare[i];

        if (caseless) {
            character1 = (char) tolower(character1);
            character2 = (char) tolower(character2);
        }

        if (i < stringLength && character1 == character2)
            continue;

        return SEC_BOOLEAN_FALSE;
    }

    return SEC_BOOLEAN_TRUE;
}

SEC_Boolean SEC_String_EndsWith(const char* string, const char* compare, SEC_Boolean caseless) {
    if (string == compare)
        return SEC_BOOLEAN_TRUE;

    size_t stringLength = strlen(string);
    size_t compareLength = strlen(compare);
    
    if (stringLength < compareLength)
        return SEC_BOOLEAN_FALSE;

    while (stringLength != 0 && compareLength != 0) {
        char character1 = string[stringLength];
        char character2 = compare[compareLength];

        if (caseless) {
            character1 = (char) tolower(character1);
            character2 = (char) tolower(character2);
        }

        if (character1 == character2) {
            stringLength--;
            compareLength--;
            continue;
        }

        return SEC_BOOLEAN_FALSE;
    }

    char finalCharacter1 = string[stringLength];
    char finalCharacter2 = compare[compareLength];

    if (caseless) {
        finalCharacter1 = (char) tolower(finalCharacter1);
        finalCharacter2 = (char) tolower(finalCharacter2);
    }

    return finalCharacter1 == finalCharacter2;
}

char* SEC_String_ToLower(char* string) {
    for (int i = 0; i < strlen(string); i++)
        string[i] = (char) tolower(string[i]);
    
    return string;
}

char* SEC_String_ToUpper(char* string) {
    for (int i = 0; i < strlen(string); i++)
        string[i] = (char) toupper(string[i]);
    
    return string;
}

SEC_Boolean SEC_String_Contains(const char* string, const char* compare, SEC_Boolean caseless) {
    if (string == compare)
        return SEC_BOOLEAN_TRUE;

    size_t stringLength = strlen(string);
    
    if (stringLength < strlen(compare))
        return SEC_BOOLEAN_FALSE;

    SEC_Boolean started = SEC_BOOLEAN_FALSE;
    size_t compareIndex = 0;

    for (int i = 0; i < stringLength; i++) {
        char character1 = string[i];
        char character2 = compare[compareIndex];

        if (character2 == '\0')
            return SEC_BOOLEAN_TRUE;

        if (caseless) {
            character1 = (char) tolower(character1);
            character2 = (char) tolower(character2);
        }

        if (character1 == character2) {
            started = SEC_BOOLEAN_TRUE;
            compareIndex++;
            continue;
        }

        if (!started)
            continue;

        return SEC_BOOLEAN_FALSE;
    }

    return started;
}

SEC_Boolean SEC_String_Equals(const char* string, const char* compare, SEC_Boolean caseless) {
    if (!caseless)
        return strcmp(string, compare) == 0;

    int i = 0;
    size_t compareLength = strlen(compare);
    
    for (; i < strlen(string); i++) {
        if (tolower(string[i]) == tolower(compare[i]))
            continue;

        return SEC_BOOLEAN_FALSE;
    }

    return i == compareLength;
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
