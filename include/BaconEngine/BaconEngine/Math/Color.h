// Purpose: Data that represent color.
// Created on: 4/26/22 @ 11:04 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

CPP_GUARD_START()
    typedef struct {
        short r;
        short g;
        short b;
    } BE_Color3S; // Short: [-32,767, 32,767]

    typedef struct {
        unsigned short r;
        unsigned short g;
        unsigned short b;
    } BE_Color3US; // Unsigned Short: [0, 65,535]

    typedef struct {
        int r;
        int g;
        int b;
    } BE_Color3I; // Int: [-32,767, 32,767]

    typedef struct {
        unsigned r;
        unsigned g;
        unsigned b;
    } BE_Color3U; // Unsigned Int: [0, 65,535]

    typedef struct {
        long r;
        long g;
        long b;
    } BE_Color3L; // Long: [-2,147,483,647, 2,147,483,647]

    typedef struct {
        unsigned long r;
        unsigned long g;
        unsigned long b;
    } BE_Color3UL; // Unsigned Long: [0, 4,294,967,295]

    typedef struct {
        long long r;
        long long g;
        long long b;
    } BE_Color3LL; // Long Long: [-9,233,372,036,854,775,807, 9,223,372,036,854,775,807]

    typedef struct {
        unsigned long long r;
        unsigned long long g;
        unsigned long long b;
    } BE_Color3ULL; // Unsigned Long Long: [0, 18,446,744,073,709,551,615]

    typedef struct {
        float r;
        float g;
        float b;
    } BE_Color3F; // Float

    typedef struct {
        double r;
        double g;
        double b;
    } BE_Color3D; // Double

    typedef struct {
        long double r;
        long double g;
        long double b;
    } BE_Color3LD; // Long Double

    typedef struct {
        short r;
        short g;
        short b;
        short a;
    } BE_Color4S; // Short: [-32,767, 32,767]

    typedef struct {
        unsigned short r;
        unsigned short g;
        unsigned short b;
        unsigned short a;
    } BE_Color4US; // Unsigned Short: [0, 65,535]

    typedef struct {
        int r;
        int g;
        int b;
        int a;
    } BE_Color4I; // Int: [-32,767, 32,767]

    typedef struct {
        unsigned r;
        unsigned g;
        unsigned b;
        unsigned a;
    } BE_Color4U; // Unsigned Int: [0, 65,535]

    typedef struct {
        long r;
        long g;
        long b;
        long a;
    } BE_Color4L; // Long: [-2,147,483,647, 2,147,483,647]

    typedef struct {
        unsigned long r;
        unsigned long g;
        unsigned long b;
        unsigned long a;
    } BE_Color4UL; // Unsigned Long: [0, 4,294,967,295]

    typedef struct {
        long long r;
        long long g;
        long long b;
        long long a;
    } BE_Color4LL; // Long Long: [-9,233,372,036,854,775,807, 9,223,372,036,854,775,807]

    typedef struct {
        unsigned long long r;
        unsigned long long g;
        unsigned long long b;
        unsigned long long a;
    } BE_Color4ULL; // Unsigned Long Long: [0, 18,446,744,073,709,551,615]

    typedef struct {
        float r;
        float g;
        float b;
        float a;
    } BE_Color4F; // Float

    typedef struct {
        double r;
        double g;
        double b;
        double a;
    } BE_Color4D; // Double

    typedef struct {
        long double r;
        long double g;
        long double b;
        long double a;
    } BE_Color4LD; // Long Double
CPP_GUARD_END()
