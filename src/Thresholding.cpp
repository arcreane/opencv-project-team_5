#include "Thresholding.h"

namespace processing {

cv::Mat applyThreshold(const cv::Mat &src, int thresholdValue) {
    if (src.empty()) {
        return {};
    }

    //Le seuillage travaille sur l'intensite : on passe en gris.
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src;
    }


    //pixel > seuil  -> 255 (blanc), pixel <= seuil -> 0   (noir)
    cv::Mat result;
    cv::threshold(gray, result, thresholdValue, 255, cv::THRESH_BINARY);

    return result;
}

}
