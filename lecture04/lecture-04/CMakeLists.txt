cmake_minimum_required(VERSION 3.10)
project(lecture-04)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../Build/LinuxRelease)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../Build/LinuxRelease)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../Build/LinuxRelease)

file(GLOB_RECURSE SOURCES *.cpp *.c)

foreach(SOURCE IN LISTS SOURCES)
message(STATUS "SOURCE - " ${SOURCE})
endforeach(SOURCE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cppcore/Inc)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cppcore/Build/LinuxRelease)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Build/LinuxRelease)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} 
lecture-04-libW 
lecture-04-libA
cppcore
)