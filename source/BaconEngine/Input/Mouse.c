// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconEngine/Input/Mouse.h"
#include "../Platform/SpecificPlatformFunctions.h"

SEC_CPP_SUPPORT_GUARD_START()
SEC_Boolean BE_Mouse_IsButtonDown(BE_Mouse_ButtonType button) {
    return button <= 5 && BE_SpecificPlatformFunctions_Get().inputFunctions.IsMouseButtonDown(button);
}
SEC_CPP_SUPPORT_GUARD_END()
