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

cv::Mat applyOtsu(const cv::Mat &src) {
    if (src.empty()) {
        return {};
    }

    //Otsu travaille aussi sur le gris.
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src;
    }

    //le 0 est ignore, Otsu trouve le seuil
    cv::Mat result;
    cv::threshold(gray, result, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    return result;
}

cv::Mat applyAdaptive(const cv::Mat &src) {
    if (src.empty()) {
        return {};
    }

    //le mode adaptatif a besoin du gris.
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src;
    }

    //un seuil different pour chaque petite zone (11x11)
    cv::Mat result;
    cv::adaptiveThreshold(gray, result, 255,
                          cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                          cv::THRESH_BINARY, 11, 2);

    return result;
}

}
