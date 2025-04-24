cmake_minimum_required(VERSION 3.28)
project(Nexpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

option(BUILD_TESTS_ONLY "Build only unit tests (no main application)" OFF)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

if(NOT BUILD_TESTS_ONLY)
  find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)
  qt_add_resources(APP_RESOURCES resources.qrc)

  add_executable(Nexpp 
    src/main.cpp
    ${APP_RESOURCES}
  )

  target_include_directories(Nexpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

  target_link_libraries(Nexpp 
    Qt6::Widgets 
    Qt6::Svg
  )

  target_compile_options(Nexpp PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wimplicit-fallthrough
  )
endif()

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
test_discover_tests(nexpp_tests)
