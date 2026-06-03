#pragma once
#include <opencv2/opencv.hpp>

class GeometricTransforms {
public:
    // Transformation affine à partir de 3 points
    static cv::Mat applyAffine(const cv::Mat& src,
                                std::vector<cv::Point2f> srcPoints,
                                std::vector<cv::Point2f> dstPoints);

    // Transformation perspective à partir de 4 points
    static cv::Mat applyPerspective(const cv::Mat& src,
                                     std::vector<cv::Point2f> srcPoints,
                                     std::vector<cv::Point2f> dstPoints);

    // Redresser une image (rotation)
    static cv::Mat deskew(const cv::Mat& src, double angle);
};