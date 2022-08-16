// Purpose: Stores a variable amount of elements.
// Created on: 8/8/22 @ 3:50 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>
#include <stddef.h>

CPP_GUARD_START()
    typedef struct {
        void** internalArray;
        int used;
        size_t size;
        int calledRealloc;
    } BE_DynamicArray;

    int BE_CreateDynamicArray(BE_DynamicArray* array, size_t size);
    int BE_ArrayAddElementToFirst(BE_DynamicArray* array, void* element);
    int BE_ArrayAddElementToLast(BE_DynamicArray* array, void* element);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int BE_ArrayRemoveFirstElement(BE_DynamicArray* array, int shift);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int BE_ArrayRemoveLastElement(BE_DynamicArray* array);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int BE_ArrayRemoveElementAt(BE_DynamicArray* array, unsigned int index);
CPP_GUARD_END()

#define BE_ARRAY_GET_ELEMENT(type, array, index) ((type*) (array).internalArray[index])
#define BE_ARRAY_GET_ELEMENT_PTR(type, array, index) ((type*) (array)->internalArray[index])
