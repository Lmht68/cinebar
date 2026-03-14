#ifndef VIDEO_PROCESSOR_H_
#define VIDEO_PROCESSOR_H_

#include <opencv2/videoio.hpp>

#include <string>
#include <vector>
#include <optional>

namespace app_video_processor
{
	struct VideoInfo
	{
		cv::VideoCapture capture;
		int frame_count;
		double fps;
		int width;
		int height;
	};

	VideoInfo LoadVideoInfo(const std::string &video_path);
	int GetFrameCountFromInterval(int frame_count, double fps, double interval);
	int NframesFromInterval(const VideoInfo &video_info, double interval);

	// Represents the detected bounds of the actual video content, excluding letterboxing
	struct VideoBounds
	{
		int left;
		int top;
		int right;
		int bottom;
	};
	constexpr double kDownScaleFactor = 0.25; // 1/4 size for faster processing
	constexpr double kMinCropRatio = 0.97;	  // Ignore crops that keep more than 97% of the original frame

	std::optional<VideoBounds> DetectBounds(const cv::Mat &frame_grayed,
											int threshold = 16,
											double min_black_ratio = 0.98);
	bool DetermineVideoBounds(const std::string &source,
							  VideoBounds &bounds,
							  int n_samples = 10);
	cv::Mat CropImage(const cv::Mat &frame, const VideoBounds &bounds);
}

#endif