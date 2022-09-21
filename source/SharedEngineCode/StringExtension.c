#include <ctype.h>
#include <stddef.h>

#include "SharedEngineCode/Internal/CppHeader.h"
#include "SharedEngineCode/StringExtension.h"

SEC_CPP_GUARD_START()
    int SEC_StringExtension_CompareCaseless(const char* string1, const char* string2) {
        if (string1 == string2)
            return 1;

        while ((*string1 != '\0' && *string2 != '\0') && tolower(*string1) == tolower(*string2)) {
            string1++;
            string2++;
        }

        return tolower(*string1) == tolower(*string2);
    }

    void SEC_StringExtension_ToLowerString(char* string) {
        if (string == NULL)
            return;

        while (*string != '\0') {
            *string = (char) tolower(*string);
            string++;
        }
    }

    void SEC_StringExtension_ToUpperString(char* string) {
        if (string == NULL)
            return;

        while (*string != '\0') {
            *string = (char) toupper(*string);
            string++;
        }
    }
SEC_CPP_GUARD_END()
