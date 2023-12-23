// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <stdio.h>
#include <SharedEngineCode/Debugging/StrictMode.h>
#include <BaconAPI/Storage/DynamicDictionary.h>

#include "BaconEngine/I18N.h"
#include "InterfaceFunctions.h"
#include "BaconEngine/Configuration.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static const char* BE_I18N_InternalTranslate(BA_DynamicDictionary* configuration, const char* key) {
    if (configuration == NULL)
        return key;

    int index = 0;
    char* value = NULL;
    
    for (; index < configuration->keys.used; index++) {
        if (strcmp(BA_DYNAMICARRAY_GET_ELEMENT(char, configuration->keys, index), key) != 0) {
            free(configuration->keys.internalArray[index]);
            free(configuration->values.internalArray[index]);
            continue;
        }

        value = BA_DYNAMICARRAY_GET_ELEMENT(char, configuration->values, index);
        break;
    }

    for (index++; index < configuration->keys.used; index++) {
        free(configuration->keys.internalArray[index]);
        free(configuration->values.internalArray[index]);
    }

    if (value != NULL)
        return value;

    BA_LOGGER_ERROR("Failed to translate: %s\n", key);
    return key;
}
#endif

const char* BE_I18N_TranslateFromFile(FILE* languageFile, const char* key) {
#ifndef BE_CLIENT_BINARY
    SEC_STRICTMODE_CHECK(strlen(key) != 0, key, "Key cannot be empty\n");
    SEC_STRICTMODE_CHECK(languageFile != NULL, key, "Language file cannot be null\n");
    return BE_I18N_InternalTranslate(BE_Configuration_ParseFromFile(languageFile), key);
#else
    BE_INTERFACEFUNCTION(const char*, FILE*, const char*);
    return function(languageFile, key);
#endif
}

const char* BE_I18N_Translate(const char* buffer, const char* key) {
#ifndef BE_CLIENT_BINARY
    SEC_STRICTMODE_CHECK(strlen(key) != 0, key, "Key cannot be empty\n");
    return BE_I18N_InternalTranslate(BE_Configuration_Parse(buffer), key);
#else
    BE_INTERFACEFUNCTION(const char*, const char*, const char*);
    return function(buffer, key);
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
