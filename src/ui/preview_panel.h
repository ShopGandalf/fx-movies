#pragma once

#include "src/video/video_player.h"
#include <string>

namespace fx {

class PreviewPanel {
public:
    void render(VideoPlayer& player, unsigned int texture_id,
                bool& is_playing, const std::string& error_message);
};

} // namespace fx
