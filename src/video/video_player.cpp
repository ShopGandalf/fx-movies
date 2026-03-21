#include "src/video/video_player.h"

namespace fx {

VideoPlayer::~VideoPlayer() {
    close();
}

bool VideoPlayer::open(const std::filesystem::path& filepath) {
    close();

    std::string path_str = filepath.string();

    if (avformat_open_input(&format_ctx_, path_str.c_str(), nullptr, nullptr) < 0) {
        close();
        return false;
    }

    if (avformat_find_stream_info(format_ctx_, nullptr) < 0) {
        close();
        return false;
    }

    video_stream_index_ = -1;
    for (unsigned i = 0; i < format_ctx_->nb_streams; ++i) {
        if (format_ctx_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index_ = static_cast<int>(i);
            break;
        }
    }

    if (video_stream_index_ < 0) {
        close();
        return false;
    }

    auto* codecpar = format_ctx_->streams[video_stream_index_]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);
    if (!codec) {
        close();
        return false;
    }

    codec_ctx_ = avcodec_alloc_context3(codec);
    if (!codec_ctx_) {
        close();
        return false;
    }

    if (avcodec_parameters_to_context(codec_ctx_, codecpar) < 0) {
        close();
        return false;
    }

    if (avcodec_open2(codec_ctx_, codec, nullptr) < 0) {
        close();
        return false;
    }

    width_ = codec_ctx_->width;
    height_ = codec_ctx_->height;

    auto r_rate = format_ctx_->streams[video_stream_index_]->r_frame_rate;
    if (r_rate.den > 0) {
        frame_rate_ = static_cast<double>(r_rate.num) / r_rate.den;
    }

    sws_ctx_ = sws_getContext(
        width_, height_, codec_ctx_->pix_fmt,
        width_, height_, AV_PIX_FMT_RGBA,
        SWS_BILINEAR, nullptr, nullptr, nullptr
    );
    if (!sws_ctx_) {
        close();
        return false;
    }

    frame_ = av_frame_alloc();
    rgba_frame_ = av_frame_alloc();
    packet_ = av_packet_alloc();

    if (!frame_ || !rgba_frame_ || !packet_) {
        close();
        return false;
    }

    int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_RGBA, width_, height_, 1);
    rgba_buffer_ = static_cast<uint8_t*>(av_malloc(buffer_size));
    av_image_fill_arrays(rgba_frame_->data, rgba_frame_->linesize,
                         rgba_buffer_, AV_PIX_FMT_RGBA, width_, height_, 1);

    return true;
}

bool VideoPlayer::decode_next_frame() {
    if (!format_ctx_ || !codec_ctx_) return false;

    while (av_read_frame(format_ctx_, packet_) >= 0) {
        if (packet_->stream_index == video_stream_index_) {
            int ret = avcodec_send_packet(codec_ctx_, packet_);
            av_packet_unref(packet_);

            if (ret < 0) return false;

            ret = avcodec_receive_frame(codec_ctx_, frame_);
            if (ret == AVERROR(EAGAIN)) continue;
            if (ret < 0) return false;

            sws_scale(sws_ctx_,
                      frame_->data, frame_->linesize,
                      0, height_,
                      rgba_frame_->data, rgba_frame_->linesize);

            return true;
        }
        av_packet_unref(packet_);
    }

    return false;
}

void VideoPlayer::close() {
    if (rgba_buffer_) {
        av_free(rgba_buffer_);
        rgba_buffer_ = nullptr;
    }
    if (packet_) {
        av_packet_free(&packet_);
        packet_ = nullptr;
    }
    if (rgba_frame_) {
        av_frame_free(&rgba_frame_);
        rgba_frame_ = nullptr;
    }
    if (frame_) {
        av_frame_free(&frame_);
        frame_ = nullptr;
    }
    if (sws_ctx_) {
        sws_freeContext(sws_ctx_);
        sws_ctx_ = nullptr;
    }
    if (codec_ctx_) {
        avcodec_free_context(&codec_ctx_);
        codec_ctx_ = nullptr;
    }
    if (format_ctx_) {
        avformat_close_input(&format_ctx_);
        format_ctx_ = nullptr;
    }
    video_stream_index_ = -1;
    width_ = 0;
    height_ = 0;
}

} // namespace fx
