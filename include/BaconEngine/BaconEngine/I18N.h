// Purpose: Manage language files.
// Created on: 9/10/22 @ 3:51 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <stdio.h>
#include <BaconAPI/String.h>

#include "BinaryExport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_BINARYEXPORT const char* BE_I18N_TranslateFromFile(FILE* languageFile, const char* key);
BE_BINARYEXPORT const char* BE_I18N_Translate(const char* buffer, const char* key);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
