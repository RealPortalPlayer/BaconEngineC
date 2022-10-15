project(CustomStandaloneClientBinary C)

add_executable(CustomStandaloneClientBinary Entry.c)

target_link_libraries(CustomStandaloneClientBinary LINK_PRIVATE BaconEngine)

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(CustomStandaloneClientBinary PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
endif()