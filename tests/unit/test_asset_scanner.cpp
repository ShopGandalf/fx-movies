#include <gtest/gtest.h>
#include "src/asset/asset_scanner.h"
#include <filesystem>
#include <fstream>

class AssetScannerTest : public ::testing::Test {
protected:
    std::filesystem::path test_dir;

    void SetUp() override {
        test_dir = std::filesystem::temp_directory_path() / "fx_movies_test_scanner";
        std::filesystem::remove_all(test_dir);
        std::filesystem::create_directories(test_dir);
    }

    void TearDown() override {
        std::filesystem::remove_all(test_dir);
    }

    void create_file(const std::string& name) {
        std::ofstream(test_dir / name) << "dummy";
    }

    void create_subdir(const std::string& name) {
        std::filesystem::create_directories(test_dir / name);
    }
};

TEST_F(AssetScannerTest, FindsVideoFiles) {
    create_file("video.mp4");
    create_file("clip.avi");
    create_file("movie.mkv");

    auto assets = fx::scan_directory(test_dir);
    EXPECT_EQ(assets.size(), 3);
}

TEST_F(AssetScannerTest, FiltersNonVideoFiles) {
    create_file("video.mp4");
    create_file("readme.txt");
    create_file("photo.jpg");
    create_file("doc.pdf");

    auto assets = fx::scan_directory(test_dir);
    EXPECT_EQ(assets.size(), 1);
    EXPECT_EQ(assets[0].filename, "video.mp4");
}

TEST_F(AssetScannerTest, EmptyDirectory) {
    auto assets = fx::scan_directory(test_dir);
    EXPECT_TRUE(assets.empty());
}

TEST_F(AssetScannerTest, NoVideoFiles) {
    create_file("readme.txt");
    create_file("photo.jpg");

    auto assets = fx::scan_directory(test_dir);
    EXPECT_TRUE(assets.empty());
}

TEST_F(AssetScannerTest, ExtractsCorrectFilename) {
    create_file("my_video.mp4");

    auto assets = fx::scan_directory(test_dir);
    ASSERT_EQ(assets.size(), 1);
    EXPECT_EQ(assets[0].filename, "my_video.mp4");
    EXPECT_EQ(assets[0].extension, ".mp4");
}

TEST_F(AssetScannerTest, CaseInsensitiveExtensions) {
    create_file("video.MP4");
    create_file("clip.Mkv");

    auto assets = fx::scan_directory(test_dir);
    EXPECT_EQ(assets.size(), 2);
}

TEST_F(AssetScannerTest, SkipsSubdirectories) {
    create_file("video.mp4");
    create_subdir("subdir");
    create_file("subdir/nested.mp4");

    auto assets = fx::scan_directory(test_dir);
    EXPECT_EQ(assets.size(), 1);
    EXPECT_EQ(assets[0].filename, "video.mp4");
}

TEST_F(AssetScannerTest, ResultsSortedByFilename) {
    create_file("charlie.mp4");
    create_file("alpha.mkv");
    create_file("bravo.avi");

    auto assets = fx::scan_directory(test_dir);
    ASSERT_EQ(assets.size(), 3);
    EXPECT_EQ(assets[0].filename, "alpha.mkv");
    EXPECT_EQ(assets[1].filename, "bravo.avi");
    EXPECT_EQ(assets[2].filename, "charlie.mp4");
}

TEST_F(AssetScannerTest, AllSupportedFormats) {
    create_file("a.mp4");
    create_file("b.avi");
    create_file("c.mkv");
    create_file("d.mov");
    create_file("e.webm");

    auto assets = fx::scan_directory(test_dir);
    EXPECT_EQ(assets.size(), 5);
}
