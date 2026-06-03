#include "Panorama.h"

cv::Mat Panorama::stitch(const std::vector<cv::Mat>& images, bool& success) {
    cv::Mat result;
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
    cv::Stitcher::Status status = stitcher->stitch(images, result);

    if (status != cv::Stitcher::OK) {
        success = false;
        return cv::Mat();
    }

    success = true;
    return result;
}