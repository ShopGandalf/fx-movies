#include "src/ui/asset_panel.h"
#include "imgui.h"

namespace fx {

void AssetPanel::render(const std::vector<Asset>& assets, int& selected_index) {
    ImGui::BeginChild("AssetManager", ImVec2(0, 0), ImGuiChildFlags_Borders);
    ImGui::Text("Asset Manager");
    ImGui::Separator();

    if (assets.empty()) {
        ImGui::TextWrapped("No supported video files found");
    } else {
        for (int i = 0; i < static_cast<int>(assets.size()); ++i) {
            bool is_selected = (selected_index == i);
            if (ImGui::Selectable(assets[i].filename.c_str(), is_selected)) {
                selected_index = i;
            }
        }
    }

    ImGui::EndChild();
}

} // namespace fx
