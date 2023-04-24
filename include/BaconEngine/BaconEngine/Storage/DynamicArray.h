// Purpose: Stores a variable amount of elements.
// Created on: 8/8/22 @ 3:50 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <stddef.h>
#include <SharedEngineCode/Internal/CppSupport.h>
#include <SharedEngineCode/Internal/Boolean.h>

#include "BaconEngine/DllExport.h"

SEC_CPP_SUPPORT_GUARD_START()
typedef struct {
    void** internalArray;
    int used;
    size_t size;
    int calledRealloc;
    SEC_Boolean frozen;
    SEC_Boolean engineMade; // Do not edit, internal use only
} BE_DynamicArray;

BE_DLLEXPORT SEC_Boolean BE_DynamicArray_Create(BE_DynamicArray* array, size_t size);
BE_DLLEXPORT SEC_Boolean BE_DynamicArray_AddElementToStart(BE_DynamicArray* array, void* element);
BE_DLLEXPORT SEC_Boolean BE_DynamicArray_AddElementToLast(BE_DynamicArray* array, void* element);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BE_DLLEXPORT SEC_Boolean BE_DynamicArray_RemoveFirstElement(BE_DynamicArray* array);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BE_DLLEXPORT SEC_Boolean BE_DynamicArray_RemoveLastElement(BE_DynamicArray* array);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BE_DLLEXPORT SEC_Boolean BE_DynamicArray_RemoveElementAt(BE_DynamicArray* array, unsigned int index);
SEC_CPP_SUPPORT_GUARD_END()

#define BE_DYNAMICARRAY_GET_ELEMENT_POINTER(type, array, index) ((type*) (array)->internalArray[(index)])
#define BE_DYNAMICARRAY_GET_ELEMENT(type, array, index) BE_DYNAMICARRAY_GET_ELEMENT_POINTER(type, &array, index)
#define BE_DYNAMICARRAY_GET_LAST_ELEMENT_POINTER(type, array) BE_DYNAMICARRAY_GET_ELEMENT_POINTER(type, array, array->used - 1)
#define BE_DYNAMICARRAY_GET_LAST_ELEMENT(type, array) BE_DYNAMICARRAY_GET_ELEMENT(type, array, array.used - 1)
