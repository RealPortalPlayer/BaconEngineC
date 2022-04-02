// Purpose: Allows making launchers easier.
// Created on: 3/28/22 @ 9:54 PM

#pragma once

#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ERROR_CODE_NULL, // No error.
    ERROR_CODE_BINARY, // Errored while opening binary file.
    ERROR_CODE_NAME_NULL, // Errored attempting to load the name method.
    ERROR_CODE_ENTRY_NULL // Errored attempting to load the entry point.
} ErrorCodes;

typedef struct Configuration {
    ErrorCodes code;
    union {
        const char* errorMessage; // The message from something like dlopen.
        struct {
            const char* clientName;
            void* clientBinary;
            int (*Start)(struct Configuration*, int, char**);
        };
    };
} Configuration;

/**
 * @return The initialized configuration file. NULL if malloc fails.
 */
Configuration* GetConfiguration(const char* path);

#ifdef __cplusplus
};
#endif