project(SharedEngineCode)

add_library(SharedEngineCode STATIC ArgumentHandler.c Launcher.c Logger.c ANSI.c OSUser.c StringExtension.c)

target_include_directories(SharedEngineCode PUBLIC ../../include/SharedEngineCode)