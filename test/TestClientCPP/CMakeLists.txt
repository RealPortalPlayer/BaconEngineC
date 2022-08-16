project(TestClientCPP)

add_library(TestClientCPP SHARED Entry.cpp)

set_target_properties(TestClientCPP PROPERTIES OUTPUT_NAME "binary")

target_link_libraries(TestClientCPP LINK_PRIVATE BaconEngine)