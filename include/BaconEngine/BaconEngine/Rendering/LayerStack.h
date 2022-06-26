// Purpose: The stack of layers.
// Created on: 4/3/22 @ 11:18 PM

#pragma once

#include <SDL.h>

#define MAX_LAYERS 65025 // 255 * 255

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct Layer {
        const char* name;
        int enabled;
        void (*OnToggle)(struct Layer* this, int enabled);
        void (*OnStart)(struct Layer* this);
        void (*OnEvent)(struct Layer* this, SDL_Event event);
        void (*OnBeforeRender)(struct Layer* this, double deltaTime);
        void (*OnAfterRender)(struct Layer* this, double deltaTime);
        void (*OnShutdown)(struct Layer* this);
    } Layer;

    void OnToggleDummy(struct Layer* this, int enabled);
    void OnStartShutdownDummy(struct Layer* this);
    void OnEventDummy(struct Layer* this, SDL_Event event);
    void OnRenderDummy(struct Layer* this, double deltaTime);
    void InitializeLayers(void);
    void DestroyLayers(void);
    Layer* CreateLayer(const char* name, void (*OnToggle)(struct Layer* this, int enabled),
                       void (*OnStart)(struct Layer* this), void (*OnEvent)(struct Layer* this, SDL_Event event),
                       void (*OnBeforeRender)(struct Layer* this, double deltaTime),
                       void (*OnAfterRender)(struct Layer* this, double deltaTime), void (*OnShutdown)(struct Layer* this));
    Layer* GetLayer(const char* name);
    int RemoveLayer(const char* name);
    int GetLayerCount(void);
    Layer** GetLayers(void);

#ifdef __cplusplus
};
#endif