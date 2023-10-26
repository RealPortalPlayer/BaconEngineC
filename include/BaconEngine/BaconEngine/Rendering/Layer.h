// Purpose: Stores every layer.
// Created on: 8/8/22 @ 3:36 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>
#include <BaconAPI/String.h>

#include "BaconEngine/Event.h"
#include "BaconEngine/BinaryExport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef enum {
    BE_LAYER_UPDATE_TYPE_BEFORE_RENDERING,
    BE_LAYER_UPDATE_TYPE_AFTER_RENDERING
} BE_Layer_UpdateTypes;

typedef struct {
    void (*OnStart)(void);
    void (*OnUpdate)(BE_Layer_UpdateTypes updateType);
    void (*OnToggle)(BA_Boolean enabled);
    BA_Boolean (*OnEvent)(BE_Event event);
    void (*OnStop)(void);
} BE_Layer_Functions;

BE_BINARYEXPORT int BE_Layer_GetAmount(void);
BE_BINARYEXPORT int BE_Layer_GetAllocatedLayersAmount(void);
BE_BINARYEXPORT int BE_Layer_GetLayersReallocationAmount(void);
BE_BINARYEXPORT BA_Boolean BE_Layer_Toggle(const char* name, BA_Boolean enable);
BE_BINARYEXPORT BA_Boolean BE_Layer_IsToggled(const char* name);
BE_BINARYEXPORT BA_Boolean BE_Layer_Exists(const char* name);

BE_BINARYEXPORT void BE_Layer_Register(const char* name, BA_Boolean enabled, BE_Layer_Functions functions);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
