// Purpose: OS specific definitions.
// Created on: 4/1/22 @ 7:25 PM

#pragma once

#define SEC_OS_WINDOWS 0
#define SEC_OS_APPLE 0
#define SEC_OS_POSIX_COMPLIANT 0
#define SEC_OS_IPHONE_SIMULATOR 0
#define SEC_OS_MAC_CATALYST 0
#define SEC_OS_IPHONE 0
#define SEC_OS_MAC 0
#define SEC_OS_APPLE_UNKNOWN 0
#define SEC_OS_UNKNOWN 0
#define SEC_OS_LINUX 0
#define SEC_OS_UNIX 0

#if defined(WIN32)
#   undef SEC_OS_WINDOWS
#   define SEC_OS_WINDOWS 1
#elif defined(__APPLE__)
#   include <TargetConditionals.h>
#   undef SEC_OS_APPLE
#   undef SEC_OS_POSIX_COMPLIANT
#   undef SEC_OS_IPHONE_SIMULATOR
#   undef SEC_OS_MAC_CATALYST
#   undef SEC_OS_IPHONE
#   undef SEC_OS_MAC
#   undef SEC_OS_APPLE_UNKNOWN
#   undef SEC_OS_UNKNOWN
#   define SEC_OS_APPLE 1
#   define SEC_OS_POSIX_COMPLIANT 1
#   if TARGET_IPHONE_SIMULATOR
#       define SEC_OS_IPHONE_SIMULATOR 1
#   elif TARGET_OS_MACCATALYST
#       define SEC_OS_MAC_CATALYST 1
#   elif TARGET_OS_IPHONE
#       define SEC_OS_IPHONE 1
#   elif TARGET_OS_MAC
#       define SEC_OS_MAC 1
#   else
#       define SEC_OS_APPLE_UNKNOWN 1
#       define SEC_OS_UNKNOWN 1
#       warning "Your Apple device is currently unknown. There might be some issues."
#   endif
#elif defined(__linux__)
#   undef SEC_OS_LINUX
#   undef SEC_OS_POSIX_COMPLIANT
#   define SEC_OS_LINUX 1
#   define SEC_OS_POSIX_COMPLIANT 1
#elif defined(__unix__)
#   undef SEC_OS_UNIX
#   undef SEC_OS_POSIX_COMPLIANT
#   define SEC_OS_UNIX 1
#   define SEC_OS_POSIX_COMPLIANT 1
#else
#   undef SEC_OS_UNKNOWN
#   define SEC_OS_UNKNOWN 1
#   warning "Your Operating System is unknown. This will cause issues."
#endif
