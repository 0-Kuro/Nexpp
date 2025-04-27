#pragma once

#include <filesystem>

class FileSystem
{
public:
  static void
      create_folder(std::filesystem::path path, std::string folder_name);
  static void create_file(std::filesystem::path path, std::string filename);

  static void put_in_file(std::filesystem::path path, std::string content);
  static void append_in_file(std::filesystem::path path, std::string content);

  static void create_symlink(
      std::filesystem::path origin, std::filesystem::path destination
  );
};
