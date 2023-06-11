// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>

#include "SharedEngineCode/ArgumentHandler.h"
#include "SharedEngineCode/Internal/CPlusPlusSupport.h"
#include "SharedEngineCode/BuiltInArguments.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
static int secArgumentHandlerAddedArgumentsCount;
static char** secArgumentHandlerArgumentVector;

void SEC_ArgumentHandler_Initialize(int argc, char** argv) {
    secArgumentHandlerAddedArgumentsCount = argc;
    secArgumentHandlerArgumentVector = argv;
}

int SEC_ArgumentHandler_GetCount(void) {
    return secArgumentHandlerAddedArgumentsCount;
}

char** SEC_ArgumentHandler_GetVector(void) {
    return secArgumentHandlerArgumentVector;
}

static int SEC_ArgumentHandler_GetArgumentIndexImplementation(const char* argument) {
    for (int i = 1; i < secArgumentHandlerAddedArgumentsCount; i++) {
        if (strcmp(secArgumentHandlerArgumentVector[i], argument) != 0)
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

    return index != -1 && index != secArgumentHandlerAddedArgumentsCount - 1 ? secArgumentHandlerArgumentVector[index + 1] : NULL;
}

int SEC_ArgumentHandler_GetInformationWithShort(const char* argument, const char* shortArgument, SEC_Boolean ignoreDontParse,
                                                SEC_ArgumentHandler_ShortResults* results) {
    int returnResult = 0;

    results->argumentIndex = SEC_ArgumentHandler_GetIndex(argument, ignoreDontParse);
    results->shortIndex = SEC_ArgumentHandler_GetIndex(shortArgument, ignoreDontParse);

    if (results->argumentIndex != -1 && results->argumentIndex != secArgumentHandlerAddedArgumentsCount - 1) {
        results->argumentValue = secArgumentHandlerArgumentVector[results->argumentIndex + 1];
        results->value = &results->argumentValue;
        results->index = &results->argumentIndex;
        returnResult++;
    }

    if (results->shortIndex != -1 && results->shortIndex != secArgumentHandlerAddedArgumentsCount - 1) {
        results->shortValue = secArgumentHandlerArgumentVector[results->shortIndex + 1];
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
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
