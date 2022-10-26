// Purpose: Secret layer initializer.
// Created on: 10/8/22 @ 5:54 PM

// Copyright (c) 2022, PortalPlayer
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "BaconEngine/Rendering/Layer.h"

SEC_CPP_SUPPORT_GUARD_START()
void BE_PrivateLayer_InitializeLayers(void);
void BE_PrivateLayer_OnUpdate(BE_Layer_UpdateTypes updateTypes);
int BE_PrivateLayer_OnEvent(BE_Event event);
void BE_PrivateLayer_DestroyLayers(void);
SEC_CPP_SUPPORT_GUARD_END()
