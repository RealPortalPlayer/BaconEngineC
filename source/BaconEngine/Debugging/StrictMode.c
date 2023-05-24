// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/BuiltInArguments.h>

#include "BaconEngine/Debugging/StrictMode.h"
#include "../InterfaceFunctions.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_Boolean BE_StrictMode_CheckLogsEnabled(void) {
#ifndef BE_CLIENT_BINARY
    static int enabled = -1;

    if (enabled == -1)
        enabled = !SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_DONT_PRINT_STRICT_CHECKS, SEC_BUILTINARGUMENTS_DONT_PRINT_STRICT_CHECKS_SHORT, 0);

    return enabled;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, void);
    return function();
#endif
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
