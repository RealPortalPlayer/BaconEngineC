// Purpose: OS specific definitions.
// Created on: 4/1/22 @ 7:25 PM

#pragma once

#define SEC_OPERATINGSYSTEM_NAME "Unknown"
#define SEC_OPERATINGSYSTEM_WINDOWS 0
#define SEC_OPERATINGSYSTEM_APPLE 0
#define SEC_OPERATINGSYSTEM_POSIX_COMPLIANT 0
#define SEC_OPERATINGSYSTEM_IPHONE_SIMULATOR 0
#define SEC_OPERATINGSYSTEM_MAC_CATALYST 0
#define SEC_OPERATINGSYSTEM_IPHONE 0
#define SEC_OPERATINGSYSTEM_MAC 0
#define SEC_OPERATINGSYSTEM_APPLE_UNKNOWN 0
#define SEC_OPERATINGSYSTEM_UNKNOWN 0
#define SEC_OPERATINGSYSTEM_LINUX 0
#define SEC_OPERATINGSYSTEM_UNIX 0
#define SEC_OPERATINGSYSTEM_SERENITY 0

#if defined(WIN32)
#   undef SEC_OPERATINGSYSTEM_WINDOWS
#   undef SEC_OPERATINGSYSTEM_NAME
#   define SEC_OPERATINGSYSTEM_WINDOWS 1
#   define SEC_OPERATINGSYSTEM_NAME "Windows"
#elif defined(__APPLE__)
#   include <TargetConditionals.h>
#   undef SEC_OPERATINGSYSTEM_APPLE
#   undef SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   define SEC_OPERATINGSYSTEM_APPLE 1
#   define SEC_OPERATINGSYSTEM_POSIX_COMPLIANT 1
#   if TARGET_IPHONE_SIMULATOR
#       undef SEC_OPERATINGSYSTEM_IPHONE_SIMULATOR
#       undef SEC_OPERATINGSYSTEM_NAME
#       define SEC_OPERATINGSYSTEM_IPHONE_SIMULATOR 1
#       define SEC_OPERATINGSYSTEM_NAME "iOS Simulator"
#   elif TARGET_OS_MACCATALYST
#       undef SEC_OPERATINGSYSTEM_MAC_CATALYST
#       undef SEC_OPERATINGSYSTEM_NAME
#       define SEC_OPERATINGSYSTEM_MAC_CATALYST 1
#       define SEC_OPERATINGSYSTEM_NAME "Mac Catalyst"
#   elif TARGET_OS_IPHONE
#       undef SEC_OPERATINGSYSTEM_IPHONE
#       undef SEC_OPERATINGSYSTEM_NAME
#       define SEC_OPERATINGSYSTEM_IPHONE 1
#       define SEC_OPERATINGSYSTEM_NAME "iOS"
#   elif TARGET_OS_MAC
#       undef SEC_OPERATINGSYSTEM_MAC
#       undef SEC_OPERATINGSYSTEM_NAME
#       define SEC_OPERATINGSYSTEM_MAC 1
#       define SEC_OPERATINGSYSTEM_NAME "macOS"
#   else
#       undef SEC_OPERATINGSYSTEM_APPLE_UNKNOWN
#       undef SEC_OPERATINGSYSTEM_UNKNOWN
#       define SEC_OPERATINGSYSTEM_APPLE_UNKNOWN 1
#       define SEC_OPERATINGSYSTEM_UNKNOWN 1
#       warning "Your Apple device is currently unknown. There might be some issues."
#   endif
#elif defined(__linux__)
#   undef SEC_OPERATINGSYSTEM_LINUX
#   undef SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   undef SEC_OPERATINGSYSTEM_NAME
#   define SEC_OPERATINGSYSTEM_LINUX 1
#   define SEC_OPERATINGSYSTEM_POSIX_COMPLIANT 1
#   define SEC_OPERATINGSYSTEM_NAME "GNU/Linux"
#elif defined(__unix__)
#   undef SEC_OPERATINGSYSTEM_UNIX
#   undef SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   undef SEC_OPERATINGSYSTEM_NAME
#   define SEC_OPERATINGSYSTEM_UNIX 1
#   define SEC_OPERATINGSYSTEM_POSIX_COMPLIANT 1
#   define SEC_OPERATINGSYSTEM_NAME "Unix"
#elif defined(__serenity__)
#   undef SEC_OPERATINGSYSTEM_SERENITY
#   undef SEC_OPERATINGSYSTEM_NAME
#   undef SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   define SEC_OPERATINGSYSTEM_SERENITY 1
#   define SEC_OPERATINGSYSTEM_POSIX_COMPLIANT 1
#   define SEC_OPERATINGSYSTEM_NAME "Serenity OS"
#else
#   undef SEC_OPERATINGSYSTEM_UNKNOWN
#   define SEC_OPERATINGSYSTEM_UNKNOWN 1
#   warning "Your Operating System is unknown. This will cause issues."
#endif
