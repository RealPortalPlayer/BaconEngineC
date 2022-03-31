// Purpose: Helps with argument handling.
// Created on: 3/28/22 @ 9:21 PM

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int argumentCount;
char** argumentVector;

/**
 * @return The index if the argument was found, -1 if not.
 */
int GetArgumentIndex(const char* argument);

/**
 * @return The value next to the argument if found, NULL if not.
 */
const char* GetArgumentValue(const char* argument);

#ifdef __cplusplus
};
#endif