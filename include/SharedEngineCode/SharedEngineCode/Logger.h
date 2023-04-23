// Purpose: Makes logging easier.
// Created on: 3/29/22 @ 2:01 AM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <SharedEngineCode/Internal/OperatingSystem.h>

#include "Internal/CppSupport.h"
#include "Internal/Boolean.h"

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   define BE_LOGGER_PRINTF_ATTRIBUTE __attribute__((__format__(__printf__, 3, 4)))
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   define BE_LOGGER_PRINTF_ATTRIBUTE
#endif


SEC_CPP_SUPPORT_GUARD_START()
typedef enum {
    SEC_LOGGER_LOG_LEVEL_FATAL,
    SEC_LOGGER_LOG_LEVEL_ERROR,
    SEC_LOGGER_LOG_LEVEL_WARN,
    SEC_LOGGER_LOG_LEVEL_INFO,
    SEC_LOGGER_LOG_LEVEL_DEBUG,
    SEC_LOGGER_LOG_LEVEL_TRACE,
    SEC_LOGGER_LOG_LEVEL_NULL
} SEC_Logger_LogLevels;

SEC_Logger_LogLevels SEC_Logger_GetLogLevel(void);
void BE_LOGGER_PRINTF_ATTRIBUTE SEC_Logger_LogImplementation(SEC_Boolean includeHeader, SEC_Logger_LogLevels logLevel, const char* message, ...);
void SEC_Logger_LogHeader(FILE* output, SEC_Logger_LogLevels logLevel);
SEC_Boolean SEC_Logger_IsLevelEnabled(SEC_Logger_LogLevels logLevel);

void SEC_Logger_SetLogLevel(SEC_Logger_LogLevels logLevel);
SEC_CPP_SUPPORT_GUARD_END()

#define SEC_LOGGER_TRACE(...) SEC_Logger_LogImplementation(SEC_TRUE, SEC_LOGGER_LOG_LEVEL_TRACE, __VA_ARGS__)
#define SEC_LOGGER_DEBUG(...) SEC_Logger_LogImplementation(SEC_TRUE, SEC_LOGGER_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define SEC_LOGGER_INFO(...) SEC_Logger_LogImplementation(SEC_TRUE, SEC_LOGGER_LOG_LEVEL_INFO, __VA_ARGS__)
#define SEC_LOGGER_WARN(...) SEC_Logger_LogImplementation(SEC_TRUE, SEC_LOGGER_LOG_LEVEL_WARN, __VA_ARGS__)
#define SEC_LOGGER_ERROR(...) SEC_Logger_LogImplementation(SEC_TRUE, SEC_LOGGER_LOG_LEVEL_ERROR, __VA_ARGS__)
#define SEC_LOGGER_FATAL(...) SEC_Logger_LogImplementation(SEC_TRUE, SEC_LOGGER_LOG_LEVEL_FATAL, __VA_ARGS__)
