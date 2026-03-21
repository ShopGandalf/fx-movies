#pragma once

#include "src/asset/asset.h"
#include <vector>

namespace fx {

class AssetPanel {
public:
    void render(const std::vector<Asset>& assets, int& selected_index);
};

} // namespace fx
