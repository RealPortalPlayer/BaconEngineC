// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconEngine/DeltaTime.h"
#include "InterfaceFunctions.h"

#ifndef BE_CLIENT_BINARY
#   include "PrivateDeltaTime.h"
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static double beDeltaTimeCurrentTime;

void BE_PrivateDeltaTime_Setter(double deltaTime) {
    beDeltaTimeCurrentTime = deltaTime;
}
#endif

double BE_DeltaTime_GetMilliseconds(void) {
#ifndef BE_CLIENT_BINARY
    return beDeltaTimeCurrentTime;
#else
     BE_INTERFACEFUNCTION(double, void);
    return function();
#endif
}

double BE_DeltaTime_GetSeconds(void) {
#ifndef BE_CLIENT_BINARY
    return beDeltaTimeCurrentTime * 1000;
#else
    BE_INTERFACEFUNCTION(double, void);
    return function();
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
