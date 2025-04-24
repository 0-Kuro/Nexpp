#include <QString>

#include "Nexpp/Data/CMakeBase.h"

std::string CMakeBase::setup_config(
    const std::string &project_name, Standard cpp_standard, bool has_flags
) const
{
  QString base_config = QString(
                            "cmake_minimum_required(VERSION 3.28)\n"
                            "project(%1)\n"
                            "\n"
                            "set(CMAKE_CXX_STANDARD %2)\n"
                            "\n"
                            "add_executable(\n"
                            "  %1\n"
                            "  src/main.cpp\n"
                            ")\n"
                            "\n"
                            "target_include_directories(\n"
                            "  %1\n"
                            "  PRIVATE\n"
                            "  ${CMAKE_CURRENT_SOURCE_DIR}/include\n"
                            ")\n"
  )
                            .arg(project_name)
                            .arg(to_string(cpp_standard));

  if(has_flags) {
    base_config.append(QString(
                           "\n"
                           "target_compile_options(\n"
                           "  %1\n"
                           "  PRIVATE\n"
                           "  -Wall\n"
                           "  -Wextra\n"
                           "  -Wpedantic\n"
                           "  -Werror\n"
                           "  -Wshadow\n"
                           "  -Wnon-virtual-dtor\n"
                           "  -Wold-style-cast\n"
                           "  -Wcast-align\n"
                           "  -Wunused\n"
                           "  -Wconversion\n"
                           "  -Wsign-conversion\n"
                           "  -Wnull-dereference\n"
                           "  -Wdouble-promotion\n"
                           "  -Wimplicit-falltrough\n"
                           ")\n"
    )
                           .arg(project_name));
  }

  return base_config.toStdString();
}
