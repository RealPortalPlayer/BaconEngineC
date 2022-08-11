#include <SharedEngineCode/Internal/CppHeader.h>
#include <stdlib.h>
#include <string.h>

#include "BaconEngine/Storage/DynamicArray.h"
#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"

CPP_GUARD_START()
    void ReallocateArray(DynamicArray* array) {
        if (array->size != array->used)
            return;

        LOG_TRACE("Ran out of free space, expanding array. This is expensive");

        array->size *= 2;
        array->calledRealloc++;
        array->internalArray = realloc(array->internalArray, sizeof(void*) * array->size);

        ASSERT(array->internalArray != NULL, "Failed to reallocate %lu bytes of data for dynamic array", sizeof(void*) * array->size);
    }

    int CreateDynamicArray(DynamicArray* array, size_t size) {
        STRICT_CHECK(size != 0, 0, "Invalid size");

        array->internalArray = malloc(sizeof(void*) * size);
        array->used = 0;
        array->size = size;

        ASSERT(array->internalArray != NULL, "Failed to allocate %lu bytes of data for dynamic array", sizeof(void*) * size);

        return 1;
    }

    int ArrayAddElementToFirst(DynamicArray* array, void* element) {
        ReallocateArray(array);
        (void) element;

        return 0;
    }

    int ArrayAddElementToLast(DynamicArray* array, void* element) {
        ReallocateArray(array);

        array->internalArray[array->used++] = element;

        return 1;
    }

    int ArrayRemoveFirstElement(DynamicArray* array, int shift) {
        if (array->used == 0)
            return 0;

        if (!shift) {
            array->internalArray[0] = NULL;
            return 1;
        }

        return ArrayRemoveElementAt(array, 0);
    }

    int ArrayRemoveLastElement(DynamicArray* array) {
        if (array->used == 0)
            return 0;

        array->internalArray[--array->used] = NULL;

        return 1;
    }

    int ArrayRemoveElementAt(DynamicArray* array, unsigned int index) {
        if ((int) index >= array->used)
            return 0;

        ReallocateArray(array);

        for (unsigned int windowId = index; (int) windowId < array->used; windowId++)
            array->internalArray[windowId] = array->internalArray[windowId + 1];

        array->used--;
        return 1;
    }
CPP_GUARD_END()
