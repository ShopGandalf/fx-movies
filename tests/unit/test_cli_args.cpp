#include <gtest/gtest.h>
#include "src/app/cli.h"
#include <filesystem>
#include <fstream>

class CliArgsTest : public ::testing::Test {
protected:
    std::filesystem::path test_dir;
    std::filesystem::path test_file;

    void SetUp() override {
        test_dir = std::filesystem::temp_directory_path() / "fx_movies_test_cli";
        std::filesystem::create_directories(test_dir);

        test_file = test_dir / "dummy.txt";
        std::ofstream(test_file) << "test";
    }

    void TearDown() override {
        std::filesystem::remove_all(test_dir);
    }
};

TEST_F(CliArgsTest, ValidDirectory) {
    const char* argv[] = {"fx_movies", test_dir.c_str()};
    auto result = fx::parse_args(2, const_cast<char**>(argv));
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.exit_code, 0);
    EXPECT_EQ(result.directory_path, std::filesystem::canonical(test_dir));
    EXPECT_TRUE(result.error_message.empty());
}

TEST_F(CliArgsTest, MissingArgument) {
    const char* argv[] = {"fx_movies"};
    auto result = fx::parse_args(1, const_cast<char**>(argv));
    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.exit_code, 1);
    EXPECT_NE(result.error_message.find("No directory path provided"), std::string::npos);
}

TEST_F(CliArgsTest, NonExistentPath) {
    const char* argv[] = {"fx_movies", "/nonexistent/path/12345"};
    auto result = fx::parse_args(2, const_cast<char**>(argv));
    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.exit_code, 2);
    EXPECT_NE(result.error_message.find("does not exist"), std::string::npos);
}

TEST_F(CliArgsTest, PathIsFile) {
    std::string file_str = test_file.string();
    const char* argv[] = {"fx_movies", file_str.c_str()};
    auto result = fx::parse_args(2, const_cast<char**>(argv));
    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.exit_code, 2);
    EXPECT_NE(result.error_message.find("not a directory"), std::string::npos);
}

TEST_F(CliArgsTest, ValidDirectoryWithTrailingSlash) {
    std::string dir_with_slash = test_dir.string() + "/";
    const char* argv[] = {"fx_movies", dir_with_slash.c_str()};
    auto result = fx::parse_args(2, const_cast<char**>(argv));
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.exit_code, 0);
}
