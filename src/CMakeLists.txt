# V4-IFJ/src/CMakeLists.txt
set(
        SOURCES
        main.c
        scanner.c
        scanner_states.c
)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        container
)

add_subdirectory(
        container
)

add_executable(ifj20 ${SOURCES})

target_link_libraries(ifj20 container)
