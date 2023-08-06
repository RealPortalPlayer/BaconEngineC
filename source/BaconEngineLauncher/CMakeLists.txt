project(BaconEngineLauncher C)

add_library(BaconEngineLauncher STATIC Main.c)

target_compile_definitions(BaconEngineLauncher PUBLIC BE_STANDALONE_CLIENT BE_ENGINE_VERSION="${BE_ENGINE_VERSION}" BE_LAUNCHER_VERSION="${BE_LAUNCHER_VERSION}")

if(NOT BE_DISABLE_PEDANTIC_MODE)
    if(MSVC)
        target_compile_options(BaconEngineLauncher PRIVATE /W4 /WX)
    else()
        target_compile_options(BaconEngineLauncher PRIVATE -Wall -Wpedantic -Werror)
    endif()
endif()

target_link_libraries(BaconEngineLauncher LINK_PUBLIC BaconEngineClientStandalone)

if(NOT WIN32)
    target_compile_options(BaconEngineLauncher PUBLIC -rdynamic)
endif()