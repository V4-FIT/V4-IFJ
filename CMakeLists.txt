cmake_minimum_required(VERSION 3.10)
project(ic20int C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS_DEBUG "-g -Wall -pedantic")
set(CMAKE_C_FLAGS_RELEASE "-O2")

include_directories(./src/)

set(SOURCES
	src/main.c
)

add_executable(ic20int ${SOURCES})

if(MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ic20int)
endif()
