#include "../src/video_processor.h"

#include <gtest/gtest.h>
#include <opencv2/core/utils/logger.hpp>

class OpenCVLogSilencer
{
public:
    OpenCVLogSilencer()
    {
        cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    }
};

static OpenCVLogSilencer silence;

TEST(VideoProcessorTest, LoadVideoInfo_NonExistentFile)
{
    EXPECT_THROW(
        app_video_processor::LoadVideoInfo("nonexistent_file.mp4"),
        std::runtime_error);
}

TEST(VideoProcessorTest, GetFrameCount_InvalidInterval)
{
    EXPECT_THROW(
        app_video_processor::GetFrameCountFromInterval(100, 30.0, 0.0),
        std::invalid_argument);
}

TEST(VideoProcessorTest, GetFrameCount_InvalidFPS)
{
    EXPECT_THROW(
        app_video_processor::GetFrameCountFromInterval(100, 0.0, 1.0),
        std::runtime_error);
}

TEST(VideoProcessorTest, GetFrameCount_IntervalTooSmall)
{
    // frame_count = 3000; arbitrary valid
    // fps = 30.0;         30 FPS
    // interval = 0.01;    too small (100 FPS sampling)

    EXPECT_THROW(
        app_video_processor::GetFrameCountFromInterval(3000, 30.0, 0.01),
        std::invalid_argument);
}

TEST(VideoProcessorTest, GetFrameCount_InvalidFrameCount)
{
    EXPECT_THROW(
        app_video_processor::GetFrameCountFromInterval(0, 30.0, 1.0),
        std::runtime_error);
}

TEST(VideoProcessorTest, GetFrameCount_ComputesCorrectValue)
{
    // 300 frames at 30 fps = 10 seconds
    // interval = 2 seconds
    // expected = round(10 / 2) = 5
    int result = app_video_processor::GetFrameCountFromInterval(300, 30.0, 2.0);
    EXPECT_EQ(result, 5);
}

TEST(VideoProcessorTest, GetFrameCount_RoundsCorrectly)
{
    // 100 frames at 30 fps = 3.3333 sec
    // interval = 1 sec
    // 3.3333 -> round -> 3
    int result = app_video_processor::GetFrameCountFromInterval(100, 30.0, 1.0);
    EXPECT_EQ(result, 3);
}

TEST(VideoProcessorTest, InvalidInterval)
{
    app_video_processor::VideoInfo info{
        {},   // capture (not used in this function)
        300,  // frame_count
        30.0, // fps
        1920,
        1080};

    EXPECT_THROW(
        app_video_processor::NframesFromInterval(info, 0.0),
        std::invalid_argument);
}

TEST(VideoProcessorTest, InvalidFrameCount)
{
    app_video_processor::VideoInfo info{
        {},
        0, // invalid frame_count
        30.0,
        1920,
        1080};

    EXPECT_THROW(
        app_video_processor::NframesFromInterval(info, 1.0),
        std::runtime_error);
}