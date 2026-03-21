#include "src/asset/asset_scanner.h"
#include <algorithm>

namespace fx {

std::vector<Asset> scan_directory(const std::filesystem::path& dir) {
    std::vector<Asset> assets;

    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (!entry.is_regular_file()) continue;

        std::string ext = entry.path().extension().string();
        if (!is_supported_extension(ext)) continue;

        assets.push_back({
            .filename = entry.path().filename().string(),
            .filepath = entry.path(),
            .extension = ext
        });
    }

    std::sort(assets.begin(), assets.end(), [](const Asset& a, const Asset& b) {
        return a.filename < b.filename;
    });

    return assets;
}

} // namespace fx
