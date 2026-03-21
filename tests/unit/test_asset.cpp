#include <gtest/gtest.h>
#include "src/asset/asset.h"

TEST(IsSupportedExtension, AcceptsMp4) {
    EXPECT_TRUE(fx::is_supported_extension(".mp4"));
}

TEST(IsSupportedExtension, AcceptsAvi) {
    EXPECT_TRUE(fx::is_supported_extension(".avi"));
}

TEST(IsSupportedExtension, AcceptsMkv) {
    EXPECT_TRUE(fx::is_supported_extension(".mkv"));
}

TEST(IsSupportedExtension, AcceptsMov) {
    EXPECT_TRUE(fx::is_supported_extension(".mov"));
}

TEST(IsSupportedExtension, AcceptsWebm) {
    EXPECT_TRUE(fx::is_supported_extension(".webm"));
}

TEST(IsSupportedExtension, CaseInsensitiveUppercase) {
    EXPECT_TRUE(fx::is_supported_extension(".MP4"));
    EXPECT_TRUE(fx::is_supported_extension(".AVI"));
    EXPECT_TRUE(fx::is_supported_extension(".MKV"));
}

TEST(IsSupportedExtension, CaseInsensitiveMixed) {
    EXPECT_TRUE(fx::is_supported_extension(".Mp4"));
    EXPECT_TRUE(fx::is_supported_extension(".mKv"));
    EXPECT_TRUE(fx::is_supported_extension(".WeBm"));
}

TEST(IsSupportedExtension, RejectsUnsupportedExtensions) {
    EXPECT_FALSE(fx::is_supported_extension(".txt"));
    EXPECT_FALSE(fx::is_supported_extension(".jpg"));
    EXPECT_FALSE(fx::is_supported_extension(".png"));
    EXPECT_FALSE(fx::is_supported_extension(".pdf"));
    EXPECT_FALSE(fx::is_supported_extension(".doc"));
}

TEST(IsSupportedExtension, RejectsEmptyString) {
    EXPECT_FALSE(fx::is_supported_extension(""));
}

TEST(IsSupportedExtension, RejectsNoExtension) {
    EXPECT_FALSE(fx::is_supported_extension("mp4"));
    EXPECT_FALSE(fx::is_supported_extension("video"));
}
