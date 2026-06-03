#pragma once

#include <QMainWindow>
#include <QPixmap>
#include <opencv2/opencv.hpp>

class QLabel;
class QScrollArea;
class QSlider;
class QComboBox;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void openImage();
    void onThresholdChanged(int value);
    void onModeChanged(int index);
    void resetImage();
    void onEqualize();
    void onClahe();
    void onCannyApply();
    void onDeskewApply();
    void onPanoramaApply();

private:
    void buildControlPanel();
    void applyThresholding();
    void displayImage(const cv::Mat &image);
    void updateScaledPixmap();

    QLabel *imageLabel_ = nullptr;
    QScrollArea *scrollArea_ = nullptr;
    QSlider *thresholdSlider_ = nullptr;
    QComboBox *thresholdModeCombo_ = nullptr;
    QLabel *thresholdValueLabel_ = nullptr;

    QSlider *cannyThresh1_ = nullptr;
    QSlider *cannyThresh2_ = nullptr;
    QComboBox *cannyApertureCombo_ = nullptr;
    QSlider *deskewAngleSlider_ = nullptr;

    cv::Mat originalImage_;
    QPixmap currentPixmap_;
};