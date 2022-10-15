project(TestGame)

add_library(TestGame SHARED Entry.c Layers.c)

set_target_properties(TestGame PROPERTIES OUTPUT_NAME "binary")

target_link_libraries(TestGame LINK_PRIVATE BaconEngine)

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(TestGame PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/TestGame")
endif()