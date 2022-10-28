// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "SharedEngineCode/Internal/CppSupport.h"
#include <stddef.h>

#include "SpecificPlatformFunctions.h"

SEC_CPP_SUPPORT_GUARD_START()
BE_SpecificPlatformFunctions rendererFunctions;

BE_SpecificPlatformFunctions BE_SpecificPlatformFunctions_Get(void) {
    return rendererFunctions;
}

void BE_SpecificPlatformFunctions_Initialize(BE_SpecificPlatformFunctions functions) {
    rendererFunctions = functions;
}
SEC_CPP_SUPPORT_GUARD_END()
