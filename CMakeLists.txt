cmake_minimum_required(VERSION 3.10)
project(ic20int C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS_DEBUG "-g -Wall -pedantic")
set(CMAKE_C_FLAGS_RELEASE "-O2")

set(SOURCES src/main.c)

add_executable(ic20int ${SOURCES})
