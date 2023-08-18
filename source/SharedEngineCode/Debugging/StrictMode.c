// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "SharedEngineCode/ArgumentHandler.h"
#include "SharedEngineCode/BuiltInArguments.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_Boolean SEC_StrictMode_CheckLogsEnabled(void) {
#ifdef BE_ALLOW_DEBUG_LOGS
    static int enabled = -1;

    if (enabled == -1)
        enabled = !SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_DONT_PRINT_STRICT_CHECKS, SEC_BUILTINARGUMENTS_DONT_PRINT_STRICT_CHECKS_SHORT, 0);

    return enabled;
#else
    return SEC_BOOLEAN_FALSE;
#endif
}

SEC_Boolean SEC_StrictMode_IsEnabled(void) {
    static int enabled = -1;

    if (enabled == -1)
        enabled = !SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_NO_STRICT, SEC_BUILTINARGUMENTS_NO_STRICT_SHORT, 0);

    return enabled;
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
