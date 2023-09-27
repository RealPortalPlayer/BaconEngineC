// Purpose: Windows specific hacks
// Created on: 9/25/23 @ 4:19 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "SharedEngineCode/Internal/OperatingSystem.h"

#if !SEC_OPERATINGSYSTEM_WINDOWS
#   error You cannot include Windows hacks on non-Windows Operating Systems.
#endif

#include <Windows.h>

#include "SharedEngineCode/Internal/CPlusPlusSupport.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_DISABLE_HACKS
WINBOOL WINAPI SEC_WindowsHacks_CheckTokenMembership(HANDLE tokenHandle, PSID securityIdentifier, PBOOL isMember);
BOOL WINAPI SEC_WindowsHacks_AllocateAndInitializeSid(PSID_IDENTIFIER_AUTHORITY identifierAuthority, BYTE subAuthorityCount, DWORD subAuthority0, DWORD subAuthority1, DWORD subAuthority2, DWORD subAuthority3, DWORD subAuthority4, DWORD subAuthority5, DWORD subAuthority6, DWORD subAuthority7, PSID* securityIdentifier);
#else
#   define SEC_WindowsHacks_CheckTokenMembership CheckTokenMembership
#   define SEC_WindowsHacks_AllocateAndInitializeSid AllocateAndInitializeSid
#endif
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
