cmake_minimum_required(VERSION 3.16)
project(opencv_image_editor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Homebrew (Apple Silicon) install locations
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)

add_executable(opencv_image_editor src/main.cpp)

target_include_directories(opencv_image_editor PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(opencv_image_editor PRIVATE Qt6::Widgets ${OpenCV_LIBS})
