#include <SharedEngineCode/Internal/CppHeader.h>
#include <string.h>

#include "BaconEngine/I18N.h"
#include "BaconEngine/Debugging/StrictMode.h"

SEC_CPP_GUARD_START()
    const char* BE_I18N_Translate(FILE* languageFile, const char* key) {
        BE_STRICTMODE_CHECK(key[0] != '\0', key, "Key cannot be empty");
        BE_STRICTMODE_CHECK(languageFile != NULL, key, "Language file cannot be null");

        char* line = NULL;
        size_t lengthCap;
        ssize_t length;
        size_t keyLength = strlen(key);

        while ((length = getline(&line, &lengthCap, languageFile)) != -1) {
            if (length == 0 || length <= keyLength || line[keyLength] != '=' || memcmp(line, key, keyLength) != 0)
                continue;

            return line + keyLength + 1;
        }

        // TODO: Should we free?
        SEC_LOGGER_ERROR("Failed to translate: %s", key);

        return key;
    }
SEC_CPP_GUARD_END()
