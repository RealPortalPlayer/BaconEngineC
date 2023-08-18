// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/Debugging/Assert.h>

#include "PrivateDynamicArray.h"
#include "../EngineMemory.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_Boolean BE_PrivateDynamicArray_Create(SEC_DynamicArray* array, size_t size) {
    if (size <= 0)
        return SEC_BOOLEAN_FALSE;

    array->internalArray = (void**) BE_EngineMemory_AllocateMemory(sizeof(void*) * size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);
    array->used = 0;
    array->size = size;
    array->frozen = SEC_BOOLEAN_FALSE;
    return SEC_BOOLEAN_TRUE;
}

SEC_Boolean BE_PrivateDynamicArray_CheckResize(SEC_DynamicArray* array) {
    if (array->size != (size_t) array->used)
        return SEC_BOOLEAN_TRUE;

    SEC_LOGGER_TRACE("Ran out of free space, expanding array\nThis is expensive, so you should try avoiding it\n");
    SEC_ASSERT((int) array->size >= array->used, "Invalid array state\n");

    array->size *= 2;
    array->calledReallocate++;
    
    void** newArray = (void**) BE_EngineMemory_ReallocateMemory(array->internalArray, sizeof(void*) * (array->size / 2), sizeof(void*) * array->size, BE_ENGINEMEMORY_MEMORY_TYPE_DYNAMIC_ARRAY);

    if (newArray == NULL)
        return SEC_BOOLEAN_FALSE;
    
    array->internalArray = newArray;
    return SEC_BOOLEAN_TRUE;
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
