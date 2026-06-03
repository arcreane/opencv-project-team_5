#include "GeometricTransforms.h"

cv::Mat GeometricTransforms::applyAffine(const cv::Mat& src,
                                          std::vector<cv::Point2f> srcPoints,
                                          std::vector<cv::Point2f> dstPoints) {
    cv::Mat M = cv::getAffineTransform(srcPoints, dstPoints);
    cv::Mat result;
    cv::warpAffine(src, result, M, src.size());
    return result;
}

cv::Mat GeometricTransforms::applyPerspective(const cv::Mat& src,
                                               std::vector<cv::Point2f> srcPoints,
                                               std::vector<cv::Point2f> dstPoints) {
    cv::Mat M = cv::getPerspectiveTransform(srcPoints, dstPoints);
    cv::Mat result;
    cv::warpPerspective(src, result, M, src.size());
    return result;
}

cv::Mat GeometricTransforms::deskew(const cv::Mat& src, double angle) {
    cv::Point2f center(src.cols / 2.0f, src.rows / 2.0f);
    cv::Mat M = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::Mat result;
    cv::warpAffine(src, result, M, src.size());
    return result;
}