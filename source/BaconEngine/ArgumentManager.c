// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <BaconAPI/Logger.h>

#include "BaconEngine/ArgumentManager.h"
#include "InterfaceFunctions.h"

#ifndef BE_CLIENT_BINARY
#   include "PrivateArgumentManager.h"
#   include "EngineMemory.h"
#   include "Storage/PrivateDynamicArray.h"
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static int BE_ArgumentManager_EqualsStringBoolean(const char* value) {
    return BA_String_Equals(value, "true", BA_BOOLEAN_TRUE) ? 2 : BA_String_Equals(value, "false", BA_BOOLEAN_TRUE);
}
#endif

int BE_ArgumentManager_GetInteger(BA_DynamicDictionary arguments, const char* name, int defaultValue) {
#ifndef BE_CLIENT_BINARY
    if (arguments.keys.size != 0) {
        const char* value = BE_ArgumentManager_GetString(arguments, name, "");
        {
            int parsedValue = BE_ArgumentManager_EqualsStringBoolean(value);

            if (parsedValue != 0)
                return parsedValue == 2;
        }

        char* errored;
        long parsedValue = strtol(value, &errored, 0);

        if (errored == NULL || strlen(errored) == 0 || parsedValue <= INT_MAX)
            return (int) parsedValue;
    }

    return defaultValue;
#else
    BE_INTERFACEFUNCTION(int, BA_DynamicDictionary, const char*, int);
    return function(arguments, name, defaultValue);
#endif
}

BA_Boolean BE_ArgumentManager_GetBoolean(BA_DynamicDictionary arguments, const char* name, int defaultValue) {
#ifndef BE_CLIENT_BINARY
    return BE_ArgumentManager_GetInteger(arguments, name, defaultValue) >= 1;
#else
    BE_INTERFACEFUNCTION(BA_Boolean, BA_DynamicDictionary, const char*, int);
    return function(arguments, name, defaultValue);
#endif
}

float BE_ArgumentManager_GetFloat(BA_DynamicDictionary arguments, const char* name, float defaultValue) {
#ifndef BE_CLIENT_BINARY
    if (arguments.keys.size != 0) {
        const char* value = BE_ArgumentManager_GetString(arguments, name, "");
        {
            int parsedValue = BE_ArgumentManager_EqualsStringBoolean(value);

            if (parsedValue != 0)
                return (float)(parsedValue == 2);
        }

        char* errored;
        float parsedValue = strtof(value, &errored);

        if (errored == NULL || strlen(errored) == 0)
            return parsedValue;
    }

    return defaultValue;
#else
    BE_INTERFACEFUNCTION(float, BA_DynamicDictionary, const char*, float);
    return function(arguments, name, defaultValue);
#endif
}

const char* BE_ArgumentManager_GetString(BA_DynamicDictionary arguments, const char* name, const char* defaultValue) {
#ifndef BE_CLIENT_BINARY
    const char* value = BA_DynamicDictionary_GetElementValueViaKey(&arguments, (void*) name, sizeof(char) + strlen(name));

    return value != NULL ? value : defaultValue;
#else
    BE_INTERFACEFUNCTION(const char*, BA_DynamicDictionary, const char*, const char*);
    return function(arguments, name, defaultValue);
#endif
}

#ifndef BE_CLIENT_BINARY
void BE_PrivateArgumentManager_ParseName(const char* input, char** name, int* argumentStartingIndex) {
    size_t inputLength = strlen(input);
    int index;

    *name = BE_EngineMemory_AllocateMemory(sizeof(char), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
    (*name)[0] = 0;
    
    {
        BA_Boolean trimmed = BA_BOOLEAN_FALSE;

        for (index = 0; index < (int) inputLength; index++) {
            if (input[index] == ' ') {
                if (!trimmed)
                    continue;

                index++;
                break;
            }

            trimmed = BA_BOOLEAN_TRUE;

            BA_String_AppendCharacter(name, input[index]);
            BE_EngineMemory_AddSize(sizeof(char), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
        }
    }

    *argumentStartingIndex = index;
}

BA_Boolean BE_PrivateArgumentManager_ParseArguments(const char* input, int startingIndex, BA_Boolean fancyArgumentParsing, BA_DynamicArray comparedArguments, BA_DynamicDictionary* arguments) {
    char* argument = (char*) BE_EngineMemory_AllocateMemory(sizeof(char), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
    int current = 0;
    int quotePosition = -1;
    BA_Boolean doubleQuote = BA_BOOLEAN_FALSE;
    BA_Boolean escaped = BA_BOOLEAN_FALSE;
    BA_Boolean added = BA_BOOLEAN_FALSE;
    BA_Boolean trimmed = BA_BOOLEAN_FALSE;
    BA_Boolean quoteAdded = BA_BOOLEAN_FALSE;
    size_t inputLength = strlen(input);
    
    argument[0] = 0;

    for (int index = startingIndex; index < (int) inputLength && current < comparedArguments.used; index++) {
        BA_Boolean validEscapeCharacter = BA_BOOLEAN_FALSE;

        if (added) {
            argument = (char*) BE_EngineMemory_AllocateMemory(sizeof(char), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

            argument[0] = 0;
        }

        added = BA_BOOLEAN_FALSE;

        if (input[index] == ' ' && (quoteAdded || !trimmed))
            continue;

        quoteAdded = BA_BOOLEAN_FALSE;

        if (input[index] == ' ' && quotePosition == -1 && !escaped) {
            publish_argument:
            BE_PrivateDynamicArray_CheckResize(&arguments->keys);
            BE_PrivateDynamicArray_CheckResize(&arguments->values);
            BA_DynamicDictionary_AddElementToLast(arguments,
                                                  (void*) BA_DYNAMICARRAY_GET_ELEMENT(BE_ArgumentManager_Argument,
                                                                                      comparedArguments,
                                                                                      current++)->name, argument);

            added = BA_BOOLEAN_TRUE;
            trimmed = BA_BOOLEAN_FALSE;
            continue;
        }

        if (input[index] == '\\' && fancyArgumentParsing) {
            if (!escaped) {
                escaped = BA_BOOLEAN_TRUE;
                continue;
            }

            validEscapeCharacter = BA_BOOLEAN_TRUE;
        }

        if (input[index] == '\'' || input[index] == '"') {
            if (!escaped) {
                if (quotePosition == -1) {
                    if (argument[0] != 0) {
                        index--;
                        goto publish_argument;
                    }

                    quotePosition = index;
                    doubleQuote = input[index] == '"';
                    continue;
                }

                if (doubleQuote == (input[index] == '"')) {
                    quotePosition = -1;
                    doubleQuote = BA_BOOLEAN_FALSE;
                    quoteAdded = BA_BOOLEAN_TRUE;
                    goto publish_argument;
                }
            }

            validEscapeCharacter = BA_BOOLEAN_TRUE;
        }

        trimmed = BA_BOOLEAN_TRUE;

        if (escaped && !validEscapeCharacter && fancyArgumentParsing) {
            BA_LOGGER_ERROR("Parsing error: invalid escape character '%c', use double backslashes instead of one\n", input[index]);
            BE_EngineMemory_DeallocateMemory(argument, sizeof(char) * (strlen(argument) + 1), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
            return BA_BOOLEAN_FALSE;
        }

        BA_String_AppendCharacter(&argument, input[index]);
        BE_EngineMemory_AddSize(sizeof(char), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);

        escaped = BA_BOOLEAN_FALSE;
    }

    if (quotePosition != -1 && fancyArgumentParsing) {
        BA_LOGGER_ERROR("Parsing error: unescaped %s quote at %i\n", doubleQuote ? "double" : "single", quotePosition);
        return BA_BOOLEAN_FALSE;
    }

    if (escaped && fancyArgumentParsing) {
        BA_LOGGER_ERROR("Parsing error: stray escape character\n");
        return BA_BOOLEAN_FALSE;
    }

    if (!added) {
        if (argument[0] != '\0') {
            BE_PrivateDynamicArray_CheckResize(&arguments->keys);
            BE_PrivateDynamicArray_CheckResize(&arguments->values);
            BA_DynamicDictionary_AddElementToLast(arguments,
                                                  (void*) BA_DYNAMICARRAY_GET_ELEMENT(BE_ArgumentManager_Argument ,
                                                                                      comparedArguments,
                                                                                      current)->name, argument);

            added = BA_BOOLEAN_TRUE;
        }

        if (!added)
            BE_EngineMemory_DeallocateMemory(argument, sizeof(char) * (strlen(argument) + 1), BE_ENGINEMEMORY_MEMORY_TYPE_COMMAND);
    }
    
    return BA_BOOLEAN_TRUE;
}
#endif
BA_CPLUSPLUS_SUPPORT_GUARD_END()
