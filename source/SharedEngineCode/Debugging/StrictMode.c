// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>

#include "SharedEngineCode/BuiltInArguments.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Boolean SEC_StrictMode_CheckLogsEnabled(void) {
#ifdef BA_ALLOW_DEBUG_LOGS
    static int enabled = -1;

    if (enabled == -1)
        enabled = !BA_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_DONT_PRINT_STRICT_CHECKS, SEC_BUILTINARGUMENTS_DONT_PRINT_STRICT_CHECKS_SHORT, 0);

    return enabled;
#else
    return BA_BOOLEAN_FALSE;
#endif
}

BA_Boolean SEC_StrictMode_IsEnabled(void) {
    static int enabled = -1;

    if (enabled == -1)
        enabled = !BA_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_NO_STRICT, SEC_BUILTINARGUMENTS_NO_STRICT_SHORT, 0);

    return enabled;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
