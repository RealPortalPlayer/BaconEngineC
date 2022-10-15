// Purpose: Makes logging easier.
// Created on: 3/29/22 @ 2:01 AM

#pragma once

#include <stdarg.h>

#include "Internal/CppSupport.h"

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
void SEC_Logger_SetLogLevel(SEC_Logger_LogLevels logLevel);
void SEC_Logger_LogImplementation(int includeHeader, int includeNewLine, SEC_Logger_LogLevels logLevel, const char* message, ...);
SEC_CPP_SUPPORT_GUARD_END()

#define SEC_LOGGER_TRACE(...) SEC_Logger_LogImplementation(1, 1, SEC_LOGGER_LOG_LEVEL_TRACE, __VA_ARGS__)
#define SEC_LOGGER_DEBUG(...) SEC_Logger_LogImplementation(1, 1, SEC_LOGGER_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define SEC_LOGGER_INFO(...) SEC_Logger_LogImplementation(1, 1, SEC_LOGGER_LOG_LEVEL_INFO, __VA_ARGS__)
#define SEC_LOGGER_WARN(...) SEC_Logger_LogImplementation(1, 1, SEC_LOGGER_LOG_LEVEL_WARN, __VA_ARGS__)
#define SEC_LOGGER_ERROR(...) SEC_Logger_LogImplementation(1, 1, SEC_LOGGER_LOG_LEVEL_ERROR, __VA_ARGS__)
#define SEC_LOGGER_FATAL(...) SEC_Logger_LogImplementation(1, 1, SEC_LOGGER_LOG_LEVEL_FATAL, __VA_ARGS__)
