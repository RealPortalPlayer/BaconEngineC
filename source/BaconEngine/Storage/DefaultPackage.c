// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/Paths.h>

#include "BaconEngine/Storage/DefaultPackage.h"
#include "BaconEngine/Storage/Package.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "../InterfaceFunctions.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static BE_Package beDefaultPackage = NULL;

SEC_Boolean BE_PrivateDefaultPackage_Open(void) {
    BE_ASSERT(beDefaultPackage == NULL, "Default package is already opened\n");
    SEC_LOGGER_INFO("Opening default engine package\n");
    
    char* filePath = malloc(sizeof(char));
    
    filePath[0] = 0;

    SEC_Paths_GetItemInsideEngineDirectory(&filePath, "Default.package");
    
    beDefaultPackage = BE_Package_Open(filePath);

    BE_STRICTMODE_CHECK(beDefaultPackage != NULL, SEC_BOOLEAN_FALSE, "Failed to open default package\n");
    return SEC_BOOLEAN_TRUE;
}

SEC_Boolean BE_PrivateDefaultPackage_IsOpen(void) {
    return beDefaultPackage != NULL;
}

void BE_PrivateDefaultPackage_Close(void) {
    BE_ASSERT(beDefaultPackage != NULL, "Default package is already closed\n");
    SEC_LOGGER_INFO("Closing default engine package\n");
    
    BE_Package_Close(beDefaultPackage);
    
    beDefaultPackage = NULL;
}
#endif

BE_BINARYEXPORT SEC_Boolean BE_DefaultPackage_GetFile(const char* filePath, void** buffer, size_t* bufferSize) {
#ifndef BE_CLIENT_BINARY
    if (!BE_PrivateDefaultPackage_IsOpen())
        return SEC_BOOLEAN_FALSE;
    
    return BE_Package_GetFile(beDefaultPackage, filePath, buffer, bufferSize);
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, const char*, void**, size_t*);
    return function(filePath, buffer, bufferSize);
#endif
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
