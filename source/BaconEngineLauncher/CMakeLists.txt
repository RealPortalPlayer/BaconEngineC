project(BaconEngineLauncher C)

add_library(BaconEngineLauncher STATIC Main.c)

target_compile_definitions(BaconEngineLauncher PUBLIC BACON_ENGINE_LAUNCHER BE_ENGINE_VERSION="${BE_ENGINE_VERSION}")

target_link_libraries(BaconEngineLauncher LINK_PUBLIC BaconEngineClient)

if(NOT WIN32)
    target_compile_options(BaconEngineLauncher PUBLIC -rdynamic)
endif()