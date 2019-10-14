cmake_minimum_required(VERSION 3.8)
project(schedule)

find_package(Threads)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
        schedule.h
        timer.h
        schedule.cpp
        queue.h )
add_executable(schedule ${SOURCE_FILES})
target_link_libraries(schedule ${CMAKE_THREAD_LIBS_INIT})