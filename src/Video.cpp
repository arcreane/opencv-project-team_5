#include "Video.h"

#include <cmath>

namespace Video {

bool processSpeed(const std::string &inputPath,
                  const std::string &outputPath,
                  double speedFactor) {
    if (speedFactor <= 0.0) return false;

    cv::VideoCapture cap(inputPath);
    if (!cap.isOpened()) return false;

    const double fps = cap.get(cv::CAP_PROP_FPS);
    const int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    const int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    // codec mp4v, FPS de sortie = FPS d'origine pour rester lisible partout
    const int fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
    cv::VideoWriter writer(outputPath, fourcc, fps, cv::Size(width, height));
    if (!writer.isOpened()) return false;

    cv::Mat frame;

    if (speedFactor >= 1.0) {
        // Time-lapse : on garde 1 frame toutes les N frames d'entree
        const int N = std::max(1, static_cast<int>(std::round(speedFactor)));
        int index = 0;
        while (cap.read(frame)) {
            if (index % N == 0) writer.write(frame);
            index++;
        }
    } else {
        // Slow-motion : on duplique chaque frame K fois
        const int K = std::max(1, static_cast<int>(std::round(1.0 / speedFactor)));
        while (cap.read(frame)) {
            for (int i = 0; i < K; ++i) writer.write(frame);
        }
    }

    writer.release();
    cap.release();
    return true;
}

}
