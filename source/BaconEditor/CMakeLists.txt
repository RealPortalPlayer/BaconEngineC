project(BaconEditor C)

add_library(BaconEditor SHARED Entry.c)

set_target_properties(BaconEditor PROPERTIES OUTPUT_NAME "binary")

target_link_libraries(BaconEditor LINK_PRIVATE BaconEngine)