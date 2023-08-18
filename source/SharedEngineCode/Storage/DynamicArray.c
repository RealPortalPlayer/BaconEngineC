// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdlib.h>
#include <string.h>

#include "SharedEngineCode/Storage/DynamicArray.h"
#include "SharedEngineCode/Logger.h"
#include "SharedEngineCode/Debugging/Assert.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
static SEC_Boolean SEC_DynamicArray_ReallocateArray(SEC_DynamicArray* array) {
    if (array->size != (size_t) array->used)
        return SEC_BOOLEAN_TRUE;

    SEC_LOGGER_TRACE("Ran out of free space, expanding array\nThis is expensive, so you should try avoiding it\n");
    SEC_ASSERT((int) array->size >= array->used, "Invalid array state\n");

    array->size *= 2;
    array->calledReallocate++;
    
    void** newArray = (void**) realloc(array->internalArray, sizeof(void*) * array->size);

    if (newArray == NULL)
        return SEC_BOOLEAN_FALSE;
    
    array->internalArray = newArray;
    return SEC_BOOLEAN_TRUE;
}

int SEC_DynamicArray_GetIndexForElement(SEC_DynamicArray* array, void* element, size_t elementSize) {
    for (int i = 0; i < array->used; i++) {
        if (memcmp(array->internalArray[i], element, elementSize) != 0)
            continue;
        
        return i;
    }

    return -1;
}

SEC_Boolean SEC_DynamicArray_Create(SEC_DynamicArray* array, size_t size) {
    if (size <= 0)
        return SEC_BOOLEAN_FALSE;
    
    array->internalArray = malloc(sizeof(void*) * size);

    if (array->internalArray == NULL)
        return SEC_BOOLEAN_FALSE;

    array->used = 0;
    array->size = size;
    array->frozen = SEC_BOOLEAN_FALSE;
    return SEC_BOOLEAN_TRUE;
}

SEC_Boolean SEC_DynamicArray_AddElementToStart(SEC_DynamicArray* array, void* element) {
    if (array->frozen)
        return SEC_BOOLEAN_FALSE;

    SEC_DynamicArray_ReallocateArray(array);

    for (int id = array->used; id >= 0; id--)
        array->internalArray[id + 1] = array->internalArray[id];

    array->internalArray[0] = element;
    array->used++;
    return SEC_BOOLEAN_TRUE;
}

SEC_Boolean SEC_DynamicArray_AddElementToLast(SEC_DynamicArray* array, void* element) {
    if (array->frozen)
        return SEC_BOOLEAN_FALSE;

    SEC_DynamicArray_ReallocateArray(array);

    array->internalArray[array->used++] = element;
    return SEC_BOOLEAN_TRUE;
}

SEC_Boolean SEC_DynamicArray_RemoveFirstElement(SEC_DynamicArray* array) {
    return array->used != 0 && !array->frozen ? SEC_DynamicArray_RemoveElementAt(array, 0) : SEC_BOOLEAN_FALSE;
}

SEC_Boolean SEC_DynamicArray_RemoveLastElement(SEC_DynamicArray* array) {
    if (array->used == 0 || array->frozen)
        return SEC_BOOLEAN_FALSE;

    array->internalArray[--array->used] = NULL;
    return SEC_BOOLEAN_TRUE;
}

SEC_Boolean SEC_DynamicArray_RemoveElementAt(SEC_DynamicArray* array, unsigned index) {
    if ((int) index >= array->used || array->frozen)
        return SEC_BOOLEAN_FALSE;

    SEC_DynamicArray_ReallocateArray(array);

    for (unsigned int id = index; (int) id < array->used; id++)
        array->internalArray[id] = array->internalArray[id + 1];

    array->used--;
    return SEC_BOOLEAN_TRUE;
}

SEC_Boolean SEC_DynamicArray_RemoveMatchedElement(SEC_DynamicArray* array, void* element, size_t elementSize, SEC_Boolean repeat) {
    SEC_Boolean removedOne = SEC_BOOLEAN_FALSE;

    for (int i = 0; i < array->used; i++) {
        int elementId = SEC_DynamicArray_GetIndexForElement(array, element, elementSize);

        if (elementId == -1)
            continue;

        SEC_DynamicArray_RemoveElementAt(array, i);

        if (repeat)
            continue;

        return SEC_BOOLEAN_TRUE;
    }

    return removedOne;
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
