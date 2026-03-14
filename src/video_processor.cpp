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

    std::optional<VideoBounds> DetectBounds(const cv::Mat &frame_grayed,
                                            int threshold,
                                            double min_black_ratio)
    {
        // downscale for faster processing
        cv::Mat frame_downscaled;
        cv::resize(frame_grayed, frame_downscaled, cv::Size(), kDownScaleFactor, kDownScaleFactor, cv::INTER_AREA);

        int h_downscaled = frame_downscaled.rows;
        int w_downscaled = frame_downscaled.cols;

        std::vector<double> rowBlackRatio(h_downscaled, 0.0);
        std::vector<double> colBlackRatio(w_downscaled, 0.0);

        // count black pixels
        for (int y = 0; y < h_downscaled; y++)
        {
            for (int x = 0; x < w_downscaled; x++)
            {
                if (frame_downscaled.at<uchar>(y, x) < threshold)
                {
                    rowBlackRatio[y] += 1;
                    colBlackRatio[x] += 1;
                }
            }
        }

        for (int y = 0; y < h_downscaled; y++)
            rowBlackRatio[y] /= w_downscaled;

        for (int x = 0; x < w_downscaled; x++)
            colBlackRatio[x] /= h_downscaled;

        std::vector<bool> blackRows(h_downscaled);
        std::vector<bool> blackCols(w_downscaled);

        for (int i = 0; i < h_downscaled; i++)
            blackRows[i] = rowBlackRatio[i] > min_black_ratio;

        for (int i = 0; i < w_downscaled; i++)
            blackCols[i] = colBlackRatio[i] > min_black_ratio;

        int top = 0;
        while (top < h_downscaled && blackRows[top])
            top++;

        int bottom = h_downscaled - 1;
        while (bottom >= 0 && blackRows[bottom])
            bottom--;

        int left = 0;
        while (left < w_downscaled && blackCols[left])
            left++;

        int right = w_downscaled - 1;
        while (right >= 0 && blackCols[right])
            right--;

        if (left >= right || top >= bottom)
            return std::nullopt;

        VideoBounds bounds{
            static_cast<int>(left / kDownScaleFactor),
            static_cast<int>(top / kDownScaleFactor),
            static_cast<int>(right / kDownScaleFactor),
            static_cast<int>(bottom / kDownScaleFactor)};

        return bounds;
    }

    bool DetermineVideoBounds(const std::string &source,
                              VideoBounds &bounds,
                              int n_samples)
    {
        cv::VideoCapture cap(source);

        if (!cap.isOpened())
            throw std::runtime_error("video_processor: Cannot open video for trimming: " + source);

        int totalFrames = static_cast<int>(
            cap.get(cv::CAP_PROP_FRAME_COUNT));

        double interval = static_cast<double>(totalFrames) / n_samples;
        std::vector<VideoBounds> detections;
        cv::Mat frame;

        for (int i = 0; i < n_samples; i++)
        {
            int frame_id = static_cast<int>((i + 0.5) * interval);
            cap.set(cv::CAP_PROP_POS_FRAMES, frame_id);
            if (!cap.read(frame))
                continue;
            cv::Mat frame_grayed;
            cv::cvtColor(frame, frame_grayed, cv::COLOR_BGR2GRAY);
            auto bounds = DetectBounds(frame_grayed);
            if (bounds)
                detections.push_back(*bounds);
        }

        cap.release();
        if (detections.empty())
            return false;

        // collect arrays for median
        std::vector<int> lefts, tops, rights, bottoms;

        for (auto &b : detections)
        {
            lefts.push_back(b.left);
            tops.push_back(b.top);
            rights.push_back(b.right);
            bottoms.push_back(b.bottom);
        }

        auto median = [](std::vector<int> &v)
        {
            std::sort(v.begin(), v.end());
            return v[v.size() / 2];
        };

        VideoBounds median_bounds{
            median(lefts),
            median(tops),
            median(rights),
            median(bottoms)};
        int crop_w = median_bounds.right - median_bounds.left;
        int crop_h = median_bounds.bottom - median_bounds.top;
        if (crop_w > frame.cols * kMinCropRatio && crop_h > frame.rows * kMinCropRatio)
            return false;

        bounds = median_bounds;
        return true;
    }

    cv::Mat CropImage(const cv::Mat &frame, const VideoBounds &bounds)
    {
        cv::Rect roi(
            bounds.left,
            bounds.top,
            bounds.right - bounds.left,
            bounds.bottom - bounds.top);

        return frame(roi);
    }
}