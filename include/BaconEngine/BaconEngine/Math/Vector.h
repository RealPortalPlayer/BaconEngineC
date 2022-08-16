// Purpose: Vector structures.
// Created on: 3/31/22 @ 1:55 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

CPP_GUARD_START()
    typedef struct {
        short x;
        short y;
    } BE_Vector2S; // Short: [-32,767, 32,767]
    
    typedef struct {
        unsigned short x;
        unsigned short y;
    } BE_Vector2US; // Unsigned Short: [0, 65,535]
    
    typedef struct {
        int x;
        int y;
    } BE_Vector2I; // Int: [-32,767, 32,767]
    
    typedef struct {
        unsigned x;
        unsigned y;
    } BE_Vector2U; // Unsigned Int: [0, 65,535]
    
    typedef struct {
        long x;
        long y;
    } BE_Vector2L; // Long: [-2,147,483,647, 2,147,483,647]
    
    typedef struct {
        unsigned long x;
        unsigned long y;
    } BE_Vector2UL; // Unsigned Long: [0, 4,294,967,295]
    
    typedef struct {
        long long x;
        long long y;
    } BE_Vector2LL; // Long Long: [-9,233,372,036,854,775,807, 9,223,372,036,854,775,807]
    
    typedef struct {
        unsigned long long x;
        unsigned long long y;
    } BE_Vector2ULL; // Unsigned Long Long: [0, 18,446,744,073,709,551,615]
    
    typedef struct {
        float x;
        float y;
    } BE_Vector2F; // Float
    
    typedef struct {
        double x;
        double y;
    } BE_Vector2D; // Double
    
    typedef struct {
        long double x;
        long double y;
    } BE_Vector2LD; // Long Double

    typedef struct {
        short x;
        short y;
        short z;
    } BE_Vector3S; // Short: [-32,767, 32,767]

    typedef struct {
        unsigned short x;
        unsigned short y;
        unsigned short z;
    } BE_Vector3US; // Unsigned Short: [0, 65,535]

    typedef struct {
        int x;
        int y;
        int z;
    } BE_Vector3I; // Int: [-32,767, 32,767]

    typedef struct {
        unsigned x;
        unsigned y;
        unsigned z;
    } BE_Vector3U; // Unsigned Int: [0, 65,535]

    typedef struct {
        long x;
        long y;
        long z;
    } BE_Vector3L; // Long: [-2,147,483,647, 2,147,483,647]

    typedef struct {
        unsigned long x;
        unsigned long y;
        unsigned long z;
    } BE_Vector3UL; // Unsigned Long: [0, 4,294,967,295]

    typedef struct {
        long long x;
        long long y;
        long long z;
    } BE_Vector3LL; // Long Long: [-9,233,372,036,854,775,807, 9,223,372,036,854,775,807]

    typedef struct {
        unsigned long long x;
        unsigned long long y;
        unsigned long long z;
    } BE_Vector3ULL; // Unsigned Long Long: [0, 18,446,744,073,709,551,615]

    typedef struct {
        float x;
        float y;
        float z;
    } BE_Vector3F; // Float

    typedef struct {
        double x;
        double y;
        double z;
    } BE_Vector3D; // Double

    typedef struct {
        long double x;
        long double y;
        long double z;
    } BE_Vector3LD; // Long Double
    
    typedef struct {
        short x;
        short y;
        short z;
        short w;
    } BE_Vector4S; // Short: [-32,767, 32,767]

    typedef struct {
        unsigned short x;
        unsigned short y;
        unsigned short z;
        unsigned short w;
    } BE_Vector4US; // Unsigned Short: [0, 65,535]

    typedef struct {
        int x;
        int y;
        int z;
        int w;
    } BE_Vector4I; // Int: [-32,767, 32,767]

    typedef struct {
        unsigned x;
        unsigned y;
        unsigned z;
        unsigned w;
    } BE_Vector4U; // Unsigned Int: [0, 65,535]

    typedef struct {
        long x;
        long y;
        long z;
        long w;
    } BE_Vector4L; // Long: [-2,147,483,647, 2,147,483,647]

    typedef struct {
        unsigned long x;
        unsigned long y;
        unsigned long z;
        unsigned long w;
    } BE_Vector4UL; // Unsigned Long: [0, 4,294,967,295]

    typedef struct {
        long long x;
        long long y;
        long long z;
        long long w;
    } BE_Vector4LL; // Long Long: [-9,233,372,036,854,775,807, 9,223,372,036,854,775,807]

    typedef struct {
        unsigned long long x;
        unsigned long long y;
        unsigned long long z;
        unsigned long long w;
    } BE_Vector4ULL; // Unsigned Long Long: [0, 18,446,744,073,709,551,615]

    typedef struct {
        float x;
        float y;
        float z;
        float w;
    } BE_Vector4F; // Float

    typedef struct {
        double x;
        double y;
        double z;
        double w;
    } BE_Vector4D; // Double

    typedef struct {
        long double x;
        long double y;
        long double z;
        long double w;
    } BE_Vector4LD; // Long Double
CPP_GUARD_END()
