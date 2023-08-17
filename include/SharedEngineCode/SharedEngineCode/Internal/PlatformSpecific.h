// Purpose: Commonly used platform specific macros
// Created on: 4/22/2023 @ 7:10 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "OperatingSystem.h"
#include "Boolean.h"

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <dlfcn.h>
#   include <unistd.h>
#   define SEC_PLATFORMSPECIFIC_CHANGE_DIRECTORY(directory) chdir(directory)
#   define SEC_PLATFORMSPECIFIC_GET_BINARY(name, options) dlopen(name, options)
#   define SEC_PLATFORMSPECIFIC_GET_ADDRESS(binary, name) dlsym(binary, name)
#   define SEC_PLATFORMSPECIFIC_GET_ERROR(variable) variable = dlerror()
#   if SEC_OPERATINGSYSTEM_APPLE
#       define SEC_PLATFORMSPECIFIC_BINARY_EXTENSION ".dylib"
#   elif SEC_OPERATINGSYSTEM_LINUX || SEC_OPERATINGSYSTEM_UNIX || SEC_OPERATINGSYSTEM_SERENITY
#       define SEC_PLATFORMSPECIFIC_BINARY_EXTENSION ".so"
#   endif
#   define SEC_PLATFORMSPECIFIC_CLOSE_BINARY(binary) dlclose(binary)
#   define SEC_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(type, variable, value) *(void**) &variable = value
 #elif SEC_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#   include <direct.h>
#   define SEC_PLATFORMSPECIFIC_CHANGE_DIRECTORY(directory) _chdir(directory)
#   define SEC_PLATFORMSPECIFIC_GET_BINARY(name, options) LoadLibrary(name)
#   define SEC_PLATFORMSPECIFIC_GET_ADDRESS(binary, name) GetProcAddress(binary, name)
#   define SEC_PLATFORMSPECIFIC_GET_ERROR(variable) do { \
        DWORD id = GetLastError();                       \
        LPSTR message = NULL;                            \
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &message, 0, NULL); \
        variable = message;                              \
    } while (SEC_BOOLEAN_FALSE)
#   define SEC_PLATFORMSPECIFIC_BINARY_EXTENSION ".dll"
#   define SEC_PLATFORMSPECIFIC_CLOSE_BINARY(binary) FreeLibrary(binary)
#   define SEC_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(type, variable, value) variable = (type) value
#endif
