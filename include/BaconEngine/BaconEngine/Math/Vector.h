// Purpose: Vector structures.
// Created on: 3/31/22 @ 1:55 PM

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        short x;
        short y;
    } Vector2S; // Short: [-32,767, 32,767]
    
    typedef struct {
        unsigned short x;
        unsigned short y;
    } Vector2US; // Unsigned Short: [0, 65,535]
    
    typedef struct {
        int x;
        int y;
    } Vector2I; // Int: [-32,767, 32,767]
    
    typedef struct {
        unsigned x;
        unsigned y;
    } Vector2U; // Unsigned Int: [0, 65,535]
    
    typedef struct {
        long x;
        long y;
    } Vector2L; // Long: [-2,147,483,647, 2,147,483,647]
    
    typedef struct {
        unsigned long x;
        unsigned long y;
    } Vector2UL; // Unsigned Long: [0, 4,294,967,295]
    
    typedef struct {
        long long x;
        long long y;
    } Vector2LL; // Long Long: [-9,233,372,036,854,775,807, 9,223,372,036,854,775,807]
    
    typedef struct {
        unsigned long long x;
        unsigned long long y;
    } Vector2ULL; // Unsigned Long Long: [0, 18,446,744,073,709,551,615]
    
    typedef struct {
        float x;
        float y;
    } Vector2F; // Float
    
    typedef struct {
        double x;
        double y;
    } Vector2D; // Double
    
    typedef struct {
        long double x;
        long double y;
    } Vector2LD; // Long Double

    typedef struct {
        short x;
        short y;
        short z;
    } Vector3S; // Short: [-32,767, 32,767]

    typedef struct {
        unsigned short x;
        unsigned short y;
        unsigned short z;
    } Vector3US; // Unsigned Short: [0, 65,535]

    typedef struct {
        int x;
        int y;
        int z;
    } Vector3I; // Int: [-32,767, 32,767]

    typedef struct {
        unsigned x;
        unsigned y;
        unsigned z;
    } Vector3U; // Unsigned Int: [0, 65,535]

    typedef struct {
        long x;
        long y;
        long z;
    } Vector3L; // Long: [-2,147,483,647, 2,147,483,647]

    typedef struct {
        unsigned long x;
        unsigned long y;
        unsigned long z;
    } Vector3UL; // Unsigned Long: [0, 4,294,967,295]

    typedef struct {
        long long x;
        long long y;
        long long z;
    } Vector3LL; // Long Long: [-9,233,372,036,854,775,807, 9,223,372,036,854,775,807]

    typedef struct {
        unsigned long long x;
        unsigned long long y;
        unsigned long long z;
    } Vector3ULL; // Unsigned Long Long: [0, 18,446,744,073,709,551,615]

    typedef struct {
        float x;
        float y;
        float z;
    } Vector3F; // Float

    typedef struct {
        double x;
        double y;
        double z;
    } Vector3D; // Double

    typedef struct {
        long double x;
        long double y;
        long double z;
    } Vector3LD; // Long Double
    
    typedef struct {
        short x;
        short y;
        short z;
        short w;
    } Vector4S; // Short: [-32,767, 32,767]

    typedef struct {
        unsigned short x;
        unsigned short y;
        unsigned short z;
        unsigned short w;
    } Vector4US; // Unsigned Short: [0, 65,535]

    typedef struct {
        int x;
        int y;
        int z;
        int w;
    } Vector4I; // Int: [-32,767, 32,767]

    typedef struct {
        unsigned x;
        unsigned y;
        unsigned z;
        unsigned w;
    } Vector4U; // Unsigned Int: [0, 65,535]

    typedef struct {
        long x;
        long y;
        long z;
        long w;
    } Vector4L; // Long: [-2,147,483,647, 2,147,483,647]

    typedef struct {
        unsigned long x;
        unsigned long y;
        unsigned long z;
        unsigned long w;
    } Vector4UL; // Unsigned Long: [0, 4,294,967,295]

    typedef struct {
        long long x;
        long long y;
        long long z;
        long long w;
    } Vector4LL; // Long Long: [-9,233,372,036,854,775,807, 9,223,372,036,854,775,807]

    typedef struct {
        unsigned long long x;
        unsigned long long y;
        unsigned long long z;
        unsigned long long w;
    } Vector4ULL; // Unsigned Long Long: [0, 18,446,744,073,709,551,615]

    typedef struct {
        float x;
        float y;
        float z;
        float w;
    } Vector4F; // Float

    typedef struct {
        double x;
        double y;
        double z;
        double w;
    } Vector4D; // Double

    typedef struct {
        long double x;
        long double y;
        long double z;
        long double w;
    } Vector4LD; // Long Double

#ifdef __cplusplus
};
#endif