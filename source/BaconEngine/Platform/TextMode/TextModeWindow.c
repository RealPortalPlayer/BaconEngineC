// Copyright (c) 2022, PortalPlayer
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "SharedEngineCode/Internal/CppSupport.h"
#include <stddef.h>

#include "TextModeWindow.h"
#include "BaconEngine/ClientInformation.h"

SEC_CPP_SUPPORT_GUARD_START()
void* BE_TextModeWindow_NoOperation(void) {
    return NULL;
}
SEC_CPP_SUPPORT_GUARD_END()
