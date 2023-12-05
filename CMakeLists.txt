cmake_minimum_required(VERSION 3.0.0)
project(ntp_client)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SOURCES src/*.cpp include/*.h*)
include_directories(include)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

add_executable(ntp_example main.cpp)
target_link_libraries(ntp_example ${PROJECT_NAME})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32)
endif()

# Instalación
INSTALL(TARGETS ${PROJECT_NAME}
        LIBRARY
        DESTINATION bin)

install(DIRECTORY "include/"
    DESTINATION "include"
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
)