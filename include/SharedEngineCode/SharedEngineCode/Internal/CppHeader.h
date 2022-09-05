// Purpose: Simplifies a guard header that makes sure this should work with C++.
// Created on: 7/29/22 @ 2:36 PM

#ifdef __cplusplus
#   define CPP_GUARD_START() extern "C" {
#   define CPP_GUARD_END() };
#else
#   define SEC_CPP_GUARD_START()
#   define SEC_CPP_GUARD_END()
#endif
