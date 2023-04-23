// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdlib.h>

#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Storage/DynamicArray.h"
#include "../EngineMemory.h"
#include "BaconEngine/Debugging/Assert.h"
#include "../InterfaceFunctions.h"

SEC_CPP_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static void BE_DynamicArray_ReallocateArray(BE_DynamicArray* array) {
    if (array->size != (size_t) array->used)
        return;

    SEC_LOGGER_TRACE("Ran out of free space, expanding array\nThis is expensive, so you should try avoiding it\n");
    BE_ASSERT((int) array->size >= array->used, "Invalid array state\n");

    array->size *= 2;
    array->calledRealloc++;

    BE_EngineMemory_ReallocateMemory(array->internalArray, sizeof(void*) * (array->size / 2), sizeof(void*) * array->size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
}
#endif

// TODO: Make a public one that does not use engine memory.
SEC_Boolean BE_DynamicArray_Create(BE_DynamicArray* array, size_t size) {
#ifndef BE_CLIENT_BINARY
    BE_STRICTMODE_CHECK(size != 0, SEC_FALSE, "Invalid size\n");

    array->internalArray = (void**) BE_EngineMemory_AllocateMemory(sizeof(void*) * size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
    array->used = 0;
    array->size = size;
    array->frozen = SEC_FALSE;
    return SEC_TRUE;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicArray*, size_t);
    return function(array, size);
#endif
}

SEC_Boolean BE_DynamicArray_AddElementToStart(BE_DynamicArray* array, void* element) {
#ifndef BE_CLIENT_BINARY
    if (array->frozen)
        return SEC_FALSE;

    BE_DynamicArray_ReallocateArray(array);

    for (int id = array->used; id >= 0; id--)
        array->internalArray[id + 1] = array->internalArray[id];

    array->internalArray[0] = element;
    array->used++;
    return SEC_TRUE;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicArray*, void*);
    return function(array, element);
#endif
}

SEC_Boolean BE_DynamicArray_AddElementToLast(BE_DynamicArray* array, void* element) {
#ifndef BE_CLIENT_BINARY
    if (array->frozen)
        return SEC_FALSE;

    BE_DynamicArray_ReallocateArray(array);

    array->internalArray[array->used++] = element;

    return SEC_TRUE;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicArray*, void*);
    return function(array, element);
#endif
}

SEC_Boolean BE_DynamicArray_RemoveFirstElement(BE_DynamicArray* array) {
#ifndef BE_CLIENT_BINARY
    return array->used != 0 && !array->frozen ? BE_DynamicArray_RemoveElementAt(array, 0) : SEC_FALSE;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicArray*);
    return function(array);
#endif
}

SEC_Boolean BE_DynamicArray_RemoveLastElement(BE_DynamicArray* array) {
#ifndef BE_CLIENT_BINARY
    if (array->used == 0 || array->frozen)
        return SEC_FALSE;

    array->internalArray[--array->used] = NULL;
    return SEC_TRUE;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicArray*);
    return function(array);
#endif
}

SEC_Boolean BE_DynamicArray_RemoveElementAt(BE_DynamicArray* array, unsigned index) {
#ifndef BE_CLIENT_BINARY
    if ((int) index >= array->used || array->frozen)
        return SEC_FALSE;

    BE_DynamicArray_ReallocateArray(array);

    for (unsigned int id = index; (int) id < array->used; id++)
        array->internalArray[id] = array->internalArray[id + 1];

    array->used--;
    return SEC_TRUE;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicArray*, unsigned);
    return function(array, index);
#endif
}
SEC_CPP_SUPPORT_GUARD_END()
