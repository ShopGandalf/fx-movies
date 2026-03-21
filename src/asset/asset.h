#pragma once

#include <filesystem>
#include <string>
#include <algorithm>

namespace fx {

struct Asset {
    std::string filename;
    std::filesystem::path filepath;
    std::string extension;
};

inline bool is_supported_extension(const std::string& ext) {
    if (ext.empty()) return false;

    std::string lower = ext;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    return lower == ".mp4" || lower == ".avi" || lower == ".mkv"
        || lower == ".mov" || lower == ".webm";
}

} // namespace fx
