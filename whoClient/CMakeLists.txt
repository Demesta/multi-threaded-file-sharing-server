cmake_minimum_required(VERSION 3.16)
project(whoClient)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(CMAKE_CXX_STANDARD 14)

add_executable(whoClient main.cpp Arguments.h sockets.cpp sockets.h logging.h logging.cpp)

target_link_libraries(whoClient Threads::Threads)