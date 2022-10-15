// Purpose: Stores a variable amount of elements.
// Created on: 8/8/22 @ 3:50 PM

#pragma once

#include <stddef.h>

#include "SharedEngineCode/Internal/CppSupport.h"

SEC_CPP_SUPPORT_GUARD_START()
typedef struct {
    void** internalArray;
    int used;
    size_t size;
    int calledRealloc;
    int frozen;
} BE_DynamicArray;

int BE_DynamicArray_Create(BE_DynamicArray* array, size_t size);
int BE_DynamicArray_AddElementToStart(BE_DynamicArray* array, void* element);
int BE_DynamicArray_AddElementToLast(BE_DynamicArray* array, void* element);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
int BE_DynamicArray_RemoveFirstElement(BE_DynamicArray* array, int shift);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
int BE_DynamicArray_RemoveLastElement(BE_DynamicArray* array);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
int BE_DynamicArray_RemoveElementAt(BE_DynamicArray* array, unsigned int index);

void BE_DynamicArray_Shrink(BE_DynamicArray* array);
SEC_CPP_SUPPORT_GUARD_END()

#define BE_DYNAMICARRAY_GET_ELEMENT(type, array, index) ((type*) (array).internalArray[index])
#define BE_DYNAMICARRAY_GET_ELEMENT_PTR(type, array, index) ((type*) (array)->internalArray[index])
#define BE_DYNAMICARRAY_GET_LAST_ELEMENT(type, array) BE_DYNAMICARRAY_GET_ELEMENT(type, (array), (array).used - 1)
#define BE_DYNAMICARRAY_GET_LAST_ELEMENT_PTR(type, array) BE_DYNAMICARRAY_GET_ELEMENT_PTR(type, array, array->used - 1)