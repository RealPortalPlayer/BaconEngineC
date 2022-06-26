#include <SDL.h>

#include "BaconEngine/Rendering/Renderer.h"
#include "BaconEngine/Rendering/Window.h"

Vector4U clearColor = {0, 0, 0, 0};

void ClearScreen(void) {
    if (GetSDLRenderer() == NULL)
        return;

    SDL_SetRenderDrawColor(GetSDLRenderer(), clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    SDL_RenderClear(GetSDLRenderer());
}