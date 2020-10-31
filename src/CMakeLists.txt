# V4-IFJ/src/CMakeLists.txt
set(SOURCES
	main.c
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(
	container
)

add_executable(ifj20 ${SOURCES})
