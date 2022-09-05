project(SharedEngineCode)

add_library(SharedEngineCode STATIC ArgumentHandler.c Launcher.c Logger.c Storage/DynamicDictionary.c Storage/DynamicArray.c ANSI.c EngineMemory.c)

target_include_directories(SharedEngineCode PUBLIC ../../include/SharedEngineCode)