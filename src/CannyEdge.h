#pragma once
#include <opencv2/opencv.hpp>

class CannyEdge {
public:
    static cv::Mat apply(const cv::Mat& src, int threshold1, int threshold2, int apertureSize = 3);
};