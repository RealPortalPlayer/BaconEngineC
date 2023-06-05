// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconEngine/Package.h"
#include "InterfaceFunctions.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
BE_Package BE_Package_Open(const char* fileName) {
#ifndef BE_CLIENT_BINARY
    return zip_open(fileName, ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');
#else
    BE_INTERFACEFUNCTION(BE_Package, const char*);
    return function(fileName);
#endif
}

SEC_Boolean BE_Package_GetFile(BE_Package package, const char* filePath, void** buffer, size_t* bufferSize) {
#ifndef BE_CLIENT_BINARY
    return zip_entry_open(package, filePath) == 0 &&
           zip_entry_read(package, buffer, bufferSize) >= 0 &&
           zip_entry_close(package) == 0;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_Package, const char*, void*, size_t*);
    return function(package, filePath, buffer, bufferSize);
#endif
}

void BE_Package_Close(BE_Package package) {
#ifndef BE_CLIENT_BINARY
    zip_close(package);
#else
    BE_INTERFACEFUNCTION(void, BE_Package)(package);
#endif
}

SEC_CPLUSPLUS_SUPPORT_GUARD_END()