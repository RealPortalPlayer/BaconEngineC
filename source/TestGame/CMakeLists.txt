project(TestGame)

add_library(TestGame SHARED Entry.c)

target_link_libraries(TestGame LINK_PRIVATE BaconEngine)