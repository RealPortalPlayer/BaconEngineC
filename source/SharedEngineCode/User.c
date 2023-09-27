// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "SharedEngineCode/Internal/OperatingSystem.h"
#include "SharedEngineCode/User.h"

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <unistd.h>
#   include <pwd.h>
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   include <stdio.h>
#   include <lmcons.h>

#   include "SharedEngineCode/Hacks/Windows.h"
#endif

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_Boolean SEC_User_IsAdministrator(void) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    return getuid() == 0;
#elif SEC_OPERATINGSYSTEM_WINDOWS
    SID_IDENTIFIER_AUTHORITY authority = SECURITY_NT_AUTHORITY;
    PSID adminGroup;
    SEC_Boolean admin = SEC_WindowsHacks_AllocateAndInitializeSid(&authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup);
    
    if (admin) {
        if (!SEC_WindowsHacks_CheckTokenMembership(NULL, adminGroup, &admin))
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
    static TCHAR buffer[UNLEN + 1] = {'\0'};
    DWORD size = UNLEN + 1;
    
    if (buffer[0] == '\0')
        GetUserName(buffer, &size);

    return buffer;
#endif
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
