set(SOURCES
    char_sequence.c
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(container STATIC ${SOURCES})
