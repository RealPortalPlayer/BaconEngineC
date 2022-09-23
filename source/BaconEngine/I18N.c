#include <SharedEngineCode/Internal/CppSupport.h>
#include <string.h>
#include <SharedEngineCode/Internal/OperatingSystem.h>
#include <stdio.h>

#if SEC_OPERATINGSYSTEM_WINDOWS
#   include <basetsd.h>

typedef SSIZE_T ssize_t;
#endif

#include "BaconEngine/I18N.h"
#include "BaconEngine/Debugging/StrictMode.h"

SEC_CPP_SUPPORT_GUARD_START()
const char* BE_I18N_Translate(FILE* languageFile, const char* key) {
    (void) languageFile;
    (void) key;
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    BE_STRICTMODE_CHECK(key[0] != '\0', key, "Key cannot be empty");
    BE_STRICTMODE_CHECK(languageFile != NULL, key, "Language file cannot be null");

    char* line = NULL;
    size_t lengthCap;
    ssize_t length;
    size_t keyLength = strlen(key);

    // FIXME: getline is not a compliant function.
    while ((length = getline(&line, &lengthCap, languageFile)) != -1) {
        if (length == 0 || length <= keyLength || line[keyLength] != '=' || memcmp(line, key, keyLength) != 0)
            continue;

        return line + keyLength + 1;
    }

    // TODO: Should we free?
    SEC_LOGGER_ERROR("Failed to translate: %s", key);

    return key;
#elif SEC_OPERATINGSYSTEM_WINDOWS
    return ""; // TODO: getline is not standard, figure out how to replace it
#endif
}
SEC_CPP_SUPPORT_GUARD_END()
