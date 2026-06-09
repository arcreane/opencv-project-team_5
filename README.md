# OpenCV Image Editor — Team 5

A small image and video editor we built for the Multimedia Application module at ISEP (Spring 2026). It works like a mini GIMP and uses OpenCV under the hood with a Qt 6 graphical interface.

## Team

Camille CHAPTINI, Gabriel PICHOFF, Yan SHEN, Dominik HORSKY.

## What you need

- CMake 3.16 or newer
- Qt 6 (Widgets module)
- OpenCV 4 (we used 4.13)
- A C++17 compiler

On macOS with Homebrew it boils down to:

```
brew install cmake qt opencv
```

On Ubuntu / Debian:

```
sudo apt install cmake qt6-base-dev libopencv-dev
```

## How to build

From the project folder:

```
mkdir build && cd build
cmake ..
cmake --build .
```

Or just open the project in CLion and click the green Run button. CLion picks up the `CMakeLists.txt` on its own.

## How to run

After building, launch the executable:

```
./build/opencv_image_editor
```

A window opens. Use **File > Open** to load an image (`.png`, `.jpg`, `.bmp`, `.tiff`) or a video (`.mp4`, `.mov`, `.avi`, `.mkv`). The processing controls are in the panel on the right. The **"Export the file..."** button at the bottom saves the result, either as an image or as a video depending on what you loaded.

## Features

Mandatory:

- Thresholding (binary, Otsu, adaptive)
- Histogram equalization (global and CLAHE)
- Canny edge detection
- Geometric transforms
- Panorama / stitching
- Morphology (work in progress)

Advanced:

- Video time-lapse and slow-motion

## Sample files

A few test images and a short video are in the `Samples/` folder so you can try the app right after building.
