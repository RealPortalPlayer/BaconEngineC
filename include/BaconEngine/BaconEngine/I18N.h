// Purpose: Manage language files.
// Created on: 9/10/22 @ 3:51 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>
#include <stdio.h>

#include "BinaryExport.h"

// TODO: Variables.

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
BE_BINARYEXPORT const char* BE_I18N_Translate(FILE* languageFile, const char* key);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
