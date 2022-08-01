#include "SharedEngineCode/OperatingSystem.h"

#if OS_POSIX_COMPLIANT
#   include <dlfcn.h>
#   include <unistd.h>
#elif OS_WINDOWS
#   include <Windows.h>
#   include <direct.h>
#endif

#include "SharedEngineCode/Launcher.h"
#include "SharedEngineCode/Internal/CppHeader.h"

#if OS_POSIX_COMPLIANT
#   define SET_ERROR() configuration->errorMessage = dlerror()
#elif OS_WINDOWS
#   define SET_ERROR() \
wchar_t errorBuffer[256]; \
FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errorBuffer, (sizeof(errorBuffer) / sizeof(wchar_t)), NULL); \
configuration->errorMessage = (LPSTR) errorBuffer
#endif

CPP_GUARD_START()
    void CreateLauncherConfiguration(LauncherConfiguration* configuration, const char* path) {
#if OS_POSIX_COMPLIANT
        chdir(path);

        configuration->clientBinary = dlopen("binary.dylib", RTLD_NOW);
#elif OS_WINDOWS
        _chdir(path);

        configuration->clientBinary = LoadLibrary("binary.dll");
#endif

        if (configuration->clientBinary == NULL) {
            configuration->code = LAUNCHER_ERROR_CODE_BINARY;

            SET_ERROR();
            return;
        }

#if OS_POSIX_COMPLIANT
        const char* (*name)(void) = dlsym(configuration->clientBinary, "GetClientName");
#elif OS_WINDOWS
        const char* (*name)(void) = (const char* (*)(void)) GetProcAddress(configuration->clientBinary, "GetClientName");
#endif

        if (name == NULL) {
            configuration->code = LAUNCHER_ERROR_CODE_NAME_NULL;

            SET_ERROR();
            return;
        }

        configuration->clientName = name();

#if OS_POSIX_COMPLIANT
        configuration->Start = dlsym(configuration->clientBinary, "StartBaconEngine");
#elif OS_WINDOWS
        configuration->Start = (int (*)(struct LauncherConfiguration, int, char**)) GetProcAddress(configuration->clientBinary, "StartBaconEngine");
#endif

        if (configuration->Start == NULL) {
            configuration->code = LAUNCHER_ERROR_CODE_ENTRY_NULL;
            SET_ERROR();
            return;
        }

        configuration->code = LAUNCHER_ERROR_CODE_NULL;
    }
CPP_GUARD_END()