#include <stdlib.h>
#include <SharedEngineCode/Logger.h>
#include <string.h>
#include <SharedEngineCode/OperatingSystem.h>

#include "BaconEngine/SceneLogic/Scene.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Debugging/StrictMode.h"

CPP_GUARD_START()
    const unsigned char sceneMagic[10] = {0xFF, 0x53, 0x43, 0x45, 0x4E, 0x45, 0x2E, 0x42, 0x45, 0xFF};

    void CreateScene(Scene* scene, const char* name) {
        scene->name = name;
        scene->clientObjects = (ClientObjectArray) {
            .array = malloc(sizeof(ClientObject) * 100),
            .used = 0,
            .size = 100
        };

        ASSERT(scene->clientObjects.array != NULL, "Failed to allocate %lu bytes of data for the array of client objects",
               sizeof(ClientObject) * 100);
    }

    int LoadScene(Scene* scene, const char* name) { // TODO: Load scene.
        (void) scene;
        (void) name;
        return 0;
//        (void) scene;
//        char* fileName = NULL;
//
//        memset(fileName, 0, strlen(name) + 6);
//        sprintf(fileName, "%s.scene", name);
//
//        FILE* sceneFile = fopen(fileName, "rb");
//
//        STRICT_CHECK(sceneFile != NULL, 0, "The requested scene does not exist");
//
//        {
//            char magic[sizeof(sceneMagic)];
//            unsigned long read = fread(magic, sizeof(char), sizeof(sceneMagic), sceneFile);
//            // FIXME: This will cause memory leaks.
//            STRICT_CHECK(read == sizeof(sceneMagic) && memcmp(magic, sceneMagic, sizeof(sceneMagic)) == 0, 0, "Either not a scene, or it's heavily corrupted");
//        }
//
//        char* sceneName = NULL;
//
//        {
//            int donePartOne = 0;
//
//            while (1) {
//                int character;
//                int size = 0;
//
//                while ((character = fgetc(sceneFile)) != 0xFF && character != EOF) {
//                    if (donePartOne != 0)
//                        sceneName[size] = (char) character;
//
//                    size++;
//                }
//
//                if (donePartOne != 0)
//                    break;
//
//                sceneName = malloc(sizeof(char) * size + 1);
//
//                fseek(sceneFile, -(size + 1), SEEK_CUR);
//
//                donePartOne = 1;
//            }
//        }
//
//        STRICT_CHECK(!feof(sceneFile), 0, "Scene is corrupted");
//
//        scene->name = sceneName;
//
//        fclose(sceneFile);
//
//        return 1;
    }

    void InsertClientObject(Scene* scene, ClientObject clientObject) {
        if (scene->clientObjects.used == scene->clientObjects.size) {
            LOG_TRACE("Filled client object array, reallocating. This can be expensive");

            scene->clientObjects.size *= 2;
            scene->clientObjects.array = realloc(scene->clientObjects.array, sizeof(ClientObject) * scene->clientObjects.size);

            ASSERT(scene->clientObjects.array != NULL, "Failed to reallocate %lu bytes of data for the array of client objects",
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
