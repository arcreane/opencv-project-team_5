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

}
