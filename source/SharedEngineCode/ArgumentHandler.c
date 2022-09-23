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

int SEC_ArgumentHandler_GetIndexWithShort(const char* argument, const char* shortArgument, int ignoreDontParse, int* argumentIndex, int* shortIndex) {
    int foundArgumentIndex = SEC_ArgumentHandler_GetIndex(argument, ignoreDontParse);
    int foundShortIndex = SEC_ArgumentHandler_GetIndex(shortArgument, ignoreDontParse);

    if (argumentIndex != NULL)
        *argumentIndex = foundArgumentIndex;

    if (shortIndex != NULL)
        *shortIndex = foundShortIndex;

    return foundArgumentIndex != -1 || foundShortIndex != -1;
}

char* SEC_ArgumentHandler_GetValue(const char* argument, int ignoreDontParse) {
    int index = SEC_ArgumentHandler_GetIndex(argument, ignoreDontParse);

    return index != -1 && index != addedArgumentsCount - 1 ? argumentVector[index + 1] : NULL;
}

int SEC_ArgumentHandler_GetValueWithShort(const char* argument, const char* shortArgument, int ignoreDontParse, char** argumentValue, char** shortValue) {
    int argumentIndex;
    int shortIndex;
    int returnValue = 0;

    if (!SEC_ArgumentHandler_GetIndexWithShort(argument, shortArgument, ignoreDontParse, &argumentIndex, &shortIndex))
        return 0;

    if (argumentValue != NULL && argumentIndex != -1 && argumentIndex != addedArgumentsCount - 1) {
        *argumentValue = argumentVector[argumentIndex + 1];
        returnValue++;
    }

    if (shortValue != NULL && shortIndex != -1 && shortIndex != addedArgumentsCount - 1) {
        *shortValue = argumentValue[argumentIndex + 1];
        returnValue++;
    }

    return returnValue;
}
SEC_CPP_SUPPORT_GUARD_END()
