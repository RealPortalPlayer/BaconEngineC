#include "SharedEngineCode/Internal/OperatingSystem.h"

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <unistd.h>
#   include <pwd.h>
#endif

#include "SharedEngineCode/Internal/CppHeader.h"

SEC_CPP_GUARD_START()
    // TODO: Windows
    int SEC_OSUser_IsAdmin(void) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
        return getuid() == 0;
#elif SEC_OPERATINGSYSTEM_WINDOWS
        return 0;
#endif
    }

    const char* SEC_OSUser_GetUsername(void) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
        struct passwd* password = getpwuid(getuid());

        return password != NULL ? password->pw_name : "";
#elif SEC_OPERATINGSYSTEM_WINDOWS
        return "";
#endif
    }
SEC_CPP_GUARD_END()
