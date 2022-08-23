// Purpose: Stores a variable amount of keys and values.
// Created on: 8/15/22 @ 8:18 PM

#pragma once

#include "SharedEngineCode/Internal/CppHeader.h"
#include <stddef.h>

#include "DynamicArray.h"

CPP_GUARD_START()
    typedef struct {
        SEC_DynamicArray keys;
        SEC_DynamicArray values;
    } SEC_DynamicDictionary;

    int SEC_CreateDynamicDictionary(SEC_DynamicDictionary* dictionary, size_t size);
    int SEC_DictionaryAddElementToFirst(SEC_DynamicDictionary* dictionary, void* key, void* value);
    int SEC_DictionaryAddElementToLast(SEC_DynamicDictionary* dictionary, void* key, void* value);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int SEC_DictionaryRemoveFirstElement(SEC_DynamicDictionary* dictionary, int shift);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int BE_DictionaryRemoveLastElement(SEC_DynamicDictionary* dictionary);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int SEC_DictionaryRemoveElementAt(SEC_DynamicDictionary* dictionary, unsigned int index);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int SEC_DictionaryRemoveElementViaKey(SEC_DynamicDictionary* dictionary, void* key, size_t elementSize);

    /**
     * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
     */
    int SEC_DictionaryRemoveElementViaValue(SEC_DynamicDictionary* dictionary, void* value, size_t elementSize);
    int SEC_GetElementIndexFromKey(SEC_DynamicDictionary dictionary, void* key, size_t elementSize);
    int SEC_GetElementIndexFromValue(SEC_DynamicDictionary dictionary, void* value, size_t elementSize);
    void* SEC_GetElementViaValue(SEC_DynamicDictionary dictionary, void* key, size_t elementSize);
    void* SEC_GetElementViaKey(SEC_DynamicDictionary dictionary, void* value, size_t elementSize);
    void SEC_GetElementsViaValue(SEC_DynamicDictionary dictionary, SEC_DynamicDictionary* results, void* value, size_t elementSize);
    void SEC_GetElementsViaKey(SEC_DynamicDictionary dictionary, SEC_DynamicDictionary* results, void* key, size_t elementSize);
CPP_GUARD_END()

#define BE_DICTIONARY_GET_KEY(type, dictionary, value, elementSize) ((type*) BE_GetElementIndexFromValue(&(dictionary), (value), (elementSize)))
#define BE_DICTIONARY_GET_KEY_PTR(type, dictionary, value, elementSize) ((type*) BE_GetElementIndexFromValue((dictionary), (value), (elementSize)))
#define BE_DICTIONARY_GET_VALUE(type, dictionary, key, elementSize) ((type*) BE_GetElementIndexFromKey(&(dictionary), (key), (elementSize)))
#define BE_DICTIONARY_GET_VALUE_PTR(type, dictionary, key, elementSize) ((type*) BE_GetElementIndexFromKey((dictionary), (key), (elementSize)))
