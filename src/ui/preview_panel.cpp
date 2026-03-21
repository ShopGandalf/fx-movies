#include "src/ui/preview_panel.h"
#include "imgui.h"
#include <cstdint>

namespace fx {

void PreviewPanel::render(VideoPlayer& player, unsigned int texture_id,
                          bool& is_playing, const std::string& error_message) {
    ImGui::BeginChild("VideoPreview", ImVec2(0, 0), ImGuiChildFlags_Borders);
    ImGui::Text("Video Preview");
    ImGui::Separator();

    if (!error_message.empty()) {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "%s", error_message.c_str());
    } else if (!player.is_open()) {
        ImGui::TextWrapped("Select a video to preview");
    } else {
        float avail_w = ImGui::GetContentRegionAvail().x;
        float avail_h = ImGui::GetContentRegionAvail().y - 40.0f;

        float video_w = static_cast<float>(player.get_width());
        float video_h = static_cast<float>(player.get_height());
        float scale = std::min(avail_w / video_w, avail_h / video_h);
        if (scale > 0.0f) {
            float display_w = video_w * scale;
            float display_h = video_h * scale;

            float offset_x = (avail_w - display_w) * 0.5f;
            if (offset_x > 0.0f) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset_x);

            ImGui::Image(static_cast<ImTextureID>(static_cast<uintptr_t>(texture_id)),
                         ImVec2(display_w, display_h));
        }

        float button_width = 100.0f;
        float offset = (avail_w - button_width) * 0.5f;
        if (offset > 0.0f) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);

        if (is_playing) {
            if (ImGui::Button("Pause", ImVec2(button_width, 0))) {
                is_playing = false;
            }
        } else {
            if (ImGui::Button("Play", ImVec2(button_width, 0))) {
                is_playing = true;
            }
        }
    }

    ImGui::EndChild();
}

} // namespace fx
