cmake_minimum_required(VERSION 3.28)
project(Nexpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)
qt_add_resources(APP_RESOURCES resources.qrc)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

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

enable_testing()

add_executable(
  nexpp_tests
  tests/hello_test.cpp 
)

target_link_libraries(
  nexpp_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(nexpp_tests)
