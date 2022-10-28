// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/BuiltInArguments.h>

#include "BaconEngine/Debugging/Assert.h"

SEC_CPP_SUPPORT_GUARD_START()
SEC_Boolean BE_Assert_CheckLogsEnabled(void) {
    static int enabled = -1;

    if (enabled == -1)
        enabled = !SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_DONT_PRINT_ASSERT_CHECKS, SEC_BUILTINARGUMENTS_DONT_PRINT_ASSERT_CHECKS_SHORT, 0);

    return enabled;
}
SEC_CPP_SUPPORT_GUARD_END()
