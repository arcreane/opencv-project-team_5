#pragma once

#include <QMainWindow>
#include <QPixmap>
#include <opencv2/opencv.hpp>

class QLabel;
class QScrollArea;
class QSlider;
class QComboBox;

//Fenetre principale de l'editeur d'images.
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void openImage();
    void onThresholdChanged(int value);  //curseur de seuillage deplace
    void onModeChanged(int index);       //changement de mode
    void resetImage();                   //revenir a l'image d'origine
    void onEqualize();                   //egalisation globale
    void onClahe();                      //egalisation CLAHE

private:
    void buildControlPanel();            //construit le panneau lateral
    void applyThresholding();            //applique le mode choisi
    void displayImage(const cv::Mat &image);
    void updateScaledPixmap();

    QLabel *imageLabel_ = nullptr;       //zone d'affichage de l'image
    QScrollArea *scrollArea_ = nullptr;  //conteneur scrollable
    QSlider *thresholdSlider_ = nullptr; //curseur de seuil (0-255)
    QComboBox *thresholdModeCombo_ = nullptr; //mode de seuillage
    QLabel *thresholdValueLabel_ = nullptr; //affiche la valeur du seuil

    cv::Mat originalImage_;              //image chargee, non modifiee
    QPixmap currentPixmap_;              //image affichee, pleine resolution
};
