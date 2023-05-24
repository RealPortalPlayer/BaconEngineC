// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconEngine/Input/Mouse.h"
#include "../InterfaceFunctions.h"

#ifndef BE_CLIENT_BINARY
#   include "../Platform/SpecificPlatformFunctions.h"
#endif

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_Boolean BE_Mouse_IsButtonDown(BE_Mouse_ButtonType button) {
#ifndef BE_CLIENT_BINARY
    return button <= 5 && BE_SpecificPlatformFunctions_Get().inputFunctions.IsMouseButtonDown(button);
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_Mouse_ButtonType);
    return function(button);
#endif
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
