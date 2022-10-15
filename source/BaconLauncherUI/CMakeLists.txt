project(BaconLauncherUI)

add_executable(BaconLauncherUI Entry.c)

target_link_libraries(BaconLauncherUI LINK_PRIVATE BaconEngineLauncher)

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(BaconLauncherUI PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
endif()