// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/Internal/CPlusPlusSupport.h>

#include "SpecificPlatformFunctions.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_SpecificPlatformFunctions rendererFunctions;

BE_SpecificPlatformFunctions BE_SpecificPlatformFunctions_Get(void) {
    return rendererFunctions;
}

void BE_SpecificPlatformFunctions_Initialize(BE_SpecificPlatformFunctions functions) {
    rendererFunctions = functions;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
