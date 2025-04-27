#include "Nexpp/FileSystem/FileSystem.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string>

class FileSystemTest : public ::testing::Test
{
protected:
  std::filesystem::path test_dir = "test_tmp/";

  void                  SetUp() override
  {
    std::filesystem::remove_all(test_dir);
    std::filesystem::create_directory(test_dir);
  }

  void TearDown() override
  {
    std::filesystem::remove_all(test_dir);
  }

  bool file_contains(
      const std::filesystem::path &file_path,
      const std::string           &expected_content
  )
  {
    std::ifstream ifs(file_path);
    std::string   content(
        (std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>()
    );
    return content == expected_content;
  }

  std::string read_file(const std::filesystem::path &file_path)
  {
    std::ifstream ifs(file_path);
    return std::string(
        (std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>()
    );
  }
};

TEST_F(FileSystemTest, CreateFolderCreatesDirectory)
{
  std::string folder_name = "my_folder";
  FileSystem::create_folder(test_dir, folder_name);
  EXPECT_TRUE(std::filesystem::exists(test_dir / folder_name));
  EXPECT_TRUE(std::filesystem::is_directory(test_dir / folder_name));
}

TEST_F(FileSystemTest, CreateFolderOnExistingDirectoryDoesNotFail)
{
  std::string folder_name = "existing_folder";
  FileSystem::create_folder(test_dir, folder_name);
  FileSystem::create_folder(test_dir, folder_name);
  EXPECT_TRUE(std::filesystem::exists(test_dir / folder_name));
}

TEST_F(FileSystemTest, CreateFileCreatesEmptyFile)
{
  std::string file_name = "my_file.txt";
  FileSystem::create_file(test_dir, file_name);
  EXPECT_TRUE(std::filesystem::exists(test_dir / file_name));
  EXPECT_TRUE(std::filesystem::is_regular_file(test_dir / file_name));
  EXPECT_EQ(read_file(test_dir / file_name), "");
}

TEST_F(FileSystemTest, CreateFileOverExistingFileTruncates)
{
  std::string file_name = "truncate_test.txt";
  FileSystem::create_file(test_dir, file_name);
  FileSystem::put_in_file(test_dir / file_name, "Old content");
  FileSystem::create_file(test_dir, file_name);
  EXPECT_EQ(read_file(test_dir / file_name), "");
}

TEST_F(FileSystemTest, PutInFileWritesContent)
{
  std::string file_name = "content.txt";
  std::string content   = "Hello, Nexpp!";
  FileSystem::create_file(test_dir, file_name);
  FileSystem::put_in_file(test_dir / file_name, content);
  EXPECT_TRUE(file_contains(test_dir / file_name, content));
}

TEST_F(FileSystemTest, PutInFileOverwritesExistingContent)
{
  std::string file_name = "overwrite.txt";
  FileSystem::create_file(test_dir, file_name);
  FileSystem::put_in_file(test_dir / file_name, "Old content");
  FileSystem::put_in_file(test_dir / file_name, "New content");
  EXPECT_TRUE(file_contains(test_dir / file_name, "New content"));
}

TEST_F(FileSystemTest, PutInFileOnNonExistingFileCreatesFile)
{
  std::string file_name = "new_file.txt";
  FileSystem::put_in_file(test_dir / file_name, "Auto-created file");
  EXPECT_TRUE(file_contains(test_dir / file_name, "Auto-created file"));
}

TEST_F(FileSystemTest, AppendInFileAppendsContent)
{
  std::string file_name = "append.txt";
  FileSystem::create_file(test_dir, file_name);
  FileSystem::put_in_file(test_dir / file_name, "Line1\n");
  FileSystem::append_in_file(test_dir / file_name, "Line2");
  EXPECT_EQ(read_file(test_dir / file_name), "Line1\nLine2");
}

TEST_F(FileSystemTest, AppendInFileCreatesFileIfMissing)
{
  std::string file_name = "append_create.txt";
  FileSystem::append_in_file(test_dir / file_name, "First line");
  EXPECT_TRUE(file_contains(test_dir / file_name, "First line"));
}

TEST_F(FileSystemTest, CreateSymlinkCreatesValidSymlink)
{
  std::string           target_file  = "target.txt";
  std::string           symlink_name = "link_to_target.txt";

  std::filesystem::path target_path =
      std::filesystem::absolute(test_dir / target_file);
  std::filesystem::path symlink_path =
      std::filesystem::absolute(test_dir / symlink_name);

  FileSystem::create_file(test_dir, target_file);
  FileSystem::create_symlink(target_path, symlink_path);

  EXPECT_TRUE(std::filesystem::exists(symlink_path));
  EXPECT_TRUE(std::filesystem::is_symlink(symlink_path));

  auto resolved_target = std::filesystem::canonical(symlink_path);
  auto expected_target = std::filesystem::canonical(target_path);
  EXPECT_EQ(resolved_target, expected_target);
}
