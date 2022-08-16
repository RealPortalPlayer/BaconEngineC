project(BaconLauncherUI)

add_executable(BaconLauncherUI Entry.c)

target_link_libraries(BaconLauncherUI LINK_PRIVATE BaconEngineLauncher)