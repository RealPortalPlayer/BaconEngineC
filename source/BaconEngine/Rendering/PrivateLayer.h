// Purpose: Secret layer initializer.
// Created on: 10/8/22 @ 5:54 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>

#include "../AntiClientGuard.h"
#include "BaconEngine/Rendering/Layer.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void BE_PrivateLayer_InitializeLayers(void);
void BE_PrivateLayer_OnUpdate(BE_Layer_UpdateTypes updateTypes);
BA_Boolean BE_PrivateLayer_OnEvent(BE_Event event);
void BE_PrivateLayer_DestroyLayers(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
