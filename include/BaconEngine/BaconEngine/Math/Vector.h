// Purpose: Vector structures.
// Created on: 3/31/22 @ 1:55 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

SEC_CPP_GUARD_START()
    typedef struct {
        short x;
        short y;
    } BE_Vector_2S; // Short: [-32,767, 32,767]
    
    typedef struct {
        unsigned short x;
        unsigned short y;
    } BE_Vector_2US; // Unsigned Short: [0, 65,535]
    
    typedef struct {
        int x;
        int y;
    } BE_Vector_2I; // Int: [-32,767, 32,767]
    
    typedef struct {
        unsigned x;
        unsigned y;
    } BE_Vector_2U; // Unsigned Int: [0, 65,535]
    
    typedef struct {
        long x;
        long y;
    } BE_Vector_2L; // Long: [-2,147,483,647, 2,147,483,647]
    
    typedef struct {
        unsigned long x;
        unsigned long y;
    } BE_Vector_2UL; // Unsigned Long: [0, 4,294,967,295]
    
    typedef struct {
        long long x;
        long long y;
    } BE_Vector_2LL; // Long Long: [-9,233,372,036,854,775,807, 9,223,372,036,854,775,807]
    
    typedef struct {
        unsigned long long x;
        unsigned long long y;
    } BE_Vector_2ULL; // Unsigned Long Long: [0, 18,446,744,073,709,551,615]
    
    typedef struct {
        float x;
        float y;
    } BE_Vector_2F; // Float
    
    typedef struct {
        double x;
        double y;
    } BE_Vector_2D; // Double
    
    typedef struct {
        long double x;
        long double y;
    } BE_Vector_2LD; // Long Double

    typedef struct {
        short x;
        short y;
        short z;
    } BE_Vector_3S; // Short: [-32,767, 32,767]

    typedef struct {
        unsigned short x;
        unsigned short y;
        unsigned short z;
    } BE_Vector_3US; // Unsigned Short: [0, 65,535]

    typedef struct {
        int x;
        int y;
        int z;
    } BE_Vector_3I; // Int: [-32,767, 32,767]

    typedef struct {
        unsigned x;
        unsigned y;
        unsigned z;
    } BE_Vector_3U; // Unsigned Int: [0, 65,535]

    typedef struct {
        long x;
        long y;
        long z;
    } BE_Vector_3L; // Long: [-2,147,483,647, 2,147,483,647]

    typedef struct {
        unsigned long x;
        unsigned long y;
        unsigned long z;
    } BE_Vector_3UL; // Unsigned Long: [0, 4,294,967,295]

    typedef struct {
        long long x;
        long long y;
        long long z;
    } BE_Vector_3LL; // Long Long: [-9,233,372,036,854,775,807, 9,223,372,036,854,775,807]

    typedef struct {
        unsigned long long x;
        unsigned long long y;
        unsigned long long z;
    } BE_Vector_3ULL; // Unsigned Long Long: [0, 18,446,744,073,709,551,615]

    typedef struct {
        float x;
        float y;
        float z;
    } BE_Vector_3F; // Float

    typedef struct {
        double x;
        double y;
        double z;
    } BE_Vector_3D; // Double

    typedef struct {
        long double x;
        long double y;
        long double z;
    } BE_Vector_3LD; // Long Double
    
    typedef struct {
        short x;
        short y;
        short z;
        short w;
    } BE_Vector_4S; // Short: [-32,767, 32,767]

    typedef struct {
        unsigned short x;
        unsigned short y;
        unsigned short z;
        unsigned short w;
    } BE_Vector_4US; // Unsigned Short: [0, 65,535]

    typedef struct {
        int x;
        int y;
        int z;
        int w;
    } BE_Vector_4I; // Int: [-32,767, 32,767]

    typedef struct {
        unsigned x;
        unsigned y;
        unsigned z;
        unsigned w;
    } BE_Vector_4U; // Unsigned Int: [0, 65,535]

    typedef struct {
        long x;
        long y;
        long z;
        long w;
    } BE_Vector_4L; // Long: [-2,147,483,647, 2,147,483,647]

    typedef struct {
        unsigned long x;
        unsigned long y;
        unsigned long z;
        unsigned long w;
    } BE_Vector_4UL; // Unsigned Long: [0, 4,294,967,295]

    typedef struct {
        long long x;
        long long y;
        long long z;
        long long w;
    } BE_Vector_4LL; // Long Long: [-9,233,372,036,854,775,807, 9,223,372,036,854,775,807]

    typedef struct {
        unsigned long long x;
        unsigned long long y;
        unsigned long long z;
        unsigned long long w;
    } BE_Vector_4ULL; // Unsigned Long Long: [0, 18,446,744,073,709,551,615]

    typedef struct {
        float x;
        float y;
        float z;
        float w;
    } BE_Vector_4F; // Float

    typedef struct {
        double x;
        double y;
        double z;
        double w;
    } BE_Vector_4D; // Double

    typedef struct {
        long double x;
        long double y;
        long double z;
        long double w;
    } BE_Vector_4LD; // Long Double
SEC_CPP_GUARD_END()

#define BE_VECTOR_CREATE(type, ...) (BE_Vector_ ## type ) {__VA_ARGS__}
