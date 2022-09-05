#include <string.h>

#include "SharedEngineCode/ArgumentHandler.h"
#include "SharedEngineCode/Internal/CppHeader.h"

SEC_CPP_GUARD_START()
    int addedArgumentsCount;
    char** argumentVector;

    void SEC_ArgumentHandler_Initialize(int argc, char** argv) {
        addedArgumentsCount = argc;
        argumentVector = argv;
    }

    int SEC_ArgumentHandler_GetCount(void) {
        return addedArgumentsCount;
    }

    char** SEC_ArgumentHandler_GetVector(void) {
        return argumentVector;
    }

    int GetArgumentIndexImplementation(const char* argument) {
        if (addedArgumentsCount == 1)
            return -1;

        for (int i = 1; i < addedArgumentsCount; i++) {
            if (strcmp(argumentVector[i], argument) != 0)
                continue;

            return i;
        }

        return -1;
    }

    int SEC_ArgumentHandler_GetIndex(const char* argument) {
        static int dontParse = -2;

        if (dontParse == -2) {
            dontParse = GetArgumentIndexImplementation("--dont-parse");

            if (dontParse == -1)
                dontParse = GetArgumentIndexImplementation("--");
        }

        int index = GetArgumentIndexImplementation(argument);

        return dontParse == -1 || index < dontParse ? index : -1;
    }

    const char* SEC_ArgumentHandler_GetValue(const char* argument) {
        int index = SEC_ArgumentHandler_GetIndex(argument);

        return index != -1 && index != addedArgumentsCount - 1 ? argumentVector[index + 1] : NULL;
    }
SEC_CPP_GUARD_END()
