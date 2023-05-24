// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconEngine/Rendering/Layer.h>
#include <BaconEngine/Rendering/Renderer.h>
#include <BaconEngine/Console/Console.h>
#include <stdlib.h>
#include <BaconEngine/DeltaTime.h>

#include "Layers.h"

int down = 0;
BE_Color3_Integer color = {0, 0, 0};

void ColorOnUpdate(BE_Layer_UpdateTypes updateType);
int ColorOnEvent(BE_Event event);
int ColorModifierOnEvent(BE_Event event);
int UIOnEvent(BE_Event event);

void InitializeTestLayers(void) {
    BE_Layer_Register("UI", 1, (BE_Layer_Functions) {
            .OnEvent = &UIOnEvent
    });
    BE_Layer_Register("ColorModifier", 1, (BE_Layer_Functions) {
            .OnEvent = &ColorModifierOnEvent
    });
    BE_Layer_Register("Color", 1, (BE_Layer_Functions) {
            .OnUpdate = &ColorOnUpdate,
            .OnEvent = &ColorOnEvent
    });
}

int enabled = 1;

int UIOnEvent(BE_Event event) {
    if (event.type == BE_EVENT_TYPE_KEYBOARD_KEY_DOWN && event.data.keyboard.key == BE_KEYBOARD_KEY_CODE_A) {
//        BE_UI_ToggleWindowRendering((enabled = !enabled));
        return 1;
    }

    return 0;
}

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

void ColorOnUpdate(BE_Layer_UpdateTypes updateType) {
    if (updateType != BE_LAYER_UPDATE_TYPE_BEFORE_RENDERING)
        return;

    int times = 1;

    if (!down)
        times = -1;

    double deltaTime = BE_DeltaTime_GetMilliseconds();

    color.r += times * (int)(1000 * deltaTime);
    color.g += times * (int)(1000 * deltaTime);
    color.b += times * (int)(1000 * deltaTime);

    color.r = MIN(255, MAX(color.r, 0));
    color.g = MIN(255, MAX(color.g, 0));
    color.b = MIN(255, MAX(color.b, 0));

    BE_Renderer_SetClearColor((BE_Color3_Unsigned) {color.r, color.g, color.b});
}

int ColorOnEvent(BE_Event event) {
#ifndef BACON_ENGINE_DISABLE_SDL
    switch (event.type) {
        case BE_EVENT_TYPE_KEYBOARD_KEY_UP:
            if (event.data.keyboard.key == BE_KEYBOARD_KEY_CODE_R ||
                event.data.keyboard.key == BE_KEYBOARD_KEY_CODE_T ||
                event.data.keyboard.key == BE_KEYBOARD_KEY_CODE_Y ||
                event.data.keyboard.key == BE_KEYBOARD_KEY_CODE_D)
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
    if (event.type == BE_EVENT_TYPE_KEYBOARD_KEY_UP && event.data.keyboard.key == BE_KEYBOARD_KEY_CODE_R)
        return 1;

    if (event.type == BE_EVENT_TYPE_KEYBOARD_KEY_DOWN) {
        if (event.data.keyboard.key == BE_KEYBOARD_KEY_CODE_R) {
            BE_Layer_Toggle("Color", 0);

            return 1;
        }

        if (event.data.keyboard.key == BE_KEYBOARD_KEY_CODE_T) {
            BE_Layer_Toggle("Color", 1);

            return 1;
        }

        if (event.data.keyboard.key == BE_KEYBOARD_KEY_CODE_Y) {
            color.r = rand() % 255;
            color.g = rand() % 255;
            color.b = rand() % 255;

            return 1;
        }

        if (event.data.keyboard.key == BE_KEYBOARD_KEY_CODE_D) {
            BE_Console_ExecuteCommand("debuginfo");
            return 1;
        }
    }

    return 0;
}