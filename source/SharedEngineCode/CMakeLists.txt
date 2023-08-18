project(SharedEngineCode)

add_library(SharedEngineCode STATIC
        ArgumentHandler.c
        Launcher.c
        Logger.c
        ANSI.c
        User.c
        MessageBox.c
        Threads.c
        Paths.c
        String.c
        Storage/DynamicArray.c
        Storage/DynamicDictionary.c
        Debugging/Assert.c
        Debugging/StrictMode.c)

if(NOT "${BE_ASAN_SANITIZER}" STREQUAL "")
    if(MSVC)
        target_compile_options(SharedEngineCode PUBLIC /fsanitize=${BE_ASAN_SANITIZER})
        target_link_options(SharedEngineCode PUBLIC /fsanitize=${BE_ASAN_SANITIZER})
        target_compile_definitions(SharedEngineCode PUBLIC BE_ASAN_ENABLED)
    else()
        target_compile_options(SharedEngineCode PUBLIC -fsanitize=${BE_ASAN_SANITIZER})
        target_link_options(SharedEngineCode PUBLIC -fsanitize=${BE_ASAN_SANITIZER})
        target_compile_definitions(SharedEngineCode PUBLIC BE_ASAN_ENABLED)
    endif()
endif()

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