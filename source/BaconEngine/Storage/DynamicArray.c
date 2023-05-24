// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdlib.h>

#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Storage/DynamicArray.h"
#include "BaconEngine/Debugging/Assert.h"
#include "../InterfaceFunctions.h"

#ifndef BE_CLIENT_BINARY
#   include "../EngineMemory.h"
#   include "PrivateDynamicArray.h"
#endif

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static void BE_DynamicArray_ReallocateArray(BE_DynamicArray* array) {
    if (array->size != (size_t) array->used)
        return;

    SEC_LOGGER_TRACE("Ran out of free space, expanding array\nThis is expensive, so you should try avoiding it\n");
    BE_ASSERT((int) array->size >= array->used, "Invalid array state\n");

    array->size *= 2;
    array->calledRealloc++;

    if (array->engineMade)
        array->internalArray = BE_EngineMemory_ReallocateMemory(array->internalArray, sizeof(void*) * (array->size / 2), sizeof(void*) * array->size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
    else
        BE_ASSERT((array->internalArray = (void**) realloc(array->internalArray, sizeof(void*) * array->size)) != NULL, "Failed to reallocate %zu bytes of data\n", sizeof(void*) * array->size);
}
#endif

// TODO: Make a public one that does not use engine memory.
SEC_Boolean BE_DynamicArray_Create(BE_DynamicArray* array, size_t size) {
#ifndef BE_CLIENT_BINARY
    BE_STRICTMODE_CHECK(size != 0, SEC_BOOLEAN_FALSE, "Invalid size\n");
    BE_ASSERT((array->internalArray = (void**) malloc(sizeof(void*) * size)) != NULL, "Failed to allocate %zu bytes of data\n", sizeof(void*) * size);

    array->used = 0;
    array->size = size;
    array->frozen = SEC_BOOLEAN_FALSE;
    array->engineMade = SEC_BOOLEAN_FALSE;
    return SEC_BOOLEAN_TRUE;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicArray*, size_t);
    return function(array, size);
#endif
}

#ifndef BE_CLIENT_BINARY
SEC_Boolean BE_PrivateDynamicArray_Create(BE_DynamicArray* array, size_t size) {
    BE_ASSERT(size != 0, "Invalid size\n");

    array->internalArray = (void**) BE_EngineMemory_AllocateMemory(sizeof(void*) * size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
    array->used = 0;
    array->size = size;
    array->frozen = SEC_BOOLEAN_FALSE;
    array->engineMade = SEC_BOOLEAN_TRUE;
    return SEC_BOOLEAN_TRUE;
}
#endif

SEC_Boolean BE_DynamicArray_AddElementToStart(BE_DynamicArray* array, void* element) {
#ifndef BE_CLIENT_BINARY
    if (array->frozen)
        return SEC_BOOLEAN_FALSE;

    BE_DynamicArray_ReallocateArray(array);

    for (int id = array->used; id >= 0; id--)
        array->internalArray[id + 1] = array->internalArray[id];

    array->internalArray[0] = element;
    array->used++;
    return SEC_BOOLEAN_TRUE;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicArray*, void*);
    return function(array, element);
#endif
}

SEC_Boolean BE_DynamicArray_AddElementToLast(BE_DynamicArray* array, void* element) {
#ifndef BE_CLIENT_BINARY
    if (array->frozen)
        return SEC_BOOLEAN_FALSE;

    BE_DynamicArray_ReallocateArray(array);

    array->internalArray[array->used++] = element;
    return SEC_BOOLEAN_TRUE;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicArray*, void*);
    return function(array, element);
#endif
}

SEC_Boolean BE_DynamicArray_RemoveFirstElement(BE_DynamicArray* array) {
#ifndef BE_CLIENT_BINARY
    return array->used != 0 && !array->frozen ? BE_DynamicArray_RemoveElementAt(array, 0) : SEC_BOOLEAN_FALSE;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicArray*);
    return function(array);
#endif
}

SEC_Boolean BE_DynamicArray_RemoveLastElement(BE_DynamicArray* array) {
#ifndef BE_CLIENT_BINARY
    if (array->used == 0 || array->frozen)
        return SEC_BOOLEAN_FALSE;

    array->internalArray[--array->used] = NULL;
    return SEC_BOOLEAN_TRUE;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicArray*);
    return function(array);
#endif
}

SEC_Boolean BE_DynamicArray_RemoveElementAt(BE_DynamicArray* array, unsigned index) {
#ifndef BE_CLIENT_BINARY
    if ((int) index >= array->used || array->frozen)
        return SEC_BOOLEAN_FALSE;

    BE_DynamicArray_ReallocateArray(array);

    for (unsigned int id = index; (int) id < array->used; id++)
        array->internalArray[id] = array->internalArray[id + 1];

    array->used--;
    return SEC_BOOLEAN_TRUE;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicArray*, unsigned);
    return function(array, index);
#endif
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
