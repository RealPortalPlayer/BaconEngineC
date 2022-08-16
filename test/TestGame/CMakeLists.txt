project(TestGame)

add_library(TestGame SHARED Entry.c Layers.c)

set_target_properties(TestGame PROPERTIES OUTPUT_NAME "binary")

target_link_libraries(TestGame LINK_PRIVATE BaconEngine)