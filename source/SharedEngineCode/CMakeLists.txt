project(SharedEngineCode)

add_library(SharedEngineCode STATIC ArgumentHandler.c Launcher.c Logger.c ANSI.c User.c MessageBox.c Threads.c Paths.c String.c)

if(NOT BE_DISABLE_PEDANTIC_MODE)
    if(MSVC)
        target_compile_options(SharedEngineCode PRIVATE /W4 /WX)
    else()
        target_compile_options(SharedEngineCode PRIVATE -Wall -Wpedantic -Werror)
    endif()
endif()

target_include_directories(SharedEngineCode PUBLIC ../../include/SharedEngineCode)

if(MSVC)
    target_compile_definitions(SharedEngineCode PUBLIC -D_CRT_SECURE_NO_WARNINGS)
endif()