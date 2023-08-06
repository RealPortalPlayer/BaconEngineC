project(BaconEngineClientStandalone)

add_library(BaconEngineClientStandalone STATIC ${BE_SOURCE_FILES})

target_include_directories(BaconEngineClientStandalone PUBLIC ../../include/BaconEngine)
target_compile_definitions(BaconEngineClientStandalone PUBLIC BE_CLIENT_BINARY)

if(NOT BE_DISABLE_PEDANTIC_MODE)
    if(MSVC)
        target_compile_options(BaconEngineClientStandalone PRIVATE /W4 /WX)
    else()
        target_compile_options(BaconEngineClientStandalone PRIVATE -Wall -Wpedantic -Werror)
    endif()
endif()

target_link_libraries(BaconEngineClientStandalone LINK_PUBLIC InterfaceStandalone zip)
add_dependencies(BaconEngineClientStandalone BaconEngine)