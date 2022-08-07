#include <stdlib.h>
#include <SharedEngineCode/Logger.h>
#include <string.h>
#include <BaconEngine/Debugging/Assert.h>

#include "BaconEngine/SceneLogic/Scene.h"

CPP_GUARD_START()
    void CreateScene(Scene* scene, const char* name) {
        ClientObject* clientObjectArray = malloc(sizeof(ClientObject) * 100);

        ASSERT(clientObjectArray != NULL, "Failed to allocate %lu bytes of data for the array of client objects",
               sizeof(ClientObject) * 100);

        scene->name = name;
        scene->clientObjects = (ClientObjectArray) {
                .array = clientObjectArray,
                .used = 0,
                .size = 100
        };
    }

    void LoadScene(Scene* scene, const char* name) { // TODO: Load scene from client archive.
        (void) scene;
        (void) name;

        ASSERT(0, "This function has not been implemented, yet");
    }

    void InsertClientObject(Scene* scene, ClientObject clientObject) {
        if (scene->clientObjects.used == scene->clientObjects.size) {
            LOG_TRACE("Filled client object array, reallocating. This can be expensive");

            scene->clientObjects.size *= 2;
            scene->clientObjects.array = realloc(scene->clientObjects.array, sizeof(ClientObject) * scene->clientObjects.size);

            ASSERT(scene->clientObjects.array != NULL, "Failed to re-allocate %lu bytes of data for the array of client objects",
                   sizeof(ClientObject) * scene->clientObjects.size);
        }

        scene->clientObjects.array[scene->clientObjects.used++] = clientObject;
    }

    int DeleteClientObject(Scene* scene, const char* name, int multi) {
        int deleted = 0;

        for (size_t i = 0; i < scene->clientObjects.used - 1; i++) {
            int deletedSomething = 0;

            if (strcmp(scene->clientObjects.array[i].name, name) == 0 && (deleted == 0 || multi)) {
                deleted++;

                deletedSomething = 1;
            }

            if (deleted == 0)
                continue;

            scene->clientObjects.array[i] = scene->clientObjects.array[i + 1];

            scene->clientObjects.used--;

            if (deletedSomething)
                i--;
        }

        return deleted;
    }
CPP_GUARD_END()
