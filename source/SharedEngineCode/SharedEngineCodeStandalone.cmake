project(SharedEngineCodeStandalone)

add_library(SharedEngineCodeStandalone STATIC ${BE_SOURCE_FILES})

if(NOT "${BE_ASAN_SANITIZER}" STREQUAL "")
    if(MSVC)
        target_compile_options(SharedEngineCodeStandalone PUBLIC /fsanitize=${BE_ASAN_SANITIZER})
        target_link_options(SharedEngineCodeStandalone PUBLIC /fsanitize=${BE_ASAN_SANITIZER})
        target_compile_definitions(SharedEngineCodeStandalone PUBLIC BE_ASAN_ENABLED)
    else()
        target_compile_options(SharedEngineCodeStandalone PUBLIC -fsanitize=${BE_ASAN_SANITIZER})
        target_link_options(SharedEngineCodeStandalone PUBLIC -fsanitize=${BE_ASAN_SANITIZER})
        target_compile_definitions(SharedEngineCodeStandalone PUBLIC BE_ASAN_ENABLED)
    endif()
endif()

if(NOT BE_DISABLE_PEDANTIC_MODE)
    if(MSVC)
        target_compile_options(SharedEngineCodeStandalone PRIVATE /W4 /WX)
    else()
        target_compile_options(SharedEngineCodeStandalone PRIVATE -Wall -Wpedantic -Werror)
    endif()
endif()

target_compile_definitions(SharedEngineCodeStandalone PUBLIC BE_STANDALONE_CLIENT)

target_include_directories(SharedEngineCodeStandalone PUBLIC ../../include/SharedEngineCode)

if(MSVC)
    target_compile_definitions(SharedEngineCodeStandalone PUBLIC -D_CRT_SECURE_NO_WARNINGS)
endif()