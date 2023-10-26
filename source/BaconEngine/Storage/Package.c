// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/Logger.h>

#include "BaconEngine/Storage/Package.h"
#include "../InterfaceFunctions.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_Package BE_Package_Open(const char* fileName) {
#ifndef BE_CLIENT_BINARY
    BA_LOGGER_TRACE("Opening package: %s\n", fileName);
    return zip_open(fileName, ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');
#else
    BE_INTERFACEFUNCTION(BE_Package, const char*);
    return function(fileName);
#endif
}

BA_Boolean BE_Package_GetFile(BE_Package package, const char* filePath, void** buffer, size_t* bufferSize) {
#ifndef BE_CLIENT_BINARY
    BA_LOGGER_TRACE("Getting file in a package: %s\n", filePath);
    return zip_entry_open(package, filePath) == 0 &&
           zip_entry_read(package, buffer, bufferSize) >= 0 &&
           zip_entry_close(package) == 0;
#else
    BE_INTERFACEFUNCTION(BA_Boolean, BE_Package, const char*, void*, size_t*);
    return function(package, filePath, buffer, bufferSize);
#endif
}

void BE_Package_Close(BE_Package package) {
#ifndef BE_CLIENT_BINARY
    BA_LOGGER_TRACE("Closing a package\n");
    zip_close(package);
#else
    BE_INTERFACEFUNCTION(void, BE_Package)(package);
#endif
}

BA_CPLUSPLUS_SUPPORT_GUARD_END()
