// Purpose: Stores every layer.
// Created on: 8/8/22 @ 3:36 PM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "BaconEngine/Event.h"

SEC_CPP_SUPPORT_GUARD_START()
// TODO: Is this even needed? Can't we just return a name then?
typedef struct {
    int valid;
    const char* name;
} BE_Layer;

typedef enum {
    BE_LAYER_UPDATE_TYPE_BEFORE_RENDERING,
    BE_LAYER_UPDATE_TYPE_AFTER_RENDERING
} BE_Layer_UpdateTypes;

typedef struct {
    void (*OnStart)(void);
    void (*OnUpdate)(BE_Layer_UpdateTypes updateType);
    void (*OnToggle)(int enabled);
    int (*OnEvent)(BE_Event event);
    void (*OnStop)(void);
} BE_Layer_Functions;

void BE_Layer_Register(const char* name, int enabled, BE_Layer_Functions functions);
BE_Layer BE_Layer_Get(const char* name);
int BE_Layer_GetAmount(void);
int BE_Layer_GetAllocatedLayersAmount(void);
int BE_Layer_GetLayersReallocationAmount(void);
int BE_Layer_Toggle(const char* name, int enable);
int BE_Layer_IsToggled(const char* name);
SEC_CPP_SUPPORT_GUARD_END()
