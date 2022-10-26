// Copyright (c) 2022, PortalPlayer
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconEngine/DeltaTime.h"
#include "PrivateDeltaTime.h"

SEC_CPP_SUPPORT_GUARD_START()
double beDeltaTimeCurrentTime;

void BE_PrivateDeltaTime_Setter(double deltaTime) {
    beDeltaTimeCurrentTime = deltaTime;
}

double BE_DeltaTime_GetMilliseconds(void) {
    return beDeltaTimeCurrentTime;
}

double BE_DeltaTime_GetSeconds(void) {
    return beDeltaTimeCurrentTime * 1000;
}
SEC_CPP_SUPPORT_GUARD_END()
