// Purpose: Helps manage package files (zip files)
// Created on: 4/26/23 @ 12:10 PM

// Copyright (c) 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <zip.h>
#include <BaconAPI/Internal/Boolean.h>
#include <BaconAPI/String.h>

#include "BaconEngine/BinaryExport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct zip_t* BE_Package;

/**
 * @param fileName Name of the file you want to open, plus it's extension, relative to the binary path
 * @return NULL if the file couldn't be found, or couldn't be opened
 * @example
 * If the client binary is inside "/home/user/Downloads/BaconEngine/Client", then calling this with the argument of
 * "Languages.package" will open the file named that from the directory above
 * @note Relative paths **should** work, but isn't recommended
 */
BE_BINARYEXPORT BE_Package BE_Package_Open(const char* fileName);

/**
 * @param package If this is null, then it will use the engine package
 * @param filePath Path inside of the package file to the file you want
 * @param buffer A pointer to the output buffer
 * @param bufferSize How large that buffer is
 * @return If the file was successfully opened
 * @example
 * Calling this with the path of "en_us.json" will open that file inside of the package, calling this with
 * "Textures/wall.png" should do the same
 * @note When you're done with the buffer, remember to call "free" on it to prevent memory leaks
 * @warning The buffer will not be NULL terminated, and as such, will be dangerous to call functions that expect a NULL terminated string
 */
BE_BINARYEXPORT BA_Boolean BE_Package_GetFile(BE_Package package, const char* filePath, void** buffer, size_t* bufferSize);
BE_BINARYEXPORT void BE_Package_Close(BE_Package package);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
