cmake_minimum_required(VERSION 3.28)
project(ProjectName)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
  ProjectName
  src/main.cpp
)

target_include_directories(ProjectName PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
