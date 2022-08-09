project(BaconLauncher)

add_executable(BaconLauncher Main.c)

target_link_libraries(BaconLauncher LINK_PRIVATE SharedEngineCode)