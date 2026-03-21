#pragma once

#include "src/asset/asset.h"
#include <vector>
#include <filesystem>

namespace fx {

std::vector<Asset> scan_directory(const std::filesystem::path& dir);

} // namespace fx
