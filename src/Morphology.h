#pragma once
#include <opencv2/opencv.hpp>

namespace Morphology {
    cv::Mat dilate(const cv::Mat &src, int kernelSize);
    cv::Mat erode(const cv::Mat &src, int kernelSize);
    cv::Mat opening(const cv::Mat &src, int kernelSize);
    cv::Mat closing(const cv::Mat &src, int kernelSize);
    cv::Mat gradient(const cv::Mat &src, int kernelSize);
}