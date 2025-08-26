cmake_minimum_required(VERSION 3.1)
project(cpp_secureAndskill)

add_subdirectory(lecture-04-lib)
add_subdirectory(lecture-04)

# 의존성 명시
add_dependencies(lecture-04 lecture-04-libA lecture-04-libW)