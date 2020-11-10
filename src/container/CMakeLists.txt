# V4-IFJ/src/container/CMakeLists.txt
set(
        SOURCES
        char_sequence.c
        forward_list.c
        hash_map.c
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(container STATIC ${SOURCES})
