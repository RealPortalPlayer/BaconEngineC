project(StandaloneClientBinary C)

add_executable(StandaloneClientBinary Entry.c)

target_link_libraries(StandaloneClientBinary LINK_PRIVATE BaconEngineLauncher)

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(StandaloneClientBinary PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
endif()