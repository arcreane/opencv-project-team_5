#pragma once

#include <opencv2/opencv.hpp>

namespace processing {
cv::Mat equalize(const cv::Mat &src);

//CLAHE : egalisation adaptative
cv::Mat applyClahe(const cv::Mat &src);

}
