// Purpose: Stores map information.
// Created on: 8/6/22 @ 7:22 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>
#include <stddef.h>

#include "ClientObject.h"

CPP_GUARD_START()
    typedef struct {
        ClientObject* array;
        size_t used;
        size_t size;
    } ClientObjectArray;

    typedef struct {
        const char* name;
        ClientObjectArray clientObjects;
    } Scene;

    void CreateScene(Scene* scene, const char* name);
    void LoadScene(Scene* scene, const char* name);
    void InsertClientObject(Scene* scene, ClientObject clientObject);
    int DeleteClientObject(Scene* scene, const char* name, int multi);
CPP_GUARD_END()
