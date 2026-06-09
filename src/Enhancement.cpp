#include "Enhancement.h"

namespace Enhancement {

cv::Mat unsharpMask(const cv::Mat &src, double strength) {
    cv::Mat blurred;
    cv::GaussianBlur(src, blurred, cv::Size(0, 0), 3);

    cv::Mat sharpened;
    cv::addWeighted(src, 1.0 + strength, blurred, -strength, 0, sharpened);
    return sharpened;
}

cv::Mat bilateralDenoise(const cv::Mat &src, int d,
                          double sigmaColor, double sigmaSpace) {
    cv::Mat result;
    cv::bilateralFilter(src, result, d, sigmaColor, sigmaSpace);
    return result;
}

}