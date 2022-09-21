// Purpose: Vector structures.
// Created on: 3/31/22 @ 1:55 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

SEC_CPP_GUARD_START()
    typedef struct {
        int x;
        int y;
    } BE_Vector_2I; // Int

    typedef struct {
        unsigned x;
        unsigned y;
    } BE_Vector_2U; // Unsigned Int

    typedef struct {
        float x;
        float y;
    } BE_Vector_2F; // Float

    typedef struct {
        int x;
        int y;
        int z;
    } BE_Vector_3I; // Int

    typedef struct {
        unsigned x;
        unsigned y;
        unsigned z;
    } BE_Vector_3U; // Unsigned Int

    typedef struct {
        float x;
        float y;
        float z;
    } BE_Vector_3F; // Float

    typedef struct {
        int x;
        int y;
        int z;
        int w;
    } BE_Vector_4I; // Int

    typedef struct {
        unsigned x;
        unsigned y;
        unsigned z;
        unsigned w;
    } BE_Vector_4U; // Unsigned Int

    typedef struct {
        float x;
        float y;
        float z;
        float w;
    } BE_Vector_4F; // Float
SEC_CPP_GUARD_END()
