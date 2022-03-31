#include <string.h>

#include "SharedEngineCode/ArgumentHandler.h"

#ifdef __cplusplus
extern "C" {
#endif

static int GetArgumentIndexImplementation(const char* argument) {
    if (argumentCount == 1)
        return -1;

    for (int i = 1; i < argumentCount; i++) {
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

    return index != -1 && index != argumentCount - 1 ? argumentVector[index + 1] : NULL;
}

#ifdef __cplusplus
};
#endif