#include "Morphology.h"

namespace Morphology {

static cv::Mat getKernel(int size) {
    return cv::getStructuringElement(
        cv::MORPH_RECT,
        cv::Size(size, size));
}

cv::Mat dilate(const cv::Mat &src, int kernelSize) {
    cv::Mat result;
    cv::dilate(src, result, getKernel(kernelSize));
    return result;
}

cv::Mat erode(const cv::Mat &src, int kernelSize) {
    cv::Mat result;
    cv::erode(src, result, getKernel(kernelSize));
    return result;
}

cv::Mat opening(const cv::Mat &src, int kernelSize) {
    cv::Mat result;
    cv::morphologyEx(src, result, cv::MORPH_OPEN, getKernel(kernelSize));
    return result;
}

cv::Mat closing(const cv::Mat &src, int kernelSize) {
    cv::Mat result;
    cv::morphologyEx(src, result, cv::MORPH_CLOSE, getKernel(kernelSize));
    return result;
}

cv::Mat gradient(const cv::Mat &src, int kernelSize) {
    cv::Mat result;
    cv::morphologyEx(src, result, cv::MORPH_GRADIENT, getKernel(kernelSize));
    return result;
}

}