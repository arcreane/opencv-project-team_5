#include "CannyEdge.h"

cv::Mat CannyEdge::apply(const cv::Mat& src, int threshold1, int threshold2, int apertureSize) {
    cv::Mat gray, edges;

    if (src.channels() == 3)
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    else
        gray = src.clone();

    cv::Canny(gray, edges, threshold1, threshold2, apertureSize);

    cv::Mat result;
    cv::cvtColor(edges, result, cv::COLOR_GRAY2BGR);
    return result;
}