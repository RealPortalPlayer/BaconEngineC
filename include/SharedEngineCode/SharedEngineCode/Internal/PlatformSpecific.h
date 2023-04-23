// Purpose: Commonly used platform specific macros
// Created on: 4/22/2023 @ 7:10 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "OperatingSystem.h"

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <dlfcn.h>
#   include <unistd.h>
#   define SEC_PLATFORMSPECIFIC_CHDIR(dir) chdir(dir)
#   define SEC_PLATFORMSPECIFIC_GET_BINARY(name, options) dlopen(name, options)
#   define SEC_PLATFORMSPECIFIC_GET_ADDRESS(binary, name) dlsym(binary, name)
#   define SEC_PLATFORMSPECIFIC_GET_ERROR(variable) variable = dlerror()
#   if SEC_OPERATINGSYSTEM_APPLE
#       define SEC_PLATFORMSPECIFIC_BINARY_EXTENSION ".dylib"
#   elif SEC_OPERATINGSYSTEM_LINUX || SEC_OPERATINGSYSTEM_UNIX || SEC_OPERATINGSYSTEM_SERENITY
#       define SEC_PLATFORMSPECIFIC_BINARY_EXTENSION ".so"
#   endif
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#   include <direct.h>
#   define SEC_PLATFORMSPECIFIC_CHDIR(dir) _chdir(dir)
#   define SEC_PLATFORMSPECIFIC_GET_BINARY(name, options) LoadLibrary(name)
#   define SEC_PLATFORMSPECIFIC_GET_ADDRESS(binary, name) GetProcAddress(binary, name)
#   define SEC_PLATFORMSPECIFIC_GET_ERROR(variable) do { \
        DWORD id = GetLastError();                       \
        LPSTR message = NULL;                            \
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &message, 0, NULL); \
        variable = message;                              \
    } while (0)
#   define SEC_PLATFORMSPECIFIC_BINARY_EXTENSION ".dll"
#endif
