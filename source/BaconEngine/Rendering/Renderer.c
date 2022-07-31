#include <SDL.h>
#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/Window.h"

CPP_GUARD_START()
    Vector4U clearColor = {0, 0, 0, 0};

    void ClearScreen(void) {
        if (GetInternalSDLRenderer() == NULL)
            return;

        SDL_SetRenderDrawColor(GetInternalSDLRenderer(), clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        SDL_RenderClear(GetInternalSDLRenderer());
    }
CPP_GUARD_END()