// Purpose: Simplifies a guard header that makes sure this should work with C++.
// Created on: 7/29/22 @ 2:36 PM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#ifdef __cplusplus
#   define SEC_CPP_SUPPORT_GUARD_START() extern "C" {
#   define SEC_CPP_SUPPORT_GUARD_END() };
#   define SEC_CPP_SUPPORT_CREATE_STRUCT(type, ...) type(__VA_ARGS__)
#else
#   define SEC_CPP_SUPPORT_GUARD_START()
#   define SEC_CPP_SUPPORT_GUARD_END()
#   define SEC_CPP_SUPPORT_CREATE_STRUCT(type, ...) (type) {__VA_ARGS__}
#endif
