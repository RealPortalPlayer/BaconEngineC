project(SharedEngineCode)

add_library(SharedEngineCode STATIC ArgumentHandler.c Launcher.c Logger.c)

target_include_directories(SharedEngineCode PUBLIC ../../include/SharedEngineCode)