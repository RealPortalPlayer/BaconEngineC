project(BaconEngineClient)

add_library(BaconEngineClient STATIC ${BE_SOURCE_FILES})

target_include_directories(BaconEngineClient PUBLIC ../../include/BaconEngine)
target_compile_definitions(BaconEngineClient PUBLIC BE_CLIENT_BINARY)

if(NOT BE_DISABLE_PEDANTIC_MODE)
    if(MSVC)
        target_compile_options(BaconEngineClient PRIVATE /W4 /WX)
    else()
        target_compile_options(BaconEngineClient PRIVATE -Wall -Wpedantic -Werror)
    endif()
endif()

target_link_libraries(BaconEngineClient LINK_PUBLIC Interface zip)
add_dependencies(BaconEngineClient BaconEngine)