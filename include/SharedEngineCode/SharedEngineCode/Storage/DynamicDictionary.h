// Purpose: Stores a variable amount of keys and values.
// Created on: 8/15/22 @ 8:18 PM

#pragma once

#include "SharedEngineCode/Internal/CppHeader.h"
#include <stddef.h>

#include "DynamicArray.h"

SEC_CPP_GUARD_START() // UNTESTED: I'm not sure any of this works.
    typedef struct {
        SEC_DynamicArray keys;
        SEC_DynamicArray values;
        int frozen;
    } SEC_DynamicDictionary;

    int SEC_DynamicDictionary_Create(SEC_DynamicDictionary* dictionary, size_t size);
    int SEC_DynamicDictionary_AddElementToStart(SEC_DynamicDictionary* dictionary, void* key, void* value);
    int SEC_DynamicDictionary_AddElementToLast(SEC_DynamicDictionary* dictionary, void* key, void* value);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int SEC_DynamicDictionary_RemoveFirstElement(SEC_DynamicDictionary* dictionary, int shift);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int BE_DynamicDictionary_RemoveLastElement(SEC_DynamicDictionary* dictionary);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int SEC_DynamicDictionary_RemoveElementAt(SEC_DynamicDictionary* dictionary, unsigned int index);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int SEC_DynamicDictionary_RemoveElementViaKey(SEC_DynamicDictionary* dictionary, void* key, size_t elementSize);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int SEC_DynamicDictionary_RemoveElementViaValue(SEC_DynamicDictionary* dictionary, void* value, size_t elementSize);
    int SEC_DynamicDictionary_GetElementIndexFromKey(SEC_DynamicDictionary dictionary, void* key, size_t elementSize);
    int SEC_DynamicDictionary_GetElementIndexFromValue(SEC_DynamicDictionary dictionary, void* value, size_t elementSize);
    void* SEC_DynamicDictionary_GetElementViaValue(SEC_DynamicDictionary dictionary, void* key, size_t elementSize);
    void* SEC_DynamicDictionary_GetElementViaKey(SEC_DynamicDictionary dictionary, void* value, size_t elementSize);
    void SEC_DynamicDictionary_GetElementsViaValue(SEC_DynamicDictionary dictionary, SEC_DynamicDictionary* results, void* value, size_t elementSize);
    void SEC_DynamicDictionary_GetElementsViaKey(SEC_DynamicDictionary dictionary, SEC_DynamicDictionary* results, void* key, size_t elementSize);
    void SEC_DynamicDictionary_Shrink(SEC_DynamicDictionary* dictionary);
SEC_CPP_GUARD_END()

#define BE_DYNAMICDICTIONARY_GET_KEY(type, dictionary, value, elementSize) ((type*) SEC_DynamicDictionary_GetElementIndexFromValue(&(dictionary), (value), (elementSize)))
#define BE_DYNAMICDICTIONARY_GET_KEY_PTR(type, dictionary, value, elementSize) ((type*) SEC_DynamicDictionary_GetElementIndexFromValue((dictionary), (value), (elementSize)))
#define BE_DYNAMICDICTIONARY_GET_VALUE(type, dictionary, key, elementSize) ((type*) SEC_DynamicDictionary_GetElementIndexFromKey(&(dictionary), (key), (elementSize)))
#define BE_DYNAMICDICTIONARY_GET_VALUE_PTR(type, dictionary, key, elementSize) ((type*) SEC_DynamicDictionary_GetElementIndexFromKey((dictionary), (key), (elementSize)))
