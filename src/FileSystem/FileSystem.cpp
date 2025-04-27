#include "Nexpp/FileSystem/FileSystem.h"

#include <filesystem>
#include <fstream>

void FileSystem::create_folder(
    std::filesystem::path path, std::string folder_name
)
{
  std::filesystem::create_directory(path.string() + folder_name);
}

void FileSystem::create_file(std::filesystem::path path, std::string filename)
{
  path /= filename;
  std::ofstream ofs(path);
  ofs.close();
}

void FileSystem::put_in_file(std::filesystem::path path, std::string content)
{
  std::ofstream ofs(path);
  ofs << content;
  ofs.close();
}

void FileSystem::append_in_file(std::filesystem::path path, std::string content)
{
  std::ofstream ofs(path, std::ios::app);
  ofs << content;
  ofs.close();
}

void FileSystem::create_symlink(
    std::filesystem::path origin, std::filesystem::path destination
)
{
  std::filesystem::create_symlink(origin, destination);
}
