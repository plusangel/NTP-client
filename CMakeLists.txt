cmake_minimum_required(VERSION 3.0.0)
project(ntp_client)

set(CMAKE_CXX_STANDARD 17)

add_library(client client.cpp)

add_executable(ntp_client main.cpp)
target_link_libraries(ntp_client client)
