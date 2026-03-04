#include "video_processor.h"

#include <opencv2/opencv.hpp>

#include <stdexcept>
#include <cmath>

namespace app_video_processor
{
    VideoInfo LoadVideoInfo(const std::string &video_path)
    {
        cv::VideoCapture video(video_path, cv::CAP_FFMPEG);

        if (!video.isOpened())
            throw std::runtime_error("video_processor: Input video not found: " + video_path);

        int frame_count = static_cast<int>(video.get(cv::CAP_PROP_FRAME_COUNT));
        double fps = video.get(cv::CAP_PROP_FPS);
        // video.release(); // Not required (RAII handles it)
        int width = static_cast<int>(video.get(cv::CAP_PROP_FRAME_WIDTH));
        int height = static_cast<int>(video.get(cv::CAP_PROP_FRAME_HEIGHT));

        return {std::move(video), frame_count, fps, width, height};
    }

    int GetFrameCountFromInterval(int frame_count, double fps, double interval)
    {
        if (interval <= 0.0)
            throw std::invalid_argument("video_processor: Interval must be greater than 0");

        if (fps <= 0.0)
            throw std::runtime_error("video_processor: Invalid FPS value");

        if (interval < 1.0 / fps)
            throw std::invalid_argument("video_processor: Interval too small for video FPS");

        if (frame_count <= 0.0)
            throw std::runtime_error("video_processor: Invalid frame count value");

        double duration = frame_count / fps;
        return static_cast<int>(std::round(duration / interval));
    }

    int NframesFromInterval(const VideoInfo &video_info, double interval)
    {
        if (interval <= 0.0)
            throw std::invalid_argument("video_processor: Interval must be greater than 0");

        return GetFrameCountFromInterval(video_info.frame_count, video_info.fps, interval);
    }
}