// Purpose: Manage language files.
// Created on: 9/10/22 @ 3:51 PM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>
#include <stdio.h>

// TODO: Variables.

SEC_CPP_SUPPORT_GUARD_START()
const char* BE_I18N_Translate(FILE* languageFile, const char* key);
SEC_CPP_SUPPORT_GUARD_END()
