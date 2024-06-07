// Purpose: The basics for a thread
// Created on: 6/7/24 @ 3:38 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Thread.h>
#include <BaconAPI/Debugging/Assert.h>

#include "../AntiClientGuard.h"

#define BE_BASETHREAD_THREAD_FUNCTION_HEADER(prefix) BA_THREAD_RETURN_VALUE BA_THREAD_ATTRIBUTE prefix ## _ThreadFunction(void* argument)
#define BE_BASETHREAD_START_HEADER(prefix) void prefix ## _Start(void)
#define BE_BASETHREAD_STOP_HEADER(prefix) void prefix ## _Stop(void)
#define BE_BASETHREAD_KILL_HEADER(prefix) void prefix ## _Kill(void)

#define BE_BASETHREAD_CREATE_HEADER(prefix) \
BE_BASETHREAD_START_HEADER(prefix);         \
BE_BASETHREAD_STOP_HEADER(prefix);          \
BE_BASETHREAD_KILL_HEADER(prefix)

#define BE_BASETHREAD_CREATE_IMPLEMENTATION(prefix, variablePrefix, name) \
static BA_Thread variablePrefix ## Thread;                                \
static BA_Boolean variablePrefix ## Initialized;                          \
BE_BASETHREAD_THREAD_FUNCTION_HEADER(prefix);                             \
BE_BASETHREAD_START_HEADER(prefix) {                                      \
    BA_ASSERT(!variablePrefix ## Initialized, "Already initialized thread %s\n", name); \
    BA_LOGGER_DEBUG("Starting thread %s\n", name);                        \
    variablePrefix ## Initialized = BA_BOOLEAN_TRUE;                      \
    BA_ASSERT(BA_Thread_Create(&variablePrefix ## Thread, prefix ## _ThreadFunction, NULL), "Failed to create thread %s\n", name); \
}                                                                         \
BE_BASETHREAD_STOP_HEADER(prefix) {                                       \
    BA_ASSERT(variablePrefix ## Initialized, "Thread %s is not running\n", name); \
    variablePrefix ## Initialized = BA_BOOLEAN_FALSE;                     \
    BA_LOGGER_DEBUG("Stopping thread %s\n", name);                        \
    BA_ASSERT(BA_Thread_Join(variablePrefix ## Thread, NULL), "Failed to stop thread %s\n", name); \
}                                                                         \
BE_BASETHREAD_KILL_HEADER(prefix) {                                       \
    BA_ASSERT(variablePrefix ## Initialized, "Thread %s is not running\n", name); \
    variablePrefix ## Initialized = BA_BOOLEAN_FALSE;                     \
    BA_LOGGER_DEBUG("Killing thread %s\n", name);                         \
    BA_ASSERT(BA_Thread_Kill(variablePrefix ## Thread), "Failed to stop thread %s\n", name); \
}                                                                         \
BE_BASETHREAD_THREAD_FUNCTION_HEADER(prefix)
