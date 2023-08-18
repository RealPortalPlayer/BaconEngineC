// Purpose: Stores a variable amount of elements.
// Created on: 8/8/22 @ 3:50 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <stddef.h>

#include "SharedEngineCode/Internal/CPlusPlusSupport.h"
#include "SharedEngineCode/Internal/Boolean.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    void** internalArray;
    int used;
    size_t size;
    int calledReallocate;
    SEC_Boolean frozen;
} SEC_DynamicArray;

/**
 * @return The index if the element was found, -1 if not
 */
int SEC_DynamicArray_GetIndexForElement(SEC_DynamicArray* array, void* element, size_t elementSize);

SEC_Boolean SEC_DynamicArray_Create(SEC_DynamicArray* array, size_t size);
SEC_Boolean SEC_DynamicArray_AddElementToStart(SEC_DynamicArray* array, void* element);
SEC_Boolean SEC_DynamicArray_AddElementToLast(SEC_DynamicArray* array, void* element);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
SEC_Boolean SEC_DynamicArray_RemoveFirstElement(SEC_DynamicArray* array);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
SEC_Boolean SEC_DynamicArray_RemoveLastElement(SEC_DynamicArray* array);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
SEC_Boolean SEC_DynamicArray_RemoveElementAt(SEC_DynamicArray* array, unsigned int index);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
SEC_Boolean SEC_DynamicArray_RemoveMatchedElement(SEC_DynamicArray* array, void* element, size_t elementSize, SEC_Boolean repeat);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()

#define SEC_DYNAMICARRAY_GET_ELEMENT_POINTER(type, array, index) ((type*) (array)->internalArray[(index)])
#define SEC_DYNAMICARRAY_GET_ELEMENT(type, array, index) SEC_DYNAMICARRAY_GET_ELEMENT_POINTER(type, &array, index)
#define SEC_DYNAMICARRAY_GET_LAST_ELEMENT_POINTER(type, array) SEC_DYNAMICARRAY_GET_ELEMENT_POINTER(type, array, array->used - 1)
#define SEC_DYNAMICARRAY_GET_LAST_ELEMENT(type, array) SEC_DYNAMICARRAY_GET_ELEMENT(type, array, array.used - 1)
