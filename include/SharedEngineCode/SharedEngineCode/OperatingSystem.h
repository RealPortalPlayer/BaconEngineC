// Purpose: OS specific definitions.
// Created on: 4/1/22 @ 7:25 PM

#define OS_WINDOWS 0
#define OS_APPLE 0
#define OS_POSIX_COMPLIANT 0
#define OS_IPHONE_SIMULATOR 0
#define OS_MAC_CATALYST 0
#define OS_IPHONE 0
#define OS_MAC 0
#define OS_APPLE_UNKNOWN 0
#define OS_UNKNOWN 0
#define OS_LINUX 0
#define OS_UNIX 0

#if defined(WIN32)
#   undef OS_WINDOWS
#   define OS_WINDOWS 1
#elif defined(__APPLE__)
#   include <TargetConditionals.h>
#   undef OS_APPLE
#   undef OS_POSIX_COMPLIANT
#   undef OS_IPHONE_SIMULATOR
#   undef OS_MAC_CATALYST
#   undef OS_IPHONE
#   undef OS_MAC
#   undef OS_APPLE_UNKNOWN
#   undef OS_UNKNOWN
#   define OS_APPLE 1
#   define OS_POSIX_COMPLIANT 1
#   if TARGET_IPHONE_SIMULATOR
#       define OS_IPHONE_SIMULATOR 1
#   elif TARGET_OS_MACCATALYST
#       define OS_MAC_CATALYST 1
#   elif TARGET_OS_IPHONE
#       define OS_IPHONE 1
#   elif TARGET_OS_MAC
#       define OS_MAC 1
#   else
#       define OS_APPLE_UNKNOWN 1
#       define OS_UNKNOWN 1
#       warning "Your Apple device is currently unknown. There might be some issues."
#   endif
#elif defined(__linux__)
#   undef OS_LINUX
#   undef OS_POSIX_COMPLIANT
#   define OS_LINUX 1
#   define OS_POSIX_COMPLIANT 1
#elif defined(__unix__)
#   undef OS_UNIX
#   undef OS_POSIX_COMPLIANT
#   define OS_UNIX 1
#   define OS_POSIX_COMPLIANT 1
#else
#   undef OS_UNKNOWN
#   define OS_UNKNOWN 1
#   warning "Your Operating System is unknown. This will cause issues."
#endif