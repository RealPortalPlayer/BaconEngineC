project(BaconEditor C)

add_library(BaconEditor SHARED Entry.c)

set_target_properties(BaconEditor PROPERTIES OUTPUT_NAME "binary")

target_link_libraries(BaconEditor LINK_PRIVATE BaconEngine)

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(BaconEditor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/BaconEditor")
endif()