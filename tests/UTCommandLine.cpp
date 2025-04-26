#include "Nexpp/CommandLine/CommandLine.h"
#include <QApplication>
#include <QStringList>
#include <gtest/gtest.h>
#include <string>
#include <vector>

class CommandLineTest : public ::testing::Test
{
protected:
  int                      argc = 0;
  std::vector<std::string> argv_storage;
  std::vector<char *>      argv;

  void                     SetUp() override
  {
    argv_storage.clear();
    argv.clear();
  }

  void TearDown() override
  {
    argv_storage.clear();
    argv.clear();
  }

  void prepare_args(const std::vector<std::string> &args)
  {
    argc         = static_cast<int>(args.size());
    argv_storage = args;
    argv.clear();
    for(auto &str : argv_storage) {
      argv.push_back(const_cast<char *>(str.c_str()));
    }
    argv.push_back(nullptr);
  }

  char **get_argv()
  {
    return argv.data();
  }
};

TEST_F(CommandLineTest, DefaultModeIsCLI)
{
  prepare_args({"nexpp", "-n", "TestProject"});
  QApplication app(argc, get_argv());
  CommandLine  cmd(app);
  EXPECT_EQ(cmd.get_mode(), AppMode::CLI);
}

TEST_F(CommandLineTest, GUI_Mode_Selection)
{
  prepare_args({"nexpp", "-n", "TestProject", "-m", "gui"});
  QApplication app(argc, get_argv());
  CommandLine  cmd(app);
  EXPECT_EQ(cmd.get_mode(), AppMode::GUI);
}

TEST_F(CommandLineTest, InvalidModeThrows)
{
  prepare_args({"nexpp", "-n", "TestProject", "-m", "invalid"});
  QApplication app(argc, get_argv());
  EXPECT_THROW(CommandLine cmd(app), std::runtime_error);
}

TEST_F(CommandLineTest, MissingProjectNameThrows)
{
  prepare_args({"nexpp"});
  QApplication app(argc, get_argv());
  EXPECT_THROW(CommandLine cmd(app), std::runtime_error);
}

TEST_F(CommandLineTest, DestinationDefaultsToCurrentDirectory)
{
  prepare_args({"nexpp", "-n", "TestProject"});
  QApplication app(argc, get_argv());
  CommandLine  cmd(app);
  EXPECT_EQ(cmd.get_destination(), "./");
}

TEST_F(CommandLineTest, DestinationIsSetCorrectly)
{
  prepare_args({"nexpp", "-n", "TestProject", "-d", "/tmp/output"});
  QApplication app(argc, get_argv());
  CommandLine  cmd(app);
  EXPECT_EQ(cmd.get_destination(), "/tmp/output");
}

TEST_F(CommandLineTest, RecognizesAllowedLibraries)
{
  prepare_args({"nexpp", "-n", "TestProject", "-l", "qt,gtest"});
  QApplication app(argc, get_argv());
  CommandLine  cmd(app);
  EXPECT_EQ(cmd.get_libraries(), QStringList({"qt", "gtest"}));
}

TEST_F(CommandLineTest, UnrecognizedLibraryThrows)
{
  prepare_args({"nexpp", "-n", "TestProject", "-l", "boost"});
  QApplication app(argc, get_argv());
  EXPECT_THROW(CommandLine cmd(app), std::runtime_error);
}

TEST_F(CommandLineTest, DedupLibraries)
{
  prepare_args({"nexpp", "-n", "TestProject", "-l", "qt,qt"});
  QApplication app(argc, get_argv());
  CommandLine  cmd(app);
  EXPECT_EQ(cmd.get_libraries().size(), 1);
}

TEST_F(CommandLineTest, StandardDefaultsToCPP23)
{
  prepare_args({"nexpp", "-n", "TestProject"});
  QApplication app(argc, get_argv());
  CommandLine  cmd(app);
  EXPECT_EQ(cmd.get_standard(), Standard::CPP23);
}

TEST_F(CommandLineTest, SelectStandard17)
{
  prepare_args({"nexpp", "-n", "TestProject", "-s", "17"});
  QApplication app(argc, get_argv());
  CommandLine  cmd(app);
  EXPECT_EQ(cmd.get_standard(), Standard::CPP17);
}

TEST_F(CommandLineTest, InvalidStandardFallsBackToCPP23)
{
  prepare_args({"nexpp", "-n", "TestProject", "-s", "99"});
  QApplication app(argc, get_argv());
  CommandLine  cmd(app);
  EXPECT_EQ(cmd.get_standard(), Standard::CPP23);
}

TEST_F(CommandLineTest, FlagsOptionIsDetected)
{
  prepare_args({"nexpp", "-n", "TestProject", "-f"});
  QApplication app(argc, get_argv());
  CommandLine  cmd(app);
  EXPECT_TRUE(cmd.has_flags());
}

TEST_F(CommandLineTest, FlagsOptionAbsentIsFalse)
{
  prepare_args({"nexpp", "-n", "TestProject"});
  QApplication app(argc, get_argv());
  CommandLine  cmd(app);
  EXPECT_FALSE(cmd.has_flags());
}
