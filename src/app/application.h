#pragma once

#include "src/asset/asset.h"
#include "src/video/video_player.h"
#include <vector>
#include <string>

struct GLFWwindow;

namespace fx {

class Application {
public:
    bool init(const std::string& window_title);
    void set_assets(std::vector<Asset> assets);
    void run();
    void shutdown();

private:
    GLFWwindow* window_ = nullptr;
    std::vector<Asset> assets_;
    int selected_index_ = -1;
    int prev_selected_index_ = -1;
    VideoPlayer player_;
    bool is_playing_ = false;
    std::string error_message_;
    unsigned int video_texture_ = 0;
};

} // namespace fx
