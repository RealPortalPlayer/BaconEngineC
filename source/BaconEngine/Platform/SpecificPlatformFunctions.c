// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "SharedEngineCode/Internal/CppSupport.h"
#include <stddef.h>

#include "SpecificPlatformFunctions.h"

SEC_CPP_SUPPORT_GUARD_START()
BE_SpecificPlatformFunctions rendererFunctions = {
    .GetTimer = NULL
};

BE_SpecificPlatformFunctions BE_SpecificPlatformFunctions_Get(void) {
    return rendererFunctions;
}

void BE_SpecificPlatformFunctions_InitializeWindow(BE_SpecificPlatformFunctions_Window functions) {
    rendererFunctions.windowFunctions = functions;
}

void BE_SpecificPlatformFunctions_SetDestroy(void (*Destroy)(void)) {
    rendererFunctions.Destroy = Destroy;
}

void BE_SpecificPlatformFunctions_SetGetTimer(double (*GetTimer)(void)) {
    rendererFunctions.GetTimer = GetTimer;
}

void BE_SpecificPlatformFunctions_InitializeRenderer(BE_SpecificPlatformFunctions_Renderer renderer) {
    rendererFunctions.rendererFunctions = renderer;
}
SEC_CPP_SUPPORT_GUARD_END()
