#include <SharedEngineCode/Internal/CppSupport.h>
#include <stdlib.h>
#include <string.h>

#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Storage/DynamicArray.h"
#include "../EngineMemory.h"

SEC_CPP_SUPPORT_GUARD_START()
void ReallocateArray(BE_DynamicArray* array) {
    if (array->size != (size_t) array->used)
        return;

    SEC_LOGGER_TRACE("Ran out of free space, expanding array\nThis is expensive, so you should try avoiding it");

    array->size *= 2;
    array->calledRealloc++;

    BE_EngineMemory_ReallocateMemory(array->internalArray, sizeof(void*) * (array->size / 2), sizeof(void*) * array->size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
}

int BE_DynamicArray_Create(BE_DynamicArray* array, size_t size) {
    BE_STRICTMODE_CHECK(size != 0, 0, "Invalid size");

    array->internalArray = BE_EngineMemory_AllocateMemory(sizeof(void*) * size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
    array->used = 0;
    array->size = size;

    return 1;
}

int BE_DynamicArray_AddElementToStart(BE_DynamicArray* array, void* element) {
    if (array->frozen)
        return 0;

    ReallocateArray(array);
    (void) element; // TODO: Add element.

    return 0;
}

int BE_DynamicArray_AddElementToLast(BE_DynamicArray* array, void* element) {
    if (array->frozen)
        return 0;

    ReallocateArray(array);

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

    ReallocateArray(array);

    for (unsigned int windowId = index; (int) windowId < array->used; windowId++)
        array->internalArray[windowId] = array->internalArray[windowId + 1];

    array->used--;
    return 1;
}

void BE_DynamicArray_Shrink(BE_DynamicArray* array) {
    if (array->size == 0 || array->size == (size_t) array->used || array->frozen)
        return;

    unsigned oldSize = array->size;

    array->size = array->used;

    SEC_LOGGER_TRACE("Shrinking array, this is expensive");
    BE_EngineMemory_ReallocateMemory(array->internalArray, oldSize, array->size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
}
SEC_CPP_SUPPORT_GUARD_END()
