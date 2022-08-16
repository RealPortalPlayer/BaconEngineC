project(BaconEngineLauncher C)

add_library(BaconEngineLauncher STATIC ../BaconLauncher/Main.c)

target_compile_definitions(BaconEngineLauncher PUBLIC BACON_ENGINE_LAUNCHER)

target_link_libraries(BaconEngineLauncher LINK_PUBLIC BaconEngine)