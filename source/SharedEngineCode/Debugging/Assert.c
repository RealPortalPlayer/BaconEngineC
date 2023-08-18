// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "SharedEngineCode/ArgumentHandler.h"
#include "SharedEngineCode/BuiltInArguments.h"
#include "SharedEngineCode/Debugging/Assert.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_Boolean SEC_Assert_CheckLogsEnabled(void) {
#ifdef BE_ALLOW_DEBUG_LOGS
    static int enabled = -1;

    if (enabled == -1)
        enabled = !SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_DONT_PRINT_ASSERT_CHECKS, SEC_BUILTINARGUMENTS_DONT_PRINT_ASSERT_CHECKS_SHORT, 0);

    return enabled;
#else
    return SEC_BOOLEAN_FALSE;
#endif
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
