// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>

#include "SharedEngineCode/Storage/DynamicDictionary.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
static SEC_Boolean SEC_DynamicDictionary_UpdateFrozenState(SEC_DynamicDictionary* dictionary) {
    dictionary->keys.frozen = dictionary->frozen;
    dictionary->values.frozen = dictionary->frozen;
    return dictionary->frozen;
}

SEC_Boolean SEC_DynamicDictionary_Create(SEC_DynamicDictionary* dictionary, size_t size) {
    SEC_Boolean returnValue = SEC_DynamicArray_Create(&dictionary->keys, size) &&
                              SEC_DynamicArray_Create(&dictionary->values, size);

    dictionary->frozen = SEC_BOOLEAN_FALSE;
    return returnValue;
}

SEC_Boolean SEC_DynamicDictionary_AddElementToStart(SEC_DynamicDictionary* dictionary, void* key, void* value) {
    if (SEC_DynamicDictionary_UpdateFrozenState(dictionary))
        return SEC_BOOLEAN_FALSE;

    return SEC_DynamicArray_AddElementToStart(&dictionary->keys, key) &&
           SEC_DynamicArray_AddElementToStart(&dictionary->values, value);
}

SEC_Boolean SEC_DynamicDictionary_AddElementToLast(SEC_DynamicDictionary* dictionary, void* key, void* value) {
    if (SEC_DynamicDictionary_UpdateFrozenState(dictionary))
        return SEC_BOOLEAN_FALSE;

    return SEC_DynamicArray_AddElementToLast(&dictionary->keys, key) &&
           SEC_DynamicArray_AddElementToLast(&dictionary->values, value);
}

SEC_Boolean SEC_DynamicDictionary_RemoveFirstElement(SEC_DynamicDictionary* dictionary) {
    if (SEC_DynamicDictionary_UpdateFrozenState(dictionary))
        return SEC_BOOLEAN_FALSE;

    return SEC_DynamicArray_RemoveFirstElement(&dictionary->keys) &&
           SEC_DynamicArray_RemoveFirstElement(&dictionary->values);
}

SEC_Boolean SEC_DynamicDictionary_RemoveLastElement(SEC_DynamicDictionary* dictionary) {
    if (SEC_DynamicDictionary_UpdateFrozenState(dictionary))
        return SEC_BOOLEAN_FALSE;

    return SEC_DynamicArray_RemoveLastElement(&dictionary->keys) &&
           SEC_DynamicArray_RemoveLastElement(&dictionary->values);
}

SEC_Boolean SEC_DynamicDictionary_RemoveElementAt(SEC_DynamicDictionary* dictionary, unsigned index) {
    if (SEC_DynamicDictionary_UpdateFrozenState(dictionary))
        return SEC_BOOLEAN_FALSE;

    return SEC_DynamicArray_RemoveElementAt(&dictionary->keys, index) &&
           SEC_DynamicArray_RemoveElementAt(&dictionary->values, index);
}

SEC_Boolean SEC_DynamicDictionary_RemoveElementViaKey(SEC_DynamicDictionary* dictionary, void* key, size_t elementSize) {
    if (SEC_DynamicDictionary_UpdateFrozenState(dictionary))
        return SEC_BOOLEAN_FALSE;

    int index = SEC_DynamicDictionary_GetElementIndexFromKey(*dictionary, key, elementSize);

    return index != -1 ? SEC_DynamicDictionary_RemoveElementAt(dictionary, index) : SEC_BOOLEAN_FALSE;
}

SEC_Boolean SEC_DynamicDictionary_RemoveElementViaValue(SEC_DynamicDictionary* dictionary, void* value, size_t elementSize) {
    if (SEC_DynamicDictionary_UpdateFrozenState(dictionary))
        return SEC_BOOLEAN_FALSE;

    int index = SEC_DynamicDictionary_GetElementIndexFromValue(*dictionary, value, elementSize);

    return index != -1 ? SEC_DynamicDictionary_RemoveElementAt(dictionary, index) : SEC_BOOLEAN_FALSE;
}

int SEC_DynamicDictionary_GetElementIndexFromKey(SEC_DynamicDictionary dictionary, void* key, size_t elementSize) {
    for (int index = 0; index < dictionary.keys.used; index++) {
        if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], key, elementSize) != 0)
            continue;

        return index;
    }

    return -1;
}

int SEC_DynamicDictionary_GetElementIndexFromValue(SEC_DynamicDictionary dictionary, void* value, size_t elementSize) {
    for (int index = 0; index < dictionary.keys.used; index++) {
        if (dictionary.values.internalArray[index] == NULL || memcmp(dictionary.values.internalArray[index], value, elementSize) != 0)
            continue;

        return index;
    }

    return -1;
}

void* SEC_DynamicDictionary_GetElementKeyViaValue(SEC_DynamicDictionary dictionary, void* value, size_t elementSize) {
    int index = SEC_DynamicDictionary_GetElementIndexFromValue(dictionary, value, elementSize);

    return index != -1 ? dictionary.keys.internalArray[index] : NULL;
}

void* SEC_DynamicDictionary_GetElementValueViaKey(SEC_DynamicDictionary dictionary, void* key, size_t elementSize) {
    int index = SEC_DynamicDictionary_GetElementIndexFromKey(dictionary, key, elementSize);

    return index != -1 ? dictionary.values.internalArray[index] : NULL;
}

void SEC_DynamicDictionary_GetElementsValueViaKey(SEC_DynamicDictionary dictionary, SEC_DynamicDictionary* results, void* key, size_t elementSize) {
    for (int index = 0; index < dictionary.keys.used; index++) {
        if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], key, elementSize) != 0)
            continue;

        SEC_DynamicDictionary_AddElementToLast(results, key, dictionary.values.internalArray[index]);
    }
}

void SEC_DynamicDictionary_GetElementsKeyViaValue(SEC_DynamicDictionary dictionary, SEC_DynamicDictionary* results, void* value, size_t elementSize) {
    for (int index = 0; index < dictionary.keys.used; index++) {
        if (dictionary.values.internalArray[index] == NULL || memcmp(dictionary.values.internalArray[index], value, elementSize) != 0)
            continue;

        SEC_DynamicDictionary_AddElementToLast(results, dictionary.keys.internalArray[index], value);
    }
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
