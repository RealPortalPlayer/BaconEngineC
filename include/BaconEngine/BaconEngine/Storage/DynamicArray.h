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
    int ArrayUnshiftElement(DynamicArray* array, void* element); // Add element to the beginning
    int ArrayPushElement(DynamicArray* array, void* element); // Add element to the end

    /**
     * Remove the first element.
     * @param array
     * @param shift - Should we shift all the elements to fill that spot?
     * @return If it was successful.
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int ArrayShiftElement(DynamicArray* array, int shift);

    /**
     * Remove the last element.
     * @param array
     * @return If it was successful.
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int ArrayPopElement(DynamicArray* array);

    int ArrayRemoveElementAt(DynamicArray* array, unsigned int index);
CPP_GUARD_END()

#define GET_ELEMENT(type, array, index) ((type*) (array).internalArray[index])
#define GET_ELEMENT_PTR(type, array, index) ((type*) (array)->internalArray[index])
