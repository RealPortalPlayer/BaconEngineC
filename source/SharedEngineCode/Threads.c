// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <errno.h>

#include "SharedEngineCode/Internal/OperatingSystem.h"

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <pthread.h>
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#endif

#include "SharedEngineCode/Threads.h"

SEC_CPP_SUPPORT_GUARD_START()
SEC_Thread SEC_Thread_GetCurrent(void) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    return pthread_self();
#elif SEC_OPERATINGSYSTEM_WINDOWS
    return GetCurrentThread();
#endif
}

SEC_Boolean SEC_Thread_Create(SEC_Thread* thread, void (*threadFunction)(void)) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    int result = pthread_create(thread, NULL, (void* (*)(void*)) threadFunction, NULL);

    if (result != 0)
        errno = result;

    return result == 0;
#elif SEC_OPERATINGSYSTEM_WINDOWS
    *thread = (SEC_Thread) CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) threadFunction, NULL, 0, NULL);
    return *thread != NULL;
#endif
}

SEC_Boolean SEC_Thread_Join(SEC_Thread thread) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    int result = pthread_join(thread, NULL);

    if (result != 0)
        errno = result;

    return result == 0;
#elif SEC_OPERATINGSYSTEM_WINDOWS
    while (WaitForSingleObject(thread, INFINITE) != WAIT_OBJECT_0)
        continue;

    return SEC_TRUE;
#endif
}

SEC_Boolean SEC_Thread_CreateLock(SEC_Thread_Lock* lock) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    int result = pthread_mutex_init(lock, NULL);

    if (result != 0)
        errno = result;

    return result == 0;
#elif SEC_OPERATINGSYSTEM_WINDOWS
    *lock = CreateMutex(NULL, FALSE, NULL);
    return *lock != NULL;
#endif
}

SEC_Boolean SEC_Thread_UseLock(SEC_Thread_Lock* lock) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    int result = pthread_mutex_lock(lock);

    if (result != 0)
        errno = result;

    return result == 0;
#elif SEC_OPERATINGSYSTEM_WINDOWS
    while (WaitForSingleObject(*lock, INFINITE) != WAIT_OBJECT_0)
        continue;

    return SEC_TRUE;
#endif
}

SEC_Boolean SEC_Thread_Unlock(SEC_Thread_Lock* lock) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    int result = pthread_mutex_unlock((pthread_mutex_t*) lock);

    if (result != 0)
        errno = result;

    return result == 0;
#elif SEC_OPERATINGSYSTEM_WINDOWS
    return ReleaseMutex(*lock);
#endif
}

SEC_Boolean SEC_Thread_DestroyLock(SEC_Thread_Lock* lock) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    int result = pthread_mutex_destroy((pthread_mutex_t*) lock);

    if (result != 0)
        errno = result;

    return result == 0;
#elif SEC_OPERATINGSYSTEM_WINDOWS
    return CloseHandle(*lock);
#endif
}
SEC_CPP_SUPPORT_GUARD_END()
