#pragma once
#include <opencv2/opencv.hpp>

namespace Enhancement {
    cv::Mat unsharpMask(const cv::Mat &src, double strength = 1.0);
    cv::Mat bilateralDenoise(const cv::Mat &src, int d = 9,
                              double sigmaColor = 75,
                              double sigmaSpace = 75);
}