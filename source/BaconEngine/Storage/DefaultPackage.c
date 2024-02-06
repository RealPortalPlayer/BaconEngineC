// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/Paths.h>
#include <BaconAPI/Debugging/Assert.h>
#include <SharedEngineCode/Debugging/StrictMode.h>
#include <BaconAPI/String.h>

#include "BaconEngine/Storage/Package.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static BE_Package beDefaultPackage = NULL;

BA_Boolean BE_PrivateDefaultPackage_Open(void) {
    BA_ASSERT(beDefaultPackage == NULL, "Default package is already opened\n");
    BA_LOGGER_INFO("Opening default engine package\n");
    
    char* filePath = BA_String_CreateEmpty();
    
    if (filePath == NULL)
        return BA_BOOLEAN_FALSE;
    
    SEC_Paths_GetItemInsideEngineDirectory(&filePath, "Default.package");
    
    beDefaultPackage = BE_Package_Open(filePath);

    SEC_STRICTMODE_CHECK(beDefaultPackage != NULL, BA_BOOLEAN_FALSE, "Failed to open default package\n");
    return BA_BOOLEAN_TRUE;
}

BA_Boolean BE_PrivateDefaultPackage_IsOpen(void) {
    return beDefaultPackage != NULL;
}

void BE_PrivateDefaultPackage_Close(void) {
    BA_ASSERT(beDefaultPackage != NULL, "Default package is already closed\n");
    BA_LOGGER_INFO("Closing default engine package\n");
    
    BE_Package_Close(beDefaultPackage);
    
    beDefaultPackage = NULL;
}

BE_Package BE_PrivateDefaultPackage_Get(void) {
    return beDefaultPackage;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
