#pragma once

#include <filesystem>
#include <cstdint>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

namespace fx {

class VideoPlayer {
public:
    VideoPlayer() = default;
    ~VideoPlayer();

    VideoPlayer(const VideoPlayer&) = delete;
    VideoPlayer& operator=(const VideoPlayer&) = delete;

    bool open(const std::filesystem::path& filepath);
    bool decode_next_frame();
    void close();

    const uint8_t* get_frame_data() const { return rgba_buffer_; }
    int get_width() const { return width_; }
    int get_height() const { return height_; }
    double get_frame_rate() const { return frame_rate_; }
    bool is_open() const { return format_ctx_ != nullptr; }

private:
    AVFormatContext* format_ctx_ = nullptr;
    AVCodecContext* codec_ctx_ = nullptr;
    SwsContext* sws_ctx_ = nullptr;
    AVFrame* frame_ = nullptr;
    AVFrame* rgba_frame_ = nullptr;
    AVPacket* packet_ = nullptr;
    uint8_t* rgba_buffer_ = nullptr;
    int video_stream_index_ = -1;
    int width_ = 0;
    int height_ = 0;
    double frame_rate_ = 30.0;
};

} // namespace fx
