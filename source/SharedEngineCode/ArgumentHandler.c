#include <string.h>

#include "SharedEngineCode/ArgumentHandler.h"
#include "SharedEngineCode/Internal/CppHeader.h"

CPP_GUARD_START()
    static int GetArgumentIndexImplementation(const char* argument) {
        if (addedArgumentsCount == 1)
            return -1;

        for (int i = 1; i < addedArgumentsCount; i++) {
            if (strcmp(argumentVector[i], argument) != 0)
                continue;

            return i;
        }

        return -1;
    }

    int GetArgumentIndex(const char* argument) {
        int dontParse = GetArgumentIndexImplementation("--dont-parse");
        int index = GetArgumentIndexImplementation(argument);

        if (dontParse == -1)
            dontParse = GetArgumentIndexImplementation("--");

        return dontParse == -1 || index < dontParse ? index : -1;
    }

    const char* GetArgumentValue(const char* argument) {
        int index = GetArgumentIndex(argument);

        return index != -1 && index != addedArgumentsCount - 1 ? argumentVector[index + 1] : NULL;
    }
CPP_GUARD_END()
