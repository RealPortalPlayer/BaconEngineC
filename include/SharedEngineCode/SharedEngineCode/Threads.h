// Purpose: Wrapper around OS specific threads
// Created on: 11/25/22 @ 1:54 PM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "Internal/CppSupport.h"
#include "Internal/Boolean.h"
#include "Internal/OperatingSystem.h"

SEC_CPP_SUPPORT_GUARD_START()
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <pthread.h>

typedef pthread_t SEC_Thread;
typedef pthread_mutex_t SEC_Thread_Lock;
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>

typedef HANDLE SEC_Thread;
typedef HANDLE SEC_Thread_Lock;
#endif

SEC_Thread SEC_Thread_GetCurrent(void);

SEC_Boolean SEC_Thread_Create(SEC_Thread* thread, void (*threadFunction)(void));
SEC_Boolean SEC_Thread_Join(SEC_Thread thread);
SEC_Boolean SEC_Thread_CreateLock(SEC_Thread_Lock* lock);
SEC_Boolean SEC_Thread_UseLock(SEC_Thread_Lock* lock);
SEC_Boolean SEC_Thread_Unlock(SEC_Thread_Lock* lock);
SEC_Boolean SEC_Thread_DestroyLock(SEC_Thread_Lock* lock);
SEC_CPP_SUPPORT_GUARD_END()
