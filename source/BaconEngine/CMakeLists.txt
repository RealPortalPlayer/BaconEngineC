project(BaconEngine)

add_library(BaconEngine STATIC dummy.c)

target_include_directories(BaconEngine PUBLIC ../../include/BaconEngine)
target_link_libraries(BaconEngine LINK_PUBLIC SharedEngineCode)