#pragma once

#include <opencv2/opencv.hpp>

namespace processing {
cv::Mat equalize(const cv::Mat &src);

//CLAHE : egalisation adaptative, tuile par tuile, avec limite de contraste.
cv::Mat applyClahe(const cv::Mat &src);

}
