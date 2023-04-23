// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>

#include "BaconEngine/Debugging/StrictMode.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Storage/DynamicDictionary.h"
#include "../InterfaceFunctions.h"

SEC_CPP_SUPPORT_GUARD_START()
#ifndef BE_CLIENT_BINARY
static SEC_Boolean BE_DynamicDictionary_UpdateFrozenState(BE_DynamicDictionary* dictionary) {
    dictionary->keys.frozen = dictionary->frozen;
    dictionary->values.frozen = dictionary->frozen;
    return dictionary->frozen;
}
#endif

SEC_Boolean BE_DynamicDictionary_Create(BE_DynamicDictionary* dictionary, size_t size) {
#ifndef BE_CLIENT_BINARY
    SEC_Boolean returnValue = BE_DynamicArray_Create(&dictionary->keys, size) && BE_DynamicArray_Create(&dictionary->values, size);

    dictionary->frozen = SEC_FALSE;
    return returnValue;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicDictionary*, size_t);
    return function(dictionary, size);
#endif
}

SEC_Boolean BE_DynamicDictionary_AddElementToStart(BE_DynamicDictionary* dictionary, void* key, void* value) {
#ifndef BE_CLIENT_BINARY
    if (BE_DynamicDictionary_UpdateFrozenState(dictionary))
        return SEC_FALSE;

    return BE_DynamicArray_AddElementToStart(&dictionary->keys, key) &&
           BE_DynamicArray_AddElementToStart(&dictionary->values, value);
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicDictionary*, void*, void*);
    return function(dictionary, key, value);
#endif
}

SEC_Boolean BE_DynamicDictionary_AddElementToLast(BE_DynamicDictionary* dictionary, void* key, void* value) {
#ifndef BE_CLIENT_BINARY
    if (BE_DynamicDictionary_UpdateFrozenState(dictionary))
        return SEC_FALSE;

    return BE_DynamicArray_AddElementToLast(&dictionary->keys, key) &&
           BE_DynamicArray_AddElementToLast(&dictionary->values, value);
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicDictionary* dictionary, void* key, void* value);
    return function(dictionary, key, value);
#endif
}

SEC_Boolean BE_DynamicDictionary_RemoveFirstElement(BE_DynamicDictionary* dictionary) {
#ifndef BE_CLIENT_BINARY
    if (BE_DynamicDictionary_UpdateFrozenState(dictionary))
        return SEC_FALSE;

    return BE_DynamicArray_RemoveFirstElement(&dictionary->keys) &&
           BE_DynamicArray_RemoveFirstElement(&dictionary->values);
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicDictionary*);
    return function(dictionary);
#endif
}

SEC_Boolean BE_DynamicDictionary_RemoveLastElement(BE_DynamicDictionary* dictionary) {
#ifndef BE_CLIENT_BINARY
    if (BE_DynamicDictionary_UpdateFrozenState(dictionary))
        return SEC_FALSE;

    return BE_DynamicArray_RemoveLastElement(&dictionary->keys) &&
           BE_DynamicArray_RemoveLastElement(&dictionary->values);
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicDictionary*);
    return function(dictionary);
#endif
}

SEC_Boolean BE_DynamicDictionary_RemoveElementAt(BE_DynamicDictionary* dictionary, unsigned index) {
#ifndef BE_CLIENT_BINARY
    if (BE_DynamicDictionary_UpdateFrozenState(dictionary))
        return SEC_FALSE;

    return BE_DynamicArray_RemoveElementAt(&dictionary->keys, index) &&
           BE_DynamicArray_RemoveElementAt(&dictionary->values, index);
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicDictionary*, unsigned);
    return function(dictionary, index);
#endif
}

SEC_Boolean BE_DynamicDictionary_RemoveElementViaKey(BE_DynamicDictionary* dictionary, void* key, size_t elementSize) {
#ifndef BE_CLIENT_BINARY
    if (BE_DynamicDictionary_UpdateFrozenState(dictionary))
        return SEC_FALSE;

    int index = BE_DynamicDictionary_GetElementIndexFromKey(*dictionary, key, elementSize);

    return index != -1 ? BE_DynamicDictionary_RemoveElementAt(dictionary, index) : SEC_FALSE;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicDictionary*, void*, size_t);
    return function(dictionary, key, elementSize);
#endif
}

SEC_Boolean BE_DynamicDictionary_RemoveElementViaValue(BE_DynamicDictionary* dictionary, void* value, size_t elementSize) {
#ifndef BE_CLIENT_BINARY
    if (BE_DynamicDictionary_UpdateFrozenState(dictionary))
        return SEC_FALSE;

    int index = BE_DynamicDictionary_GetElementIndexFromValue(*dictionary, value, elementSize);

    return index != -1 ? BE_DynamicDictionary_RemoveElementAt(dictionary, index) : SEC_FALSE;
#else
    BE_INTERFACEFUNCTION(SEC_Boolean, BE_DynamicDictionary*, void*, size_t);
    return function(dictionary, value, elementSize);
#endif
}

int BE_DynamicDictionary_GetElementIndexFromKey(BE_DynamicDictionary dictionary, void* key, size_t elementSize) {
#ifndef BE_CLIENT_BINARY
    for (int index = 0; index < dictionary.keys.used; index++) {
        if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], key, elementSize) != 0)
            continue;

        return index;
    }

    return -1;
#else
    BE_INTERFACEFUNCTION(int, BE_DynamicDictionary, void*, size_t);
    return function(dictionary, key, elementSize);
#endif
}

int BE_DynamicDictionary_GetElementIndexFromValue(BE_DynamicDictionary dictionary, void* value, size_t elementSize) {
#ifndef BE_CLIENT_BINARY
    for (int index = 0; index < dictionary.keys.used; index++) {
        if (dictionary.values.internalArray[index] == NULL || memcmp(dictionary.values.internalArray[index], value, elementSize) != 0)
            continue;

        return index;
    }

    return -1;
#else
    BE_INTERFACEFUNCTION(int, BE_DynamicDictionary, void*, size_t);
    return function(dictionary, value, elementSize);
#endif
}

void* BE_DynamicDictionary_GetElementKeyViaValue(BE_DynamicDictionary dictionary, void* value, size_t elementSize) {
#ifndef BE_CLIENT_BINARY
    int index = BE_DynamicDictionary_GetElementIndexFromValue(dictionary, value, elementSize);

    return index != -1 ? dictionary.keys.internalArray[index] : NULL;
#else
    BE_INTERFACEFUNCTION(void*, BE_DynamicDictionary, void*, size_t);
    return function(dictionary, value, elementSize);
#endif
}

void* BE_DynamicDictionary_GetElementValueViaKey(BE_DynamicDictionary dictionary, void* key, size_t elementSize) {
#ifndef BE_CLIENT_BINARY
    int index = BE_DynamicDictionary_GetElementIndexFromKey(dictionary, key, elementSize);

    return index != -1 ? dictionary.values.internalArray[index] : NULL;
#else
    BE_INTERFACEFUNCTION(void*, BE_DynamicDictionary, void*, size_t);
    return function(dictionary, key, elementSize);
#endif
}

void BE_DynamicDictionary_GetElementsValueViaKey(BE_DynamicDictionary dictionary, BE_DynamicDictionary* results, void* key, size_t elementSize) {
#ifndef BE_CLIENT_BINARY
    for (int index = 0; index < dictionary.keys.used; index++) {
        if (dictionary.keys.internalArray[index] == NULL || memcmp(dictionary.keys.internalArray[index], key, elementSize) != 0)
            continue;

        BE_DynamicDictionary_AddElementToLast(results, key, dictionary.values.internalArray[index]);
    }
#else
    BE_INTERFACEFUNCTION(void, BE_DynamicDictionary, BE_DynamicDictionary*, void*, size_t)(dictionary, results, key, elementSize);
#endif
}

void BE_DynamicDictionary_GetElementsKeyViaValue(BE_DynamicDictionary dictionary, BE_DynamicDictionary* results, void* value, size_t elementSize) {
#ifndef BE_CLIENT_BINARY
    for (int index = 0; index < dictionary.keys.used; index++) {
        if (dictionary.values.internalArray[index] == NULL || memcmp(dictionary.values.internalArray[index], value, elementSize) != 0)
            continue;

        BE_DynamicDictionary_AddElementToLast(results, dictionary.keys.internalArray[index], value);
    }
#else
    BE_INTERFACEFUNCTION(void, BE_DynamicDictionary, BE_DynamicDictionary*, void*, size_t)(dictionary, results, value, elementSize);
#endif
}
SEC_CPP_SUPPORT_GUARD_END()
