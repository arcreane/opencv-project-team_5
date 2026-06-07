#pragma once

#include <opencv2/opencv.hpp>
#include <string>

namespace Video {

// Change la vitesse d'une video.
//   speedFactor > 1.0 : time-lapse (acceleration)
//   speedFactor < 1.0 : slow-motion (ralenti)
//   speedFactor = 1.0 : copie a vitesse normale
// Retourne true si l'export a reussi.
bool processSpeed(const std::string &inputPath,
                  const std::string &outputPath,
                  double speedFactor);

}
