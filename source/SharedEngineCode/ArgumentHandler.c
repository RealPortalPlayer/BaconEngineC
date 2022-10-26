// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>

#include "SharedEngineCode/ArgumentHandler.h"
#include "SharedEngineCode/Internal/CppSupport.h"
#include "SharedEngineCode/BuiltInArguments.h"

SEC_CPP_SUPPORT_GUARD_START()
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

int SEC_ArgumentHandler_GetArgumentIndexImplementation(const char* argument) {
    for (int i = 1; i < addedArgumentsCount; i++) {
        if (strcmp(argumentVector[i], argument) != 0)
            continue;

        return i;
    }

    return -1;
}

int SEC_ArgumentHandler_GetIndex(const char* argument, int ignoreDontParse) {
    static int dontParse = -2;

    if (dontParse == -2)
        dontParse = SEC_ArgumentHandler_GetArgumentIndexImplementation(SEC_BUILTINARGUMENTS_DONT_PARSE);

    int index = SEC_ArgumentHandler_GetArgumentIndexImplementation(argument);

    return ignoreDontParse || dontParse == -1 || index < dontParse ? index : -1;
}

char* SEC_ArgumentHandler_GetValue(const char* argument, int ignoreDontParse) {
    int index = SEC_ArgumentHandler_GetIndex(argument, ignoreDontParse);

    return index != -1 && index != addedArgumentsCount - 1 ? argumentVector[index + 1] : NULL;
}

int SEC_ArgumentHandler_GetInfoWithShort(const char* argument, const char* shortArgument, int ignoreDontParse,
                                         SEC_ArgumentHandler_ShortResults* results) {
    int returnResult = 0;

    results->argumentIndex = SEC_ArgumentHandler_GetIndex(argument, ignoreDontParse);
    results->shortIndex = SEC_ArgumentHandler_GetIndex(shortArgument, ignoreDontParse);

    if (results->argumentIndex != -1 && results->argumentIndex != addedArgumentsCount - 1) {
        results->argumentValue = argumentVector[results->argumentIndex + 1];
        results->value = &results->argumentValue;
        results->index = &results->argumentIndex;
        returnResult++;
    }

    if (results->shortIndex != -1 && results->shortIndex != addedArgumentsCount - 1) {
        results->shortValue = argumentVector[results->shortIndex + 1];
        results->value = &results->shortValue;
        results->index = &results->shortIndex;
        returnResult++;
    }

    return returnResult;
}

int SEC_ArgumentHandler_ContainsArgumentOrShort(const char* argument, const char* shortArgument, int ignoreDontParse) {
    return SEC_ArgumentHandler_GetIndex(argument, ignoreDontParse) != -1 ||
           SEC_ArgumentHandler_GetIndex(shortArgument, ignoreDontParse) != -1;
}
SEC_CPP_SUPPORT_GUARD_END()
