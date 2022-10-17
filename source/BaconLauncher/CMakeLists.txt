project(BaconLauncher C)

if(WIN32)
    add_executable(BaconLauncher WIN32 Main.c)
else()
    add_executable(BaconLauncher Main.c)
endif()

target_link_libraries(BaconLauncher LINK_PRIVATE SharedEngineCode)

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(BaconLauncher PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
endif()