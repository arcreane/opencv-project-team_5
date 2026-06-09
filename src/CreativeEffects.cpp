#include "CreativeEffects.h"

namespace CreativeEffects {

cv::Mat pencilSketch(const cv::Mat &src) {
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    cv::Mat inverted;
    cv::bitwise_not(gray, inverted);

    cv::Mat blurred;
    cv::GaussianBlur(inverted, blurred, cv::Size(21, 21), 0);

    cv::Mat sketch;
    cv::divide(gray, 255 - blurred, sketch, 256.0);

    return sketch;
}

cv::Mat cartoon(const cv::Mat &src) {
    cv::Mat smoothed = src.clone();
    for (int i = 0; i < 2; i++) {
        cv::Mat tmp;
        cv::bilateralFilter(smoothed, tmp, 9, 75, 75);
        smoothed = tmp;
    }

    cv::Mat gray, edges;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, gray, 7);
    cv::adaptiveThreshold(gray, edges,
        255,
        cv::ADAPTIVE_THRESH_MEAN_C,
        cv::THRESH_BINARY,
        9, 2);

    cv::Mat edgesBGR;
    cv::cvtColor(edges, edgesBGR, cv::COLOR_GRAY2BGR);

    cv::Mat result;
    cv::bitwise_and(smoothed, edgesBGR, result);
    return result;
}

}