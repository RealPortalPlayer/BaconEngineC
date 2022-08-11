// Purpose: Stores a variable amount of elements.
// Created on: 8/8/22 @ 3:50 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

CPP_GUARD_START()
    typedef struct {
        void** internalArray;
        int used;
        size_t size;
        int calledRealloc;
    } DynamicArray;

    int CreateDynamicArray(DynamicArray* array, size_t size);
    int ArrayAddElementToFirst(DynamicArray* array, void* element);
    int ArrayAddElementToLast(DynamicArray* array, void* element);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int ArrayRemoveFirstElement(DynamicArray* array, int shift);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int ArrayRemoveLastElement(DynamicArray* array);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int ArrayRemoveElementAt(DynamicArray* array, unsigned int index);
CPP_GUARD_END()

#define GET_ELEMENT(type, array, index) ((type*) (array).internalArray[index])
#define GET_ELEMENT_PTR(type, array, index) ((type*) (array)->internalArray[index])
