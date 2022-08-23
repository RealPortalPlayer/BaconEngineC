// Purpose: Stores a variable amount of elements.
// Created on: 8/8/22 @ 3:50 PM

#pragma once

#include "SharedEngineCode/Internal/CppHeader.h"
#include <stddef.h>

CPP_GUARD_START()
    typedef struct {
        void** internalArray;
        int used;
        size_t size;
        int calledRealloc;
    } SEC_DynamicArray;

    int SEC_CreateDynamicArray(SEC_DynamicArray* array, size_t size);
    int SEC_ArrayAddElementToFirst(SEC_DynamicArray* array, void* element);
    int SEC_ArrayAddElementToLast(SEC_DynamicArray* array, void* element);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int SEC_ArrayRemoveFirstElement(SEC_DynamicArray* array, int shift);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int SEC_ArrayRemoveLastElement(SEC_DynamicArray* array);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int SEC_ArrayRemoveElementAt(SEC_DynamicArray* array, unsigned int index);
CPP_GUARD_END()

#define SEC_ARRAY_GET_ELEMENT(type, array, index) ((type*) (array).internalArray[index])
#define SEC_ARRAY_GET_ELEMENT_PTR(type, array, index) ((type*) (array)->internalArray[index])
