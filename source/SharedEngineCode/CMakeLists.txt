project(SharedEngineCode)

add_library(SharedEngineCode STATIC
        Launcher.c
        MessageBox.c
        Paths.c
        Debugging/StrictMode.c
        ${BE_SOURCE_FILES})

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
target_link_libraries(SharedEngineCode LINK_PUBLIC BaconAPI)

if(MSVC)
    target_compile_definitions(SharedEngineCode PUBLIC -D_CRT_SECURE_NO_WARNINGS)
endif()

if(BA_DISABLE_HACKS)
    target_compile_definitions(SharedEngineCode PUBLIC BA_DISABLE_HACKS)
endif()