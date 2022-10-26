// Purpose: Stores a variable amount of keys and values.
// Created on: 8/15/22 @ 8:18 PM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <stddef.h>

#include "SharedEngineCode/Internal/CppSupport.h"
#include "DynamicArray.h"

SEC_CPP_SUPPORT_GUARD_START() // UNTESTED: I'm not sure any of this works.
typedef struct {
    BE_DynamicArray keys;
    BE_DynamicArray values;
    int frozen;
} BE_DynamicDictionary;

int BE_DynamicDictionary_Create(BE_DynamicDictionary* dictionary, size_t size);
int BE_DynamicDictionary_AddElementToStart(BE_DynamicDictionary* dictionary, void* key, void* value);
int BE_DynamicDictionary_AddElementToLast(BE_DynamicDictionary* dictionary, void* key, void* value);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
int BE_DynamicDictionary_RemoveFirstElement(BE_DynamicDictionary* dictionary, int shift);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
int BE_DynamicDictionary_RemoveLastElement(BE_DynamicDictionary* dictionary);

/**
    * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
    */
int BE_DynamicDictionary_RemoveElementAt(BE_DynamicDictionary* dictionary, unsigned int index);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
int BE_DynamicDictionary_RemoveElementViaKey(BE_DynamicDictionary* dictionary, void* key, size_t elementSize);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
int BE_DynamicDictionary_RemoveElementViaValue(BE_DynamicDictionary* dictionary, void* value, size_t elementSize);
int BE_DynamicDictionary_GetElementIndexFromKey(BE_DynamicDictionary dictionary, void* key, size_t elementSize);
int BE_DynamicDictionary_GetElementIndexFromValue(BE_DynamicDictionary dictionary, void* value, size_t elementSize);
void* BE_DynamicDictionary_GetElementKeyViaValue(BE_DynamicDictionary dictionary, void* value, size_t elementSize);
void* BE_DynamicDictionary_GetElementValueViaKey(BE_DynamicDictionary dictionary, void* key, size_t elementSize);
void BE_DynamicDictionary_GetElementsKeyViaValue(BE_DynamicDictionary dictionary, BE_DynamicDictionary* results, void* value, size_t elementSize);
void BE_DynamicDictionary_GetElementsValueViaKey(BE_DynamicDictionary dictionary, BE_DynamicDictionary* results, void* key, size_t elementSize);
void BE_DynamicDictionary_Shrink(BE_DynamicDictionary* dictionary);
SEC_CPP_SUPPORT_GUARD_END()

#define BE_DYNAMICDICTIONARY_GET_KEY(type, dictionary, value, elementSize) ((type*) BE_DynamicDictionary_GetElementIndexFromValue(&(dictionary), (value), (elementSize)))
#define BE_DYNAMICDICTIONARY_GET_KEY_PTR(type, dictionary, value, elementSize) ((type*) BE_DynamicDictionary_GetElementIndexFromValue((dictionary), (value), (elementSize)))
#define BE_DYNAMICDICTIONARY_GET_VALUE(type, dictionary, key, elementSize) ((type*) BE_DynamicDictionary_GetElementIndexFromKey(&(dictionary), (key), (elementSize)))
#define BE_DYNAMICDICTIONARY_GET_VALUE_PTR(type, dictionary, key, elementSize) ((type*) BE_DynamicDictionary_GetElementIndexFromKey((dictionary), (key), (elementSize)))
