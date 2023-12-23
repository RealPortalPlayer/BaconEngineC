engine_project(NAME BaconEditor SOURCES Entry.c)

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(BaconEditor PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/Clients/BaconEditor")
    set_target_properties(BaconEditor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/Clients/BaconEditor")
endif()