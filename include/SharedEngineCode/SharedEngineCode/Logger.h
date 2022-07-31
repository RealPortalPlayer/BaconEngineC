// Purpose: Makes logging easier.
// Created on: 3/29/22 @ 2:01 AM

#pragma once

#include <stdarg.h>

#include "Internal/CppHeader.h"

CPP_GUARD_START()
    typedef enum {
        LOG_LEVEL_NULL,
        LOG_LEVEL_TRACE,
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_INFO,
        LOG_LEVEL_WARN,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_FATAL
    } LogLevels;

    volatile LogLevels currentLogLevel;

    void LogImplementation(int includeHeader, LogLevels logLevel, const char* message, ...);
CPP_GUARD_END()

#define LOG_TRACE(...) LogImplementation(1, LOG_LEVEL_TRACE, __VA_ARGS__)
#define LOG_DEBUG(...) LogImplementation(1, LOG_LEVEL_DEBUG, __VA_ARGS__)
#define LOG_INFO(...) LogImplementation(1, LOG_LEVEL_INFO, __VA_ARGS__)
#define LOG_WARN(...) LogImplementation(1, LOG_LEVEL_WARN, __VA_ARGS__)
#define LOG_ERROR(...) LogImplementation(1, LOG_LEVEL_ERROR, __VA_ARGS__)
#define LOG_FATAL(...) LogImplementation(1, LOG_LEVEL_FATAL, __VA_ARGS__)