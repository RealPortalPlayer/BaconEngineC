#include <string.h>
#include <stdlib.h>
#include <SharedEngineCode/StringExtension.h>

#include "BaconEngine/Console/ArgumentManager.h"

SEC_CPP_SUPPORT_GUARD_START()
int BE_ArgumentManager_EqualsStringBoolean(char* value) {
    if (SEC_StringExtension_CompareCaseless(value, "true") == 0)
        return 2;

    return SEC_StringExtension_CompareCaseless(value, "false") == 0 ? 1 : 0;
}

int BE_ArgumentHandler_GetInt(BE_DynamicDictionary arguments, const char* name, int defaultValue) {
    if (arguments.keys.size != 0) {
        char* value = (char*) BE_ArgumentHandler_GetString(arguments, name, "");
        {
            int parsedValue = BE_ArgumentManager_EqualsStringBoolean(value);

            if (parsedValue != 0)
                return parsedValue == 2;
        }

        char* errored;
        int parsedValue = (int) strtol(value, &errored, 0); // FIXME: This isn't a good idea.

        if (errored == NULL)
            return parsedValue;
    }

    return defaultValue;
}

int BE_ArgumentHandler_GetBoolean(BE_DynamicDictionary arguments, const char* name, int defaultValue) {
    return BE_ArgumentHandler_GetInt(arguments, name, defaultValue) >= 1;
}

float BE_ArgumentHandler_GetFloat(BE_DynamicDictionary arguments, const char* name, float defaultValue) {
    if (arguments.keys.size != 0) {
        char* value = (char*) BE_ArgumentHandler_GetString(arguments, name, "");
        {
            int parsedValue = BE_ArgumentManager_EqualsStringBoolean(value);

            if (parsedValue != 0)
                return (float)(parsedValue == 2);
        }

        char* errored;
        float parsedValue = strtof(value, &errored);

        if (errored == NULL)
            return parsedValue;
    }

    return defaultValue;
}

const char* BE_ArgumentHandler_GetString(BE_DynamicDictionary arguments, const char* name, const char* defaultValue) {
    const char* value = BE_DynamicDictionary_GetElementViaKey(arguments, (void*) name, sizeof(char) * strlen(name) + 1);

    return value != NULL ? value : defaultValue;
}
SEC_CPP_SUPPORT_GUARD_END()
