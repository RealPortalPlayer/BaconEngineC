// Purpose: Makes logging easier.
// Created on: 3/29/22 @ 2:01 AM

#pragma once

#include <stdarg.h>

#include "Internal/CppHeader.h"

CPP_GUARD_START()
    typedef enum {
        SEC_LOG_LEVEL_NULL,
        SEC_LOG_LEVEL_TRACE,
        SEC_LOG_LEVEL_DEBUG,
        SEC_LOG_LEVEL_INFO,
        SEC_LOG_LEVEL_WARN,
        SEC_LOG_LEVEL_ERROR,
        SEC_LOG_LEVEL_FATAL
    } SEC_LogLevels;

    SEC_LogLevels SEC_GetLogLevel(void);
    void SEC_SetLogLevel(SEC_LogLevels logLevel);
    void SEC_LogImplementation(int includeHeader, SEC_LogLevels logLevel, const char* message, ...);
CPP_GUARD_END()

#define SEC_LOG_TRACE(...) SEC_LogImplementation(1, SEC_LOG_LEVEL_TRACE, __VA_ARGS__)
#define SEC_LOG_DEBUG(...) SEC_LogImplementation(1, SEC_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define SEC_LOG_INFO(...) SEC_LogImplementation(1, SEC_LOG_LEVEL_INFO, __VA_ARGS__)
#define SEC_LOG_WARN(...) SEC_LogImplementation(1, SEC_LOG_LEVEL_WARN, __VA_ARGS__)
#define SEC_LOG_ERROR(...) SEC_LogImplementation(1, SEC_LOG_LEVEL_ERROR, __VA_ARGS__)
#define SEC_LOG_FATAL(...) SEC_LogImplementation(1, SEC_LOG_LEVEL_FATAL, __VA_ARGS__)
