// Purpose: Stores a variable amount of keys and values.
// Created on: 8/15/22 @ 8:18 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>
#include <stddef.h>

#include "DynamicArray.h"

CPP_GUARD_START()
    typedef struct {
        BE_DynamicArray keys;
        BE_DynamicArray values;
    } BE_DynamicDictionary;

    int BE_CreateDynamicDictionary(BE_DynamicDictionary* dictionary, size_t size);
    int BE_DictionaryAddElementToFirst(BE_DynamicDictionary* dictionary, void* key, void* value);
    int BE_DictionaryAddElementToLast(BE_DynamicDictionary* dictionary, void* key, void* value);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int BE_DictionaryRemoveFirstElement(BE_DynamicDictionary* dictionary, int shift);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int BE_DictionaryRemoveLastElement(BE_DynamicDictionary* dictionary);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int BE_DictionaryRemoveElementAt(BE_DynamicDictionary* dictionary, unsigned int index);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int BE_DictionaryRemoveElementViaKey(BE_DynamicDictionary* dictionary, void* key, size_t elementSize);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int BE_DictionaryRemoveElementViaValue(BE_DynamicDictionary* dictionary, void* value, size_t elementSize);
    int BE_GetElementIndexFromKey(BE_DynamicDictionary dictionary, void* key, size_t elementSize);
    int BE_GetElementIndexFromValue(BE_DynamicDictionary dictionary, void* value, size_t elementSize);
    void* BE_GetElementViaValue(BE_DynamicDictionary dictionary, void* key, size_t elementSize);
    void* BE_GetElementViaKey(BE_DynamicDictionary dictionary, void* value, size_t elementSize);
    void BE_GetElementsViaValue(BE_DynamicDictionary dictionary, BE_DynamicDictionary* result, void* key, size_t elementSize);
    void BE_GetElementsViaKey(BE_DynamicDictionary dictionary, BE_DynamicDictionary* result, void* value, size_t elementSize);
CPP_GUARD_END()

#define BE_DICTIONARY_GET_KEY(type, dictionary, value, elementSize) ((type*) BE_GetElementIndexFromValue(&(dictionary), (value), (elementSize)))
#define BE_DICTIONARY_GET_KEY_PTR(type, dictionary, value, elementSize) ((type*) BE_GetElementIndexFromValue((dictionary), (value), (elementSize)))
#define BE_DICTIONARY_GET_VALUE(type, dictionary, key, elementSize) ((type*) BE_GetElementIndexFromKey(&(dictionary), (key), (elementSize)))
#define BE_DICTIONARY_GET_VALUE_PTR(type, dictionary, key, elementSize) ((type*) BE_GetElementIndexFromKey((dictionary), (key), (elementSize)))
