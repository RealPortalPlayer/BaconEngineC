// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <Interface/EntryPoint.h>

SEC_Boolean I_EntryPoint_SupportsServer() {
    return true;
}

const char* I_EntryPoint_GetName() {
    return "Test C++";
}
