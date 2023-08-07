// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <stdlib.h>

#include "BaconEngine/Console/ArgumentManager.h"
#include "../InterfaceFunctions.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static int BE_ArgumentManager_EqualsStringBoolean(const char* value) {
    if (SEC_String_Equals(value, "true", SEC_BOOLEAN_TRUE))
        return 2;

    return SEC_String_Equals(value, "false", SEC_BOOLEAN_TRUE) ? 1 : 0;
}
#endif

int BE_ArgumentManager_GetInteger(BE_DynamicDictionary arguments, const char* name, int defaultValue) {
#ifndef BE_CLIENT_BINARY
    if (arguments.keys.size != 0) {
        const char* value = BE_ArgumentManager_GetString(arguments, name, "");
        {
            int parsedValue = BE_ArgumentManager_EqualsStringBoolean(value);

            if (parsedValue != 0)
                return parsedValue == 2;
        }

        char* errored;
        int parsedValue = (int) strtol(value, &errored, 0);

        if (errored == NULL || strlen(errored) == 0)
            return parsedValue;
    }

    return defaultValue;
#else
    BE_INTERFACEFUNCTION(int, BE_DynamicDictionary, const char*, int);
    return function(arguments, name, defaultValue);
#endif
}

SEC_Boolean BE_ArgumentManager_GetBoolean(BE_DynamicDictionary arguments, const char* name, int defaultValue) {
#ifndef BE_CLIENT_BINARY
    return BE_ArgumentManager_GetInteger(arguments, name, defaultValue) >= 1;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicDictionary, const char*, int);
    return function(arguments, name, defaultValue);
#endif
}

float BE_ArgumentManager_GetFloat(BE_DynamicDictionary arguments, const char* name, float defaultValue) {
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
    BE_INTERFACEFUNCTION(float, BE_DynamicDictionary, const char*, float);
    return function(arguments, name, defaultValue);
#endif
}

const char* BE_ArgumentManager_GetString(BE_DynamicDictionary arguments, const char* name, const char* defaultValue) {
#ifndef BE_CLIENT_BINARY
    const char* value = BE_DynamicDictionary_GetElementValueViaKey(arguments, (void*) name, sizeof(char) + strlen(name));

    return value != NULL ? value : defaultValue;
#else
    BE_INTERFACEFUNCTION(const char*, BE_DynamicDictionary, const char*, const char*);
    return function(arguments, name, defaultValue);
#endif
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
