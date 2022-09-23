#include <SharedEngineCode/Internal/CppSupport.h>
#include <stddef.h>

#include "SpecificRendererFunctions.h"

SEC_CPP_SUPPORT_GUARD_START()
BE_SpecificRendererFunctions rendererFunctions = {
    .GetTimer = NULL
};

BE_SpecificRendererFunctions BE_SpecificRendererFunctions_Get(void) {
    return rendererFunctions;
}

void BE_SpecificRendererFunctions_InitializeWindow(BE_SpecificRendererFunctions_Window functions) {
    rendererFunctions.windowFunctions = functions;
}

void BE_SpecificRendererFunctions_SetDestroy(void (*Destroy)(void)) {
    rendererFunctions.Destroy = Destroy;
}

void BE_SpecificRendererFunctions_SetGetTimer(double (*GetTimer)(void)) {
    rendererFunctions.GetTimer = GetTimer;
}

void BE_SpecificRendererFunctions_InitializeRenderer(BE_SpecificRendererFunctions_Renderer renderer) {
    rendererFunctions.rendererFunctions = renderer;
}
SEC_CPP_SUPPORT_GUARD_END()
