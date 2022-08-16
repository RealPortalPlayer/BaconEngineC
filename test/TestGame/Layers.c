#include <BaconEngine/Rendering/Layer.h>
#include <BaconEngine/Rendering/Renderer.h>
#include <BaconEngine/Console/Console.h>
#include <stdlib.h>
#include <BaconEngine/Rendering/UI.h>

#include "Layers.h"

int down = 0;
BE_Color3I color = {0, 0, 0};

void ColorOnUpdate(BE_LayerUpdateTypes updateType, double deltaTime);
int ColorOnEvent(BE_Event event);
int ColorModifierOnEvent(BE_Event event);
int UIOnEvent(BE_Event event);

void InitializeTestLayers(void) {
    BE_RegisterLayer("UI", 1, (BE_ClientLayerFunctions) {
            .LayerOnEvent = &UIOnEvent
    });
    BE_RegisterLayer("ColorModifier", 1, (BE_ClientLayerFunctions) {
            .LayerOnEvent = &ColorModifierOnEvent
    });
    BE_RegisterLayer("Color", 1, (BE_ClientLayerFunctions) {
            .LayerOnUpdate = &ColorOnUpdate,
            .LayerOnEvent = &ColorOnEvent
    });
}

int enabled = 1;

int UIOnEvent(BE_Event event) {
    if (event.type == BE_EVENT_TYPE_KEYBOARD_KEY_DOWN && event.unionVariables.keyboard.key == BE_KEY_CODE_A) {
        BE_ToggleUIRendering((enabled = !enabled));
        return 1;
    }

    return 0;
}

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

void ColorOnUpdate(BE_LayerUpdateTypes updateType, double deltaTime) {
    if (updateType != LAYER_UPDATE_TYPE_BEFORE_RENDERING)
        return;

    int times = 1;

    if (!down)
        times = -1;

    color.r += times * (int)(1000 * deltaTime);
    color.g += times * (int)(1000 * deltaTime);
    color.b += times * (int)(1000 * deltaTime);

    color.r = MIN(255, MAX(color.r, 0));
    color.g = MIN(255, MAX(color.g, 0));
    color.b = MIN(255, MAX(color.b, 0));

    BE_SetClearColor((BE_Color3U) {color.r, color.g, color.b});
}

int ColorOnEvent(BE_Event event) {
#ifndef BACON_ENGINE_DISABLE_SDL
    switch (event.type) {
        case BE_EVENT_TYPE_KEYBOARD_KEY_UP:
            if (event.unionVariables.keyboard.key == BE_KEY_CODE_R ||
                event.unionVariables.keyboard.key == BE_KEY_CODE_T ||
                event.unionVariables.keyboard.key == BE_KEY_CODE_Y ||
                event.unionVariables.keyboard.key == BE_KEY_CODE_D)
                return 0;

            down = 0;
            return 1;

        case BE_EVENT_TYPE_KEYBOARD_KEY_DOWN:
            down = 1;
            return 1;

        default:
            break;
    }
#endif

    return 0;
}

int ColorModifierOnEvent(BE_Event event) {
    if (event.type == BE_EVENT_TYPE_KEYBOARD_KEY_UP && event.unionVariables.keyboard.key == BE_KEY_CODE_R)
        return 1;

    if (event.type == BE_EVENT_TYPE_KEYBOARD_KEY_DOWN) {
        if (event.unionVariables.keyboard.key == BE_KEY_CODE_R) {
            BE_ToggleLayer("Color", 0);

            return 1;
        }

        if (event.unionVariables.keyboard.key == BE_KEY_CODE_T) {
            BE_ToggleLayer("Color", 1);

            return 1;
        }

        if (event.unionVariables.keyboard.key == BE_KEY_CODE_Y) {
            color.r = rand() % 255;
            color.g = rand() % 255;
            color.b = rand() % 255;

            return 1;
        }

        if (event.unionVariables.keyboard.key == BE_KEY_CODE_D) {
            BE_ExecuteCommand("debuginfo");
            return 1;
        }
    }

    return 0;
}