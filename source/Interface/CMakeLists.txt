project(Interface)

add_library(Interface STATIC EntryPoint.c)

target_include_directories(Interface PUBLIC ../../include/Interface)

if(NOT BE_DISABLE_PEDANTIC_MODE)
    if(MSVC)
        target_compile_options(Interface PRIVATE /W4 /WX)
    else()
        target_compile_options(Interface PRIVATE -Wall -Wpedantic -Werror)
    endif()
endif()

target_link_libraries(Interface LINK_PUBLIC SharedEngineCode)

target_compile_definitions(Interface PUBLIC BE_ENGINE_VERSION="${BE_ENGINE_VERSION}")

include(InterfaceStandalone.cmake)