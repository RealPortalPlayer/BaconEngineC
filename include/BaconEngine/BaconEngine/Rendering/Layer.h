// Purpose: Stores every layer.
// Created on: 8/8/22 @ 3:36 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/SDLSupport.h"
#include "BaconEngine/Event.h"

SEC_CPP_GUARD_START()
    typedef struct {
        int valid;
        int enabled;
        const char* name;
    } BE_Layer;

    typedef enum {
        LAYER_UPDATE_TYPE_BEFORE_RENDERING,
        LAYER_UPDATE_TYPE_AFTER_RENDERING
    } BE_Layer_UpdateTypes;

    typedef struct {
        void (*OnStart)(void);
        void (*OnUpdate)(BE_Layer_UpdateTypes updateType, double deltaTime);
        void (*OnToggle)(int enabled);
        int (*OnEvent)(BE_Event event);
        void (*OnStop)(void);
    } BE_Layer_Functions;

    void BE_Layer_InitializeLayers(void);
    void BE_Layer_Register(const char* name, int enabled, BE_Layer_Functions functions);
    BE_Layer BE_Layer_Get(const char* name);
    int BE_Layer_GetAmount(void);
    int BE_Layer_GetAllocatedLayersAmount(void);
    int BE_Layer_GetLayersReallocationAmount(void);
    int BE_Layer_Toggle(const char* name, int enable);
    void BE_Layer_OnUpdate(BE_Layer_UpdateTypes updateTypes, double deltaTime);
    int BE_Layer_OnEvent(SDL_Event sdlEvent);
    void BE_Layer_DestroyLayers(void);
SEC_CPP_GUARD_END()
