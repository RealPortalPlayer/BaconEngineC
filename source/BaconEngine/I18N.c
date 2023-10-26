// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <stdio.h>
#include <SharedEngineCode/Debugging/StrictMode.h>

#include "BaconEngine/I18N.h"
#include "InterfaceFunctions.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
const char* BE_I18N_TranslateFromFile(FILE* languageFile, const char* key) {
#ifndef BE_CLIENT_BINARY
    SEC_STRICTMODE_CHECK(strlen(key) != 0, key, "Key cannot be empty\n");
    SEC_STRICTMODE_CHECK(languageFile != NULL, key, "Language file cannot be null\n");

    char* line;
    ssize_t length;
    size_t keyLength = strlen(key);

    while ((length = BA_String_GetLine(languageFile, &line, NULL)) != -1) {
        if (length == -2) {
            BA_LOGGER_TRACE("Failed to allocate enough memory for translated buffer: %s\n", key);
            return key;
        }

        if (length == 0 || length <= keyLength || line[keyLength] != '=' || memcmp(line, key, keyLength) != 0) {
            free(line);
            continue;
        }
        
        return line + keyLength + 1;
    }
    
    BA_LOGGER_ERROR("Failed to translate: %s\n", key);
    return key;
#else
    BE_INTERFACEFUNCTION(const char*, FILE*, const char*);
    return function(languageFile, key);
#endif
}

const char* BE_I18N_Translate(const char* buffer, const char* key) {
#ifndef BE_CLIENT_BINARY
    SEC_STRICTMODE_CHECK(strlen(key) != 0, key, "Key cannot be empty\n");
    
    BA_DynamicArray* translations = BA_String_Split(buffer, "\n");
    size_t keyLength = strlen(key);
    char* chosenLine = NULL;

    for (int i = 0; i < translations->used; i++) {
        char* line = BA_DYNAMICARRAY_GET_ELEMENT_POINTER(char, translations, i);
        size_t lineLength = strlen(line);
        
        if (lineLength == 0 || lineLength <= keyLength || line[keyLength] != '=' || memcmp(line, key, keyLength) != 0) {
            free(translations->internalArray[i]);
            continue;
        }
        
        chosenLine = line + keyLength + 1;
        break;
    }

    free(translations->internalArray);
    free(translations);
    
    if (chosenLine != NULL)
        return chosenLine;

    BA_LOGGER_ERROR("Failed to translate: %s\n", key);
    return key;
#else
    BE_INTERFACEFUNCTION(const char*, const char*, const char*);
    return function(buffer, key);
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
