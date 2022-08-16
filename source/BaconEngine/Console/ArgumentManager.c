#include <SharedEngineCode/Internal/CppHeader.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "BaconEngine/Console/ArgumentManager.h"

CPP_GUARD_START()
    int ArgumentEqualsTrueORFalse(char* value) {
        char* tempValue = (char*) value;
        {
            int i = 0;

            while (tempValue[i++] != '\0')
                tempValue[i] = (char) tolower(tempValue[i]);
        }

        if (strcmp(tempValue, "true") == 0)
            return 2;

        return strcmp(tempValue, "false") == 0 ? 1 : 0;
    }

    int BE_GetArgumentInt(ArgumentsDictionary arguments, const char* name, int defaultValue) {
        if (arguments.size != 0) {
            char* value = (char*) BE_GetArgumentString(arguments, name, "");
            {
                int parsedValue = ArgumentEqualsTrueORFalse(value);

                if (parsedValue != 0)
                    return parsedValue == 2;
            }

            char* errored;
            int parsedValue = (int) strtol(value, &errored, 0);

            if (errored == NULL)
                return parsedValue;
        }

        return defaultValue;
    }

    int BE_GetArgumentBoolean(ArgumentsDictionary arguments, const char* name, int defaultValue) {
        return BE_GetArgumentInt(arguments, name, defaultValue) >= 1;
    }

    float BE_GetArgumentFloat(ArgumentsDictionary arguments, const char* name, float defaultValue) {
        if (arguments.size != 0) {
            char* value = (char*) BE_GetArgumentString(arguments, name, "");
            {
                int parsedValue = ArgumentEqualsTrueORFalse(value);

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

    const char* BE_GetArgumentString(ArgumentsDictionary arguments, const char* name, const char* defaultValue) {
        if (arguments.size != 0) {
            for (int i = 0; i < (int) arguments.size; i++) {
                if (strcmp(arguments.keys[i], name) != 0)
                    continue;

                return arguments.values[i];
            }
        }

        return defaultValue;
    }
CPP_GUARD_END()
