// Purpose: Makes logging easier.
// Created on: 3/29/22 @ 2:01 AM

#pragma once

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LOG_LEVEL_NULL,
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
} LogLevels;

LogLevels currentLogLevel;

void LogImplementation(LogLevels logLevel, const char* message, ...);

#ifdef __cplusplus
};
#endif

#define LOG_TRACE(...) LogImplementation(LOG_LEVEL_TRACE, __VA_ARGS__)
#define LOG_DEBUG(...) LogImplementation(LOG_LEVEL_DEBUG, __VA_ARGS__)
#define LOG_INFO(...) LogImplementation(LOG_LEVEL_INFO, __VA_ARGS__)
#define LOG_WARN(...) LogImplementation(LOG_LEVEL_WARN, __VA_ARGS__)
#define LOG_ERROR(...) LogImplementation(LOG_LEVEL_ERROR, __VA_ARGS__)
#define LOG_FATAL(...) LogImplementation(LOG_LEVEL_FATAL, __VA_ARGS__)