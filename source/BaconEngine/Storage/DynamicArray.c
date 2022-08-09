#include <SharedEngineCode/Internal/CppHeader.h>
#include <stdlib.h>

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

    int ArrayUnshiftElement(DynamicArray* array, void* element) {
        ReallocateArray(array);

        return 0; // TODO: Unshift
    }

    int ArrayPushElement(DynamicArray* array, void* element) {
        ReallocateArray(array);

        array->internalArray[array->used++] = element;

        return 1;
    }

    int ArrayShiftElement(DynamicArray* array, int shift) {
        if (array->used == 0)
            return 0;

        array->internalArray[0] = NULL;

        if (shift)
            return 0; // TODO: Shift

        return 1;
    }

    int ArrayPopElement(DynamicArray* array) {
        if (array->used == 0)
            return 0;

        array->internalArray[--array->used] = NULL;

        return 1;
    }
CPP_GUARD_END()
