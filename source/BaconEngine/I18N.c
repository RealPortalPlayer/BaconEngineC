// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <stdio.h>

#include "BaconEngine/I18N.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "InterfaceFunctions.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
const char* BE_I18N_Translate(FILE* languageFile, const char* key) {
#ifndef BE_CLIENT_BINARY
    BE_STRICTMODE_CHECK(key[0] != '\0', key, "Key cannot be empty\n");
    BE_STRICTMODE_CHECK(languageFile != NULL, key, "Language file cannot be null\n");

    //char* line = NULL;
    //size_t lineCap;
    //ssize_t length;
    //size_t keyLength = strlen(key);

    // FIXME: getline is not a compliant function.
    // TODO: Should we free line?
    /*while ((length = getline(&line, &lineCap, languageFile)) != -1) {
        if (length == 0 || length <= keyLength || line[keyLength] != '=' || memcmp(line, key, keyLength) != 0)
            continue;

        return line + keyLength + 1;
    }*/

    SEC_LOGGER_ERROR("Failed to translate: %s\n", key);

    return key;
#else
    BE_INTERFACEFUNCTION(const char*, FILE*, const char*);
    return function(languageFile, key);
#endif
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
