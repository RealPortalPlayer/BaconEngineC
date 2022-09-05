// Purpose: Stores a variable amount of elements.
// Created on: 8/8/22 @ 3:50 PM

#pragma once

#include "SharedEngineCode/Internal/CppHeader.h"
#include <stddef.h>

SEC_CPP_GUARD_START()
    typedef struct {
        void** internalArray;
        int used;
        size_t size;
        int calledRealloc;
        int frozen;
    } SEC_DynamicArray;

    int SEC_DynamicArray_Create(SEC_DynamicArray* array, size_t size);
    int SEC_DynamicArray_AddElementToStart(SEC_DynamicArray* array, void* element);
    int SEC_DynamicArray_AddElementToLast(SEC_DynamicArray* array, void* element);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int SEC_DynamicArray_RemoveFirstElement(SEC_DynamicArray* array, int shift);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int SEC_DynamicArray_RemoveLastElement(SEC_DynamicArray* array);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int SEC_DynamicArray_RemoveElementAt(SEC_DynamicArray* array, unsigned int index);

    void SEC_DynamicArray_Shrink(SEC_DynamicArray* array);
SEC_CPP_GUARD_END()

#define SEC_DYNAMICARRAY_GET_ELEMENT(type, array, index) ((type*) (array).internalArray[index])
#define SEC_DYNAMICARRAY_GET_ELEMENT_PTR(type, array, index) ((type*) (array)->internalArray[index])
