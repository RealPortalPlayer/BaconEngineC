project(SharedEngineCode)

set(BE_SOURCE_FILES
        ArgumentHandler.c 
        Launcher.c 
        Logger.c
        ANSI.c
        User.c
        MessageBox.c
        Threads.c
        Paths.c
        String.c)

add_library(SharedEngineCode STATIC ${BE_SOURCE_FILES})

if(NOT BE_DISABLE_PEDANTIC_MODE)
    if(MSVC)
        target_compile_options(SharedEngineCode PRIVATE /W4 /WX)
        target_compile_options(SharedEngineCode PUBLIC /wd4204)
    else()
        target_compile_options(SharedEngineCode PRIVATE -Wall -Wpedantic -Werror)
    endif()
endif()

target_include_directories(SharedEngineCode PUBLIC ../../include/SharedEngineCode)

if(MSVC)
    target_compile_definitions(SharedEngineCode PUBLIC -D_CRT_SECURE_NO_WARNINGS)
endif()

include(SharedEngineCodeStandalone.cmake)