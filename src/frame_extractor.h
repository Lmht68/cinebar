#ifndef FRAME_EXTRACTOR_H_
#define FRAME_EXTRACTOR_H_

#include "parser.h"

#include <opencv2/opencv.hpp>

namespace app_frame_extractor
{
    constexpr int kNumkMean = 3;                 // Number of clusters for k-means
    constexpr int kTermCriteriaMaxIter = 10;     // Max iterations for k-means
    constexpr double kTermCriteriaEpsilon = 1.0; // Epsilon for k
    constexpr int kKMeansMaxAttempts = 3;

    cv::Mat ExtractFrameStripe(const cv::Mat &frame, int width);
    cv::Vec3b ExtractSmoothedColor(const cv::Mat &frame);
    cv::Vec3b ExtractColorMean(const cv::Mat &frame);
    cv::Vec3b ExtractColorkMeans(const cv::Mat &frame);
    cv::Vec3b ExtractDominantHue(const cv::Mat &frame);

    using ColorFunc = cv::Vec3b (*)(const cv::Mat &);
    using StripeFunc = cv::Mat (*)(const cv::Mat &, int);
    const std::map<cinebar::Method, ColorFunc> kColorExtractorMap{
        {cinebar::Method::Avg, ExtractColorMean},
        {cinebar::Method::Smoothed, ExtractSmoothedColor},
        {cinebar::Method::KMeans, ExtractColorkMeans},
        {cinebar::Method::HSV, ExtractDominantHue}};

    ColorFunc getColorFunction(cinebar::Method method);
    StripeFunc getStripeFunction();
}

#endif