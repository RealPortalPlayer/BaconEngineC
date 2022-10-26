// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdlib.h>
#include <string.h>

#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Storage/DynamicArray.h"
#include "../EngineMemory.h"
#include "BaconEngine/Debugging/Assert.h"

SEC_CPP_SUPPORT_GUARD_START()
void BE_DynamicArray_ReallocateArray(BE_DynamicArray* array) {
    if (array->size != (size_t) array->used)
        return;

    SEC_LOGGER_TRACE("Ran out of free space, expanding array\nThis is expensive, so you should try avoiding it\n");
    BE_ASSERT(array->size >= array->used, "Invalid array state\n");

    array->size *= 2;
    array->calledRealloc++;

    BE_EngineMemory_ReallocateMemory(array->internalArray, sizeof(void*) * (array->size / 2), sizeof(void*) * array->size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
}

// TODO: Make a public one that does not use engine memory.
int BE_DynamicArray_Create(BE_DynamicArray* array, size_t size) {
    BE_STRICTMODE_CHECK(size != 0, 0, "Invalid size\n");

    array->internalArray = (void**) BE_EngineMemory_AllocateMemory(sizeof(void*) * size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
    array->used = 0;
    array->size = size;
    array->frozen = 0;
    return 1;
}

int BE_DynamicArray_AddElementToStart(BE_DynamicArray* array, void* element) {
    if (array->frozen)
        return 0;

    BE_DynamicArray_ReallocateArray(array);

    for (int id = array->used - 1; id > 0; id--)
        array->internalArray[id] = array->internalArray[id - 1];

    array->internalArray[0] = element;
    array->used++;
    return 1;
}

int BE_DynamicArray_AddElementToLast(BE_DynamicArray* array, void* element) {
    if (array->frozen)
        return 0;

    BE_DynamicArray_ReallocateArray(array);

    array->internalArray[array->used++] = element;

    return 1;
}

int BE_DynamicArray_RemoveFirstElement(BE_DynamicArray* array, int shift) {
    if (array->used == 0 || array->frozen)
        return 0;

    if (!shift) {
        array->internalArray[0] = NULL;
        return 1;
    }

    return BE_DynamicArray_RemoveElementAt(array, 0);
}

int BE_DynamicArray_RemoveLastElement(BE_DynamicArray* array) {
    if (array->used == 0 || array->frozen)
        return 0;

    array->internalArray[--array->used] = NULL;
    return 1;
}

int BE_DynamicArray_RemoveElementAt(BE_DynamicArray* array, unsigned int index) {
    if ((int) index >= array->used || array->frozen)
        return 0;

    BE_DynamicArray_ReallocateArray(array);

    for (unsigned int id = index; (int) id < array->used; id++)
        array->internalArray[id] = array->internalArray[id + 1];

    array->used--;
    return 1;
}

void BE_DynamicArray_Shrink(BE_DynamicArray* array) {
    if (array->size == 0 || array->size == (size_t) array->used || array->frozen)
        return;

    unsigned oldSize = array->size;

    array->size = array->used;

    SEC_LOGGER_TRACE("Shrinking array, this is expensive\n");
    BE_EngineMemory_ReallocateMemory(array->internalArray, oldSize, array->size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
}
SEC_CPP_SUPPORT_GUARD_END()
