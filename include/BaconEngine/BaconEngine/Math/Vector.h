// Purpose: Vector structures.
// Created on: 3/31/22 @ 1:55 PM

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int x;
    int y;
} Vector2I;

typedef struct {
    unsigned int x;
    unsigned int y;
} Vector2UI;

typedef struct {
    long x;
    long y;
} Vector2L;

typedef struct {
    unsigned long x;
    unsigned long y;
} Vector2UL;

typedef struct {
    long long x;
    long long y;
} Vector2LL;

typedef struct {
    float x;
    float y;
} Vector2F;

typedef struct {
    double x;
    double y;
} Vector2D;

typedef struct {
    int x;
    int y;
    int z;
} Vector3I;

typedef struct {
    unsigned int x;
    unsigned int y;
    unsigned int z;
} Vector3UI;

typedef struct {
    long x;
    long y;
    long z;
} Vector3L;

typedef struct {
    unsigned long x;
    unsigned long y;
    unsigned long z;
} Vector3UL;

typedef struct {
    long long x;
    long long y;
    long long z;
} Vector3LL;

typedef struct {
    float x;
    float y;
    float z;
} Vector3F;

typedef struct {
    double x;
    double y;
    double z;
} Vector3D;

typedef struct {
    int x;
    int y;
    int z;
    int w;
} Vector4I;

typedef struct {
    unsigned int x;
    unsigned int y;
    unsigned int z;
    unsigned int w;
} Vector4UI;

typedef struct {
    long x;
    long y;
    long z;
    long w;
} Vector4L;

typedef struct {
    unsigned long x;
    unsigned long y;
    unsigned long z;
    unsigned long w;
} Vector4UL;

typedef struct {
    long long x;
    long long y;
    long long z;
    long long w;
} Vector4LL;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vector4F;

typedef struct {
    double x;
    double y;
    double z;
    double w;
} Vector4D;

#ifdef __cplusplus
};
#endif