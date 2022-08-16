#include <SharedEngineCode/Internal/CppHeader.h>
#include <stdlib.h>
#include <string.h>

#include "BaconEngine/Storage/DynamicArray.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"

CPP_GUARD_START()
    void ReallocateArray(BE_DynamicArray* array) {
        if (array->size != array->used)
            return;

        SEC_LOG_TRACE("Ran out of free space, expanding array\nThis is expensive, so you should try avoiding it");

        array->size *= 2;
        array->calledRealloc++;

        BE_ASSERT_REALLOC(array->internalArray, sizeof(void*) * array->size, "a dynamic array");
    }

    int BE_CreateDynamicArray(BE_DynamicArray* array, size_t size) {
        BE_STRICT_CHECK(size != 0, 0, "Invalid size");
        BE_ASSERT_MALLOC(array->internalArray, sizeof(void*) * size, "a dynamic array");

        array->used = 0;
        array->size = size;

        return 1;
    }

    int BE_ArrayAddElementToFirst(BE_DynamicArray* array, void* element) {
        ReallocateArray(array);
        (void) element;

        return 0;
    }

    int BE_ArrayAddElementToLast(BE_DynamicArray* array, void* element) {
        ReallocateArray(array);

        array->internalArray[array->used++] = element;

        return 1;
    }

    int BE_ArrayRemoveFirstElement(BE_DynamicArray* array, int shift) {
        if (array->used == 0)
            return 0;

        if (!shift) {
            array->internalArray[0] = NULL;
            return 1;
        }

        return BE_ArrayRemoveElementAt(array, 0);
    }

    int BE_ArrayRemoveLastElement(BE_DynamicArray* array) {
        if (array->used == 0)
            return 0;

        array->internalArray[--array->used] = NULL;

        return 1;
    }

    int BE_ArrayRemoveElementAt(BE_DynamicArray* array, unsigned int index) {
        if ((int) index >= array->used)
            return 0;

        ReallocateArray(array);

        for (unsigned int windowId = index; (int) windowId < array->used; windowId++)
            array->internalArray[windowId] = array->internalArray[windowId + 1];

        array->used--;
        return 1;
    }
CPP_GUARD_END()
