#include <SharedEngineCode/Internal/CppHeader.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "BaconEngine/Console/ArgumentManager.h"

#define STRTO_CUSTOM_IMPLEMENTATION(type, strto) \
const char* value = GetArgumentString(arguments, name, ""); \
char* endPointer;                                \
if (value[0] == '\0')                            \
    return defaultValue;                         \
type parsed = (type) (strto);                    \
return endPointer[0] == '\0' ? parsed : defaultValue

#define STRTO_IMPLEMENTATION(type, strtot) STRTO_CUSTOM_IMPLEMENTATION(type, strto ## strtot(value, &endPointer, 0))

CPP_GUARD_START()
    int ContainsArgument(Arguments arguments, const char* name) {
        return GetArgumentString(arguments, name, "")[0] != '\0';
    }

    short GetArgumentShort(Arguments arguments, const char* name, short defaultValue) {
        STRTO_IMPLEMENTATION(short, l);
    }

    unsigned short GetArgumentUnsignedShort(Arguments arguments, const char* name, unsigned short defaultValue) {
        STRTO_IMPLEMENTATION(unsigned short, ul);
    }

    int GetArgumentInt(Arguments arguments, const char* name, int defaultValue) {
        char* value = (char*) GetArgumentString(arguments, name, "");
        char* endPointer;

        if (value[0] == '\0')
            return defaultValue;

        for (int i = 0; value[i]; i++)
            value[i] = (char) tolower(value[i]);

        {
            int true = strcmp(value, "true");

            if (true == 0 || strcmp(value, "false") == 0)
                return true == 0;
        }

        int parsed = (int) strtol(value, &endPointer, 0);

        return endPointer[0] == '\0' ? parsed : defaultValue;
    }

    unsigned GetArgumentUnsigned(Arguments arguments, const char* name, unsigned defaultValue) {
        STRTO_IMPLEMENTATION(unsigned, ul);
    }

    long GetArgumentLong(Arguments arguments, const char* name, long defaultValue) {
        STRTO_IMPLEMENTATION(long, l);
    }

    unsigned long GetArgumentUnsignedLong(Arguments arguments, const char* name, unsigned long defaultValue) {
        STRTO_IMPLEMENTATION(unsigned long, ul);
    }

    long long GetArgumentLongLong(Arguments arguments, const char* name, long long defaultValue) {
        STRTO_IMPLEMENTATION(long long, ll);
    }

    unsigned long long GetArgumentUnsignedLongLong(Arguments arguments, const char* name, unsigned long long defaultValue) {
        STRTO_IMPLEMENTATION(unsigned long long, ull);
    }

    float GetArgumentFloat(Arguments arguments, const char* name, float defaultValue) {
        STRTO_CUSTOM_IMPLEMENTATION(float, strtof(value, &endPointer));
    }

    double GetArgumentDouble(Arguments arguments, const char* name, double defaultValue) {
        STRTO_CUSTOM_IMPLEMENTATION(double, strtod(value, &endPointer));
    }

    long double GetArgumentLongDouble(Arguments arguments, const char* name, long double defaultValue) {
        STRTO_CUSTOM_IMPLEMENTATION(long double, strtold(value, &endPointer));
    }

    const char* GetArgumentString(Arguments arguments, const char* name, const char* defaultValue) {
        for (int i = 0; i < MAX_ARGUMENTS; i++) {
            if (arguments.keys[i] == NULL)
                return defaultValue;

            if (strcmp(arguments.keys[i], name) != 0)
                continue;

            return arguments.values[i];
        }

        return defaultValue;
    }

    int GetArgumentBoolean(Arguments arguments, const char* name, int defaultValue) {
        return GetArgumentInt(arguments, name, defaultValue) >= 1;
    }
CPP_GUARD_END()