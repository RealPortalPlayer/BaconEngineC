// Purpose: Stores every layer.
// Created on: 8/8/22 @ 3:36 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>
#include <SDL_events.h>

CPP_GUARD_START()
    typedef struct {
        int valid;
        int enabled;
        const char* name;
    } ClientLayer;

    typedef enum {
        LAYER_UPDATE_TYPE_BEFORE_RENDERING,
        LAYER_UPDATE_TYPE_AFTER_RENDERING
    } LayerUpdateTypes;

    typedef struct {
        void (*LayerOnStart)(void);
        void (*LayerOnUpdate)(LayerUpdateTypes updateType, double deltaTime);
        void (*LayerOnToggle)(int enabled);
        int (*LayerOnEvent)(SDL_Event event);
        void (*LayerOnStop)(void);
    } ClientLayerFunctions;

    void InitializeLayers(void);
    void RegisterLayer(const char* name, int enabled, ClientLayerFunctions functions);
    ClientLayer GetLayer(const char* name);
    int GetLayerAmount(void);
    int GetAllocatedLayersAmount(void);
    int GetLayerReallocationAmount(void);
    int ToggleLayer(const char* name, int enable);
    void LayerOnUpdate(LayerUpdateTypes layerUpdateType, double deltaTime);
    int LayerOnEvent(SDL_Event event);
    void DestroyLayers(void);
CPP_GUARD_END()
