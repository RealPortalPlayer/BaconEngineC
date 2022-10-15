project(TestClientCPP)

add_library(TestClientCPP SHARED Entry.cpp)

set_target_properties(TestClientCPP PROPERTIES OUTPUT_NAME "binary")

target_link_libraries(TestClientCPP LINK_PRIVATE BaconEngine)

if(NOT BE_DONT_SET_BINARY_OUTPUT_PATH)
    set_target_properties(TestClientCPP PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/TestClientCPP")
endif()