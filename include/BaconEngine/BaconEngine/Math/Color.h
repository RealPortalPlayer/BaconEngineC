// Purpose: Data that represent color.
// Created on: 4/26/22 @ 11:04 PM

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        short r;
        short g;
        short b;
    } Color3S; // Short: [-32,767, 32,767]

    typedef struct {
        unsigned short r;
        unsigned short g;
        unsigned short b;
    } Color3US; // Unsigned Short: [0, 65,535]

    typedef struct {
        int r;
        int g;
        int b;
    } Color3I; // Int: [-32,767, 32,767]

    typedef struct {
        unsigned r;
        unsigned g;
        unsigned b;
    } Color3U; // Unsigned Int: [0, 65,535]

    typedef struct {
        long r;
        long g;
        long b;
    } Color3L; // Long: [-2,147,483,647, 2,147,483,647]

    typedef struct {
        unsigned long r;
        unsigned long g;
        unsigned long b;
    } Color3UL; // Unsigned Long: [0, 4,294,967,295]

    typedef struct {
        long long r;
        long long g;
        long long b;
    } Color3LL; // Long Long: [-9,233,372,036,854,775,807, 9,223,372,036,854,775,807]

    typedef struct {
        unsigned long long r;
        unsigned long long g;
        unsigned long long b;
    } Color3ULL; // Unsigned Long Long: [0, 18,446,744,073,709,551,615]

    typedef struct {
        float r;
        float g;
        float b;
    } Color3F; // Float

    typedef struct {
        double r;
        double g;
        double b;
    } Color3D; // Double

    typedef struct {
        long double r;
        long double g;
        long double b;
    } Color3LD; // Long Double

    typedef struct {
        short r;
        short g;
        short b;
        short a;
    } Color4S; // Short: [-32,767, 32,767]

    typedef struct {
        unsigned short r;
        unsigned short g;
        unsigned short b;
        unsigned short a;
    } Color4US; // Unsigned Short: [0, 65,535]

    typedef struct {
        int r;
        int g;
        int b;
        int a;
    } Color4I; // Int: [-32,767, 32,767]

    typedef struct {
        unsigned r;
        unsigned g;
        unsigned b;
        unsigned a;
    } Color4U; // Unsigned Int: [0, 65,535]

    typedef struct {
        long r;
        long g;
        long b;
        long a;
    } Color4L; // Long: [-2,147,483,647, 2,147,483,647]

    typedef struct {
        unsigned long r;
        unsigned long g;
        unsigned long b;
        unsigned long a;
    } Color4UL; // Unsigned Long: [0, 4,294,967,295]

    typedef struct {
        long long r;
        long long g;
        long long b;
        long long a;
    } Color4LL; // Long Long: [-9,233,372,036,854,775,807, 9,223,372,036,854,775,807]

    typedef struct {
        unsigned long long r;
        unsigned long long g;
        unsigned long long b;
        unsigned long long a;
    } Color4ULL; // Unsigned Long Long: [0, 18,446,744,073,709,551,615]

    typedef struct {
        float r;
        float g;
        float b;
        float a;
    } Color4F; // Float

    typedef struct {
        double r;
        double g;
        double b;
        double a;
    } Color4D; // Double

    typedef struct {
        long double r;
        long double g;
        long double b;
        long double a;
    } Color4LD; // Long Double

#ifdef __cplusplus
};
#endif