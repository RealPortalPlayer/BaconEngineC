// Copyright (c) 2022, PortalPlayer
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "SharedEngineCode/Internal/CppSupport.h"
#include "SharedEngineCode/ANSI.h"
#include "SharedEngineCode/ArgumentHandler.h"
#include "SharedEngineCode/BuiltInArguments.h"

SEC_CPP_SUPPORT_GUARD_START()
const char* ansiCodesCharArray[SEC_ANSI_CODE_SIZE] = {
    "\033[1m", "\033[4m", "\033[5m", "\033[7m",
    "\033[m",
    "\033[0;30m", "\033[0;31m", "\033[0;32m", "\033[0;33m",
    "\033[0;34m", "\033[0;35m", "\033[0;36m", "\033[0;37m",
    "\033[0;39m", "\033[1;30m", "\033[1;31m", "\033[1;32m",
    "\033[1;33m", "\033[1;34m", "\033[1;35m", "\033[1;36m",
    "\033[1;37m", "\033[1;39m", "\033[0;40m", "\033[0;41m",
    "\033[0;42m", "\033[0;43m", "\033[0;44m", "\033[0;45m",
    "\033[0;46m", "\033[0;47m", "\033[0;49m"
};

int SEC_ANSI_IsEnabled(void) {
    static int enabled = -1;

    if (enabled == -1)
        enabled = !SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_DISABLE_ANSI_COLORING, SEC_BUILTINARGUMENTS_DISABLE_ANSI_COLORING_SHORT, 0);

    return enabled;
}

const char* SEC_ANSI_ConvertCodeToString(SEC_ANSI_Codes ansi) {
    return SEC_ANSI_IsEnabled() && ansi < SEC_ANSI_CODE_SIZE ? ansiCodesCharArray[ansi] : "";
}
SEC_CPP_SUPPORT_GUARD_END()
