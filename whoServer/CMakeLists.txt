cmake_minimum_required(VERSION 3.16)
project(whoServer)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(CMAKE_CXX_STANDARD 14)

add_executable(whoServer main.cpp Arguments.h List.cpp List.h Hash.cpp Hash.h Worker_list.h utils.cpp utils.h)

target_link_libraries(whoServer Threads::Threads)
