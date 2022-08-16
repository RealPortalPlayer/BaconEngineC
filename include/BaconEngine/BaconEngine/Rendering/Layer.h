// Purpose: Stores every layer.
// Created on: 8/8/22 @ 3:36 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/SDLSupport.h"
#include "BaconEngine/Event.h"

CPP_GUARD_START()
    typedef struct {
        int valid;
        int enabled;
        const char* name;
    } BE_ClientLayer;

    typedef enum {
        LAYER_UPDATE_TYPE_BEFORE_RENDERING,
        LAYER_UPDATE_TYPE_AFTER_RENDERING
    } BE_LayerUpdateTypes;

    typedef struct {
        void (*LayerOnStart)(void);
        void (*LayerOnUpdate)(BE_LayerUpdateTypes updateType, double deltaTime);
        void (*LayerOnToggle)(int enabled);
        int (*LayerOnEvent)(BE_Event event);
        void (*LayerOnStop)(void);
    } BE_ClientLayerFunctions;

    void BE_InitializeLayers(void);
    void BE_RegisterLayer(const char* name, int enabled, BE_ClientLayerFunctions functions);
    BE_ClientLayer BE_GetLayer(const char* name);
    int BE_GetLayerAmount(void);
    int BE_GetAllocatedLayersAmount(void);
    int BE_GetLayerReallocationAmount(void);
    int BE_ToggleLayer(const char* name, int enable);
    void BE_LayerOnUpdate(BE_LayerUpdateTypes layerUpdateType, double deltaTime);
    int BE_LayerOnEvent(SDL_Event sdlEvent);
    void BE_DestroyLayers(void);
CPP_GUARD_END()
