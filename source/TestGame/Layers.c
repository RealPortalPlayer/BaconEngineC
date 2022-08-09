#include <BaconEngine/Rendering/Layer.h>
#include <BaconEngine/Rendering/Renderer.h>
#include <BaconEngine/Console/Console.h>

#include "Layers.h"

int down = 0;
Color3U color = {0, 0, 0};

void ColorOnUpdate(LayerUpdateTypes updateType);
int ColorOnEvent(SDL_Event event);
int ColorModifierOnEvent(SDL_Event event);

void InitializeTestLayers(void) {
    RegisterLayer("ColorModifier", 1, (ClientLayerFunctions) {
            .LayerOnEvent = &ColorModifierOnEvent
    });
    RegisterLayer("Color", 1, (ClientLayerFunctions) {
        .LayerOnUpdate = &ColorOnUpdate,
        .LayerOnEvent = &ColorOnEvent
    });
}

void ColorOnUpdate(LayerUpdateTypes updateType) {
    if (updateType != LAYER_UPDATE_TYPE_BEFORE_RENDERING)
        return;

    SetClearColor(color);

    if (down) {
        if (color.r != 255)
            color.r++;

        if (color.g != 255)
            color.g++;

        if (color.b != 255)
            color.b++;

        return;
    }

    if (color.r != 0)
        color.r--;

    if (color.g != 0)
        color.g--;

    if (color.b != 0)
        color.b--;
}

int ColorOnEvent(SDL_Event event) {
    switch (event.type) {
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_r ||
                event.key.keysym.sym == SDLK_t ||
                event.key.keysym.sym == SDLK_y ||
                event.key.keysym.sym == SDLK_d)
                return 0;

            down = 0;
            return 1;

        case SDL_KEYDOWN:
            down = 1;
            return 1;
    }

    return 0;
}

int ColorModifierOnEvent(SDL_Event event) {
    if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_r)
        return 1;

    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_r) {
            ToggleLayer("Color", 0);

            return 1;
        }

        if (event.key.keysym.sym == SDLK_t) {
            ToggleLayer("Color", 1);

            return 1;
        }

        if (event.key.keysym.sym == SDLK_y) {
            color.r = rand() % 255;
            color.g = rand() % 255;
            color.b = rand() % 255;

            return 1;
        }

        if (event.key.keysym.sym == SDLK_d) {
            ExecuteCommand("debuginfo");
            return 1;
        }
    }

    return 0;
}