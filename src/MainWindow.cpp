#include "MainWindow.h"

#include <QAction>
#include <QComboBox>
#include <QDockWidget>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

#include "HistogramEqualization.h"
#include "Thresholding.h"

static QImage matToQImage(const cv::Mat &mat) {
    if (mat.empty()) {
        return {};
    }
    //Image en niveaux de gris (1 canal).
    if (mat.type() == CV_8UC1) {
        return QImage(mat.data, mat.cols, mat.rows,
                      static_cast<int>(mat.step),
                      QImage::Format_Grayscale8)
            .copy();
    }
    //Image couleur (3 canaux BGR -> RGB).
    if (mat.type() == CV_8UC3) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        return QImage(rgb.data, rgb.cols, rgb.rows,
                      static_cast<int>(rgb.step),
                      QImage::Format_RGB888)
            .copy();
    }
    return {};
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("OpenCV Image Editor - Team 5");
    resize(1000, 700);

    //Zone d'affichage de l'image, dans une zone scrollable.
    imageLabel_ = new QLabel("Ouvre une image avec  Fichier > Ouvrir");
    imageLabel_->setAlignment(Qt::AlignCenter);

    scrollArea_ = new QScrollArea;
    scrollArea_->setWidget(imageLabel_);
    scrollArea_->setWidgetResizable(true);
    setCentralWidget(scrollArea_);

    //Menu Fichier > Ouvrir
    QMenu *fileMenu = menuBar()->addMenu("Fichier");
    QAction *openAction = fileMenu->addAction("Ouvrir...");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openImage);

    //Panneau lateral avec les controles de traitement.
    buildControlPanel();
}

void MainWindow::buildControlPanel() {
    auto *dock = new QDockWidget("Traitements", this);
    //panneau non fermable, pour ne pas le perdre
    dock->setFeatures(QDockWidget::DockWidgetMovable |
                      QDockWidget::DockWidgetFloatable);
    auto *panel = new QWidget;
    auto *layout = new QVBoxLayout(panel);

    //Bloc Seuillage
    auto *title = new QLabel("<b>Seuillage</b>");

    //choix du mode
    thresholdModeCombo_ = new QComboBox;
    thresholdModeCombo_->addItem("Binaire");
    thresholdModeCombo_->addItem("Otsu (auto)");
    thresholdModeCombo_->addItem("Adaptatif (local)");

    thresholdValueLabel_ = new QLabel("Seuil : 127");

    thresholdSlider_ = new QSlider(Qt::Horizontal);
    thresholdSlider_->setRange(0, 255);
    thresholdSlider_->setValue(127);

    auto *resetButton = new QPushButton("Reinitialiser");

    layout->addWidget(title);
    layout->addWidget(thresholdModeCombo_);
    layout->addWidget(thresholdValueLabel_);
    layout->addWidget(thresholdSlider_);
    layout->addWidget(resetButton);

    //Bloc Egalisation d'histogramme
    auto *eqTitle = new QLabel("<b>Egalisation d'histogramme</b>");
    auto *equalizeButton = new QPushButton("Globale");
    auto *claheButton = new QPushButton("CLAHE (local)");
    layout->addWidget(eqTitle);
    layout->addWidget(equalizeButton);
    layout->addWidget(claheButton);
    layout->addStretch();  // pousse les controles vers le haut

    dock->setWidget(panel);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    //applique le seuillage en direct ;
    connect(thresholdSlider_, &QSlider::valueChanged,
            this, &MainWindow::onThresholdChanged);
    //restaure l'image d'origine.
    connect(resetButton, &QPushButton::clicked,
            this, &MainWindow::resetImage);

    //change de mode
    connect(thresholdModeCombo_, &QComboBox::currentIndexChanged,
            this, &MainWindow::onModeChanged);

    //egalise l'histogramme
    connect(equalizeButton, &QPushButton::clicked,
            this, &MainWindow::onEqualize);
    connect(claheButton, &QPushButton::clicked,
            this, &MainWindow::onClahe);
}

void MainWindow::openImage() {
    const QString path = QFileDialog::getOpenFileName(
        this, "Ouvrir une image", QString(),
        "Images (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)");
    if (path.isEmpty()) {
        return;
    }

    cv::Mat image = cv::imread(path.toStdString(), cv::IMREAD_COLOR);
    if (image.empty()) {
        QMessageBox::warning(this, "Erreur",
                             "Impossible de charger cette image.");
        return;
    }

    originalImage_ = image;
    displayImage(originalImage_);
}

void MainWindow::onThresholdChanged(int value) {
    thresholdValueLabel_->setText(QString("Seuil : %1").arg(value));
    applyThresholding();
}

void MainWindow::onModeChanged(int index) {
    //Otsu trouve le seuil tout seul, le curseur ne sert plus
    thresholdSlider_->setEnabled(index == 0);
    applyThresholding();
}

void MainWindow::applyThresholding() {
    if (originalImage_.empty()) {
        return;  //pas d'image chargee
    }
    cv::Mat result;
    if (thresholdModeCombo_->currentIndex() == 1) {
        result = processing::applyOtsu(originalImage_);
    } else if (thresholdModeCombo_->currentIndex() == 2) {
        result = processing::applyAdaptive(originalImage_);
    } else {
        result = processing::applyThreshold(originalImage_,
                                            thresholdSlider_->value());
    }
    displayImage(result);
}

void MainWindow::resetImage() {
    if (originalImage_.empty()) {
        return;
    }
    displayImage(originalImage_);
}

void MainWindow::onEqualize() {
    if (originalImage_.empty()) {
        return;  //pas d'image chargee
    }
    const cv::Mat result = processing::equalize(originalImage_);
    displayImage(result);
}

void MainWindow::onClahe() {
    if (originalImage_.empty()) {
        return;  //pas d'image chargee
    }
    const cv::Mat result = processing::applyClahe(originalImage_);
    displayImage(result);
}

void MainWindow::displayImage(const cv::Mat &image) {
    currentPixmap_ = QPixmap::fromImage(matToQImage(image));
    updateScaledPixmap();
}

void MainWindow::updateScaledPixmap() {
    if (currentPixmap_.isNull()) {
        return;
    }
    //Redimensionne l'image, en conservant les proportions
    const QSize viewSize = scrollArea_->viewport()->size();
    imageLabel_->setPixmap(currentPixmap_.scaled(
        viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    updateScaledPixmap();
}
