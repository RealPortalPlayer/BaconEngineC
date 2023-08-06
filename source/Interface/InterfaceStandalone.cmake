project(InterfaceStandalone)

add_library(InterfaceStandalone STATIC EntryPoint.c)

target_include_directories(InterfaceStandalone PUBLIC ../../include/Interface)

if(NOT BE_DISABLE_PEDANTIC_MODE)
    if(MSVC)
        target_compile_options(InterfaceStandalone PRIVATE /W4 /WX)
    else()
        target_compile_options(InterfaceStandalone PRIVATE -Wall -Wpedantic -Werror)
    endif()
endif()

target_link_libraries(InterfaceStandalone LINK_PUBLIC SharedEngineCodeStandalone)

target_compile_definitions(InterfaceStandalone PUBLIC BE_ENGINE_VERSION="${BE_ENGINE_VERSION}")