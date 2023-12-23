// Purpose: Export function without being platform specific
// Created on: 4/22/2023 @ 2:03 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/OperatingSystem.h>

#ifndef BE_CLIENT_BINARY
#   if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#       define BE_BINARYEXPORT __attribute__((visibility("default")))
#   elif BA_OPERATINGSYSTEM_WINDOWS
#       define BE_BINARYEXPORT __declspec(dllexport)
#   endif
#else
#   define BE_BINARYEXPORT
#endif
