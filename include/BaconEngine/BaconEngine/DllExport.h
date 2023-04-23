// Purpose: Export function without being platform specific
// Created on: 4/22/2023 @ 2:03 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/OperatingSystem.h>

#ifndef BE_CLIENT_BINARY
#   if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#       define BE_DLLEXPORT __attribute__((visibility("default")))
#   elif SEC_OPERATINGSYSTEM_WINDOWS
#       define BE_DLLEXPORT __declspec(dllexport)
#   endif
#else
#   define BE_DLLEXPORT
#endif
