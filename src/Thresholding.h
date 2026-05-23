#pragma once

#include <opencv2/opencv.hpp>

//Operations de seuillage (thresholding).
namespace processing {

//Applique un seuillage binaire sur une image.
//L'image est d'abord convertie en niveaux de gris : chaque pixel
//devient blanc (255) si son intensite depasse `thresholdValue`,
//noir (0) sinon.
cv::Mat applyThreshold(const cv::Mat &src, int thresholdValue);

//Seuillage automatique : Otsu calcule le seuil tout seul.
cv::Mat applyOtsu(const cv::Mat &src);

}
