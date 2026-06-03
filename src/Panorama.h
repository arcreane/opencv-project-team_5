#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>

class Panorama {
public:
    // Assemble plusieurs images en un panorama
    static cv::Mat stitch(const std::vector<cv::Mat>& images, bool& success);
};