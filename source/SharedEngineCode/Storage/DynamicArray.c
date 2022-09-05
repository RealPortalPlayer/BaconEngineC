#include "SharedEngineCode/Internal/CppHeader.h"
#include <stdlib.h>
#include <string.h>

#include "SharedEngineCode/Storage/DynamicArray.h"
#include "SharedEngineCode/Debugging/StrictMode.h"
#include "SharedEngineCode/Debugging/Assert.h"

SEC_CPP_GUARD_START()
    void ReallocateArray(SEC_DynamicArray* array) {
        if (array->size != array->used)
            return;

        SEC_LOGGER_TRACE("Ran out of free space, expanding array\nThis is expensive, so you should try avoiding it");

        array->size *= 2;
        array->calledRealloc++;

        SEC_ASSERT_REALLOC(array->internalArray, sizeof(void*) * (array->size / 2), sizeof(void*) * array->size, "a dynamic array");
    }

    int SEC_DynamicArray_Create(SEC_DynamicArray* array, size_t size) {
        SEC_STRICTMODE_CHECK(size != 0, 0, "Invalid size");
        SEC_ASSERT_MALLOC(array->internalArray, sizeof(void*) * size, "a dynamic array");

        array->used = 0;
        array->size = size;

        return 1;
    }

    int SEC_DynamicArray_AddElementToStart(SEC_DynamicArray* array, void* element) {
        if (array->frozen)
            return 0;

        ReallocateArray(array);
        (void) element;

        return 0;
    }

    int SEC_DynamicArray_AddElementToLast(SEC_DynamicArray* array, void* element) {
        if (array->frozen)
            return 0;

        ReallocateArray(array);

        array->internalArray[array->used++] = element;

        return 1;
    }

    int SEC_DynamicArray_RemoveFirstElement(SEC_DynamicArray* array, int shift) {
        if (array->used == 0 || array->frozen)
            return 0;

        if (!shift) {
            array->internalArray[0] = NULL;
            return 1;
        }

        return SEC_DynamicArray_RemoveElementAt(array, 0);
    }

    int SEC_DynamicArray_RemoveLastElement(SEC_DynamicArray* array) {
        if (array->used == 0 || array->frozen)
            return 0;

        array->internalArray[--array->used] = NULL;

        return 1;
    }

    int SEC_DynamicArray_RemoveElementAt(SEC_DynamicArray* array, unsigned int index) {
        if ((int) index >= array->used || array->frozen)
            return 0;

        ReallocateArray(array);

        for (unsigned int windowId = index; (int) windowId < array->used; windowId++)
            array->internalArray[windowId] = array->internalArray[windowId + 1];

        array->used--;
        return 1;
    }

    void SEC_DynamicArray_Shrink(SEC_DynamicArray* array) {
        if (array->size == 0 || array->size == array->used || array->frozen)
            return;

        SEC_LOGGER_TRACE("Shrinking array, this is expensive");
        SEC_ASSERT_REALLOC(array->internalArray, array->size, (array->size = array->used), "a shrunken array"); // TODO: Is this even needed?
    }
SEC_CPP_GUARD_END()
