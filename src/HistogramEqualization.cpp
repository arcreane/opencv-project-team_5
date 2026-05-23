#include "HistogramEqualization.h"

#include <vector>

namespace processing {

cv::Mat equalize(const cv::Mat &src) {
    if (src.empty()) {
        return {};
    }

    //image deja en gris : egalisation directe
    if (src.channels() == 1) {
        cv::Mat result;
        cv::equalizeHist(src, result);
        return result;
    }

    //image couleur : on separe la luminosite des couleurs
    cv::Mat ycrcb;
    cv::cvtColor(src, ycrcb, cv::COLOR_BGR2YCrCb);

    std::vector<cv::Mat> channels;
    cv::split(ycrcb, channels);

    //le canal Y porte la luminosite : on egalise seulement lui
    cv::equalizeHist(channels[0], channels[0]);

    cv::merge(channels, ycrcb);

    cv::Mat result;
    cv::cvtColor(ycrcb, result, cv::COLOR_YCrCb2BGR);
    return result;
}

cv::Mat applyClahe(const cv::Mat &src) {
    if (src.empty()) {
        return {};
    }

    //objet CLAHE : limite de contraste 2.0, grille de 8x8 tuiles
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));

    //image deja en gris : CLAHE directement
    if (src.channels() == 1) {
        cv::Mat result;
        clahe->apply(src, result);
        return result;
    }

    //image couleur : CLAHE sur la luminosite seulement
    cv::Mat ycrcb;
    cv::cvtColor(src, ycrcb, cv::COLOR_BGR2YCrCb);

    std::vector<cv::Mat> channels;
    cv::split(ycrcb, channels);

    clahe->apply(channels[0], channels[0]);

    cv::merge(channels, ycrcb);

    cv::Mat result;
    cv::cvtColor(ycrcb, result, cv::COLOR_YCrCb2BGR);
    return result;
}

}
