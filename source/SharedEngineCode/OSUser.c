#include "SharedEngineCode/Internal/OperatingSystem.h"

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <unistd.h>
#   include <pwd.h>
#else
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#   include <Lmcons.h>
#endif

#include "SharedEngineCode/Internal/CppSupport.h"

SEC_CPP_SUPPORT_GUARD_START()
int SEC_OSUser_IsAdmin(void) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    return getuid() == 0;
#elif SEC_OPERATINGSYSTEM_WINDOWS
    SID_IDENTIFIER_AUTHORITY authority = SECURITY_NT_AUTHORITY;
    PSID adminGroup;
    int admin = AllocateAndInitializeSid(&authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0,
                                         0, 0, 0, &adminGroup);

    if (admin) {
        if (!CheckTokenMembership(NULL, adminGroup, &admin))
            admin = 0;

        FreeSid(adminGroup);
    }

    return admin;
#endif
}

const char* SEC_OSUser_GetUsername(void) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    struct passwd* password = getpwuid(getuid());

    return password != NULL ? password->pw_name : "";
#elif SEC_OPERATINGSYSTEM_WINDOWS
    return ""; // TODO: Windows.
#endif
}
SEC_CPP_SUPPORT_GUARD_END()
