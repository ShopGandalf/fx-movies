#include "ui/help_window.h"

#ifdef IMGUI_VERSION
#include <imgui.h>
#endif

namespace fx {

HelpWindow::HelpWindow(std::string title) : title_(std::move(title)) {}

void HelpWindow::open()  { is_open_ = true;  }
void HelpWindow::close() { is_open_ = false; }
bool HelpWindow::is_open() const { return is_open_; }

std::shared_ptr<ICommand> HelpWindow::open_command() {
    return std::make_shared<RelayCommand>([this]() { open(); });
}

void HelpWindow::render() {
    if (!is_open_) return;

#ifdef IMGUI_VERSION
    ImGui::SetNextWindowSize(ImVec2(420, 280), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(title_.c_str(), &is_open_)) {
        ImGui::SeparatorText("fx-movies");
        ImGui::Spacing();
        ImGui::TextWrapped("Use the file browser on the left to select a video.");
        ImGui::TextWrapped("Press Play / Pause to control playback.");
        ImGui::Spacing();
        ImGui::SeparatorText("Keyboard shortcuts");
        ImGui::TextUnformatted("F1   Show this window");
        ImGui::Spacing();
        if (ImGui::Button("Close", ImVec2(80, 0))) close();
    }
    ImGui::End();
#endif
}

} // namespace fx
