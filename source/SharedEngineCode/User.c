// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "SharedEngineCode/Internal/OperatingSystem.h"
#include "SharedEngineCode/User.h"

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <unistd.h>
#   include <pwd.h>
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#   include <stdio.h>
#endif

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_Boolean SEC_User_IsAdministrator(void) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    return getuid() == 0;
#elif SEC_OPERATINGSYSTEM_WINDOWS
    SID_IDENTIFIER_AUTHORITY authority = SECURITY_NT_AUTHORITY;
    PSID adminGroup;
    SEC_Boolean admin = AllocateAndInitializeSid(&authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0,
                                                 0, 0, 0, &adminGroup);

    if (admin) {
        if (!CheckTokenMembership(NULL, adminGroup, &admin))
            admin = SEC_BOOLEAN_FALSE;

        FreeSid(adminGroup);
    }

    return admin;
#endif
}

const char* SEC_User_GetName(void) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    struct passwd* password = getpwuid(getuid());

    return password != NULL ? password->pw_name : "";
#elif SEC_OPERATINGSYSTEM_WINDOWS
    return ""; // TODO: Windows.
#endif
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
