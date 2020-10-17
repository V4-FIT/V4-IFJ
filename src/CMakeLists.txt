set(SOURCES
	main.c
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ifj20 ${SOURCES})

if(MSVC)
	#...
else()
	set(CMAKE_C_FLAGS_DEBUG "-g -Wall -pedantic")
	set(CMAKE_C_FLAGS_RELEASE "-O2")
endif()
