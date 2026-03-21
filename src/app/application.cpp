#include "src/app/application.h"
#include "src/ui/asset_panel.h"
#include "src/ui/preview_panel.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <chrono>

namespace fx {

bool Application::init(const std::string& window_title) {
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(1280, 720, window_title.c_str(), nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glGenTextures(1, &video_texture_);
    glBindTexture(GL_TEXTURE_2D, video_texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Application::set_assets(std::vector<Asset> assets) {
    assets_ = std::move(assets);
}

void Application::run() {
    AssetPanel asset_panel;
    PreviewPanel preview_panel;

    auto last_frame_time = std::chrono::steady_clock::now();
    double frame_interval = 1.0 / 30.0;

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar
                               | ImGuiWindowFlags_NoResize
                               | ImGuiWindowFlags_NoMove
                               | ImGuiWindowFlags_NoCollapse
                               | ImGuiWindowFlags_NoBringToFrontOnFocus;

        ImGui::Begin("##MainWindow", nullptr, flags);

        float total_width = ImGui::GetContentRegionAvail().x;
        float asset_width = total_width * 0.25f;
        float min_width = 150.0f;
        if (asset_width < min_width) asset_width = min_width;

        ImGui::BeginChild("LeftPanel", ImVec2(asset_width, 0));
        asset_panel.render(assets_, selected_index_);
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("RightPanel", ImVec2(0, 0));

        if (selected_index_ != prev_selected_index_ && selected_index_ >= 0
            && selected_index_ < static_cast<int>(assets_.size())) {
            player_.close();
            error_message_.clear();
            if (player_.open(assets_[selected_index_].filepath)) {
                is_playing_ = true;
                frame_interval = 1.0 / player_.get_frame_rate();
                if (player_.decode_next_frame()) {
                    glBindTexture(GL_TEXTURE_2D, video_texture_);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                                 player_.get_width(), player_.get_height(),
                                 0, GL_RGBA, GL_UNSIGNED_BYTE,
                                 player_.get_frame_data());
                }
            } else {
                error_message_ = "Failed to open video: " + assets_[selected_index_].filename;
                is_playing_ = false;
            }
            prev_selected_index_ = selected_index_;
        }

        if (is_playing_ && player_.is_open()) {
            auto now = std::chrono::steady_clock::now();
            double elapsed = std::chrono::duration<double>(now - last_frame_time).count();
            if (elapsed >= frame_interval) {
                if (player_.decode_next_frame()) {
                    glBindTexture(GL_TEXTURE_2D, video_texture_);
                    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                                    player_.get_width(), player_.get_height(),
                                    GL_RGBA, GL_UNSIGNED_BYTE,
                                    player_.get_frame_data());
                } else {
                    is_playing_ = false;
                }
                last_frame_time = now;
            }
        }

        preview_panel.render(player_, video_texture_, is_playing_, error_message_);
        ImGui::EndChild();

        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window_, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);
    }
}

void Application::shutdown() {
    player_.close();

    if (video_texture_) {
        glDeleteTextures(1, &video_texture_);
        video_texture_ = 0;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    glfwTerminate();
}

} // namespace fx
