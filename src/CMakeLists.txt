set(SOURCES
	main.c
	scanner.c
	scanner_states.c
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(
	container
)

add_executable(ifj20 ${SOURCES})
