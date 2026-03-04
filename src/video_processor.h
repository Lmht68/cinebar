#ifndef VIDEO_PROCESSOR_H_
#define VIDEO_PROCESSOR_H_

#include <string>

#include <opencv2/videoio.hpp>

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
}

#endif