#pragma once
#include <opencv2/opencv.hpp>

namespace CreativeEffects {
    cv::Mat pencilSketch(const cv::Mat &src);
    cv::Mat cartoon(const cv::Mat &src);
}