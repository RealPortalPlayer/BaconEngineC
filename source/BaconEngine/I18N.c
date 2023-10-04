// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <stdio.h>
#include <SharedEngineCode/Debugging/StrictMode.h>

#include "BaconEngine/I18N.h"
#include "InterfaceFunctions.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
const char* BE_I18N_Translate(FILE* languageFile, const char* key) {
#ifndef BE_CLIENT_BINARY
    SEC_STRICTMODE_CHECK(strlen(key) != 0, key, "Key cannot be empty\n");
    SEC_STRICTMODE_CHECK(languageFile != NULL, key, "Language file cannot be null\n");

    // TODO: String formatting? We cannot just use SEC_String_Format, since the translated string is controlled by the
    //       user. Perhaps a new function for SEC_String just for formatting user controlled strings?
    char* line;
    ssize_t length;
    size_t keyLength = strlen(key);

    while ((length = SEC_String_GetLine(languageFile, &line, NULL)) != -1) {
        if (length == -2) {
            SEC_LOGGER_TRACE("Failed to allocate enough memory for translated buffer: %s\n", key);
            return key;
        }

        if (length == 0 || length <= keyLength || line[keyLength] != '=' || memcmp(line, key, keyLength) != 0)
            continue;
        
        return line + keyLength + 1;
    }
    
    SEC_LOGGER_ERROR("Failed to translate: %s\n", key);
    return key;
#else
    BE_INTERFACEFUNCTION(const char*, FILE*, const char*);
    return function(languageFile, key);
#endif
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
