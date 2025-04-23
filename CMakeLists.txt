cmake_minimum_required(VERSION 3.28)
project(Nexpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)
qt_add_resources(APP_RESOURCES resources.qrc)

add_executable(
  Nexpp 
  src/main.cpp
)

target_sources(
  Nexpp
  PRIVATE
  ${APP_RESOURCES}
)

target_include_directories(Nexpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Nexpp Qt6::Widgets Qt6::Svg)
