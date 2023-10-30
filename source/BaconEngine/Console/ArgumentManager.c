// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "BaconEngine/Console/ArgumentManager.h"
#include "../InterfaceFunctions.h"

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
BA_CPLUSPLUS_SUPPORT_GUARD_END()
