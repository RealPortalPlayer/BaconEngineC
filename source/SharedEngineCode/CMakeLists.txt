project(SharedEngineCode)

add_library(SharedEngineCode STATIC ArgumentHandler.c Launcher.c Logger.c ANSI.c User.c StringExtension.c MessageBox.c Threads.c)

target_include_directories(SharedEngineCode PUBLIC ../../include/SharedEngineCode)

if(MSVC)
    target_compile_definitions(SharedEngineCode PUBLIC -D_CRT_SECURE_NO_WARNINGS)
endif()