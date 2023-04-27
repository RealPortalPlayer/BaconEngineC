engine_project(NAME BaconLauncherUI SOURCES Entry.c STANDALONE)

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(BaconLauncherUI PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
    set_target_properties(BaconLauncherUI PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
endif()