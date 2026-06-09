#include "MainWindow.h"

#include <QAction>
#include <QComboBox>
#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
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
#include "CannyEdge.h"
#include "GeometricTransforms.h"
#include "Panorama.h"
#include "Video.h"
#include "CreativeEffects.h"
#include "Enhancement.h"
#include "Morphology.h"

static QImage matToQImage(const cv::Mat &mat) {
    if (mat.empty()) return {};
    if (mat.type() == CV_8UC1) {
        return QImage(mat.data, mat.cols, mat.rows,
                      static_cast<int>(mat.step),
                      QImage::Format_Grayscale8).copy();
    }
    if (mat.type() == CV_8UC3) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        return QImage(rgb.data, rgb.cols, rgb.rows,
                      static_cast<int>(rgb.step),
                      QImage::Format_RGB888).copy();
    }
    return {};
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("OpenCV Image Editor - Team 5");
    resize(1000, 700);

    imageLabel_ = new QLabel("Ouvre une image avec Fichier > Ouvrir");
    imageLabel_->setAlignment(Qt::AlignCenter);

    scrollArea_ = new QScrollArea;
    scrollArea_->setWidget(imageLabel_);
    scrollArea_->setWidgetResizable(true);
    setCentralWidget(scrollArea_);

    QMenu *fileMenu = menuBar()->addMenu("Fichier");
    QAction *openAction = fileMenu->addAction("Ouvrir...");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openImage);

    buildControlPanel();
}

void MainWindow::buildControlPanel() {
    auto *dock = new QDockWidget("Traitements", this);
    dock->setFeatures(QDockWidget::DockWidgetMovable |
                      QDockWidget::DockWidgetFloatable);
    auto *panel = new QWidget;
    auto *layout = new QVBoxLayout(panel);


    layout->addWidget(new QLabel("<b>Seuillage</b>"));
    thresholdModeCombo_ = new QComboBox;
    thresholdModeCombo_->addItem("Binaire");
    thresholdModeCombo_->addItem("Otsu (auto)");
    thresholdModeCombo_->addItem("Adaptatif (local)");
    thresholdValueLabel_ = new QLabel("Seuil : 127");
    thresholdSlider_ = new QSlider(Qt::Horizontal);
    thresholdSlider_->setRange(0, 255);
    thresholdSlider_->setValue(127);
    auto *resetButton = new QPushButton("Reinitialiser");
    layout->addWidget(thresholdModeCombo_);
    layout->addWidget(thresholdValueLabel_);
    layout->addWidget(thresholdSlider_);
    layout->addWidget(resetButton);

    // Histogramme
    layout->addWidget(new QLabel("<b>Egalisation d'histogramme</b>"));
    auto *equalizeButton = new QPushButton("Globale");
    auto *claheButton = new QPushButton("CLAHE (local)");
    layout->addWidget(equalizeButton);
    layout->addWidget(claheButton);

    // Canny
    layout->addWidget(new QLabel("<b>Canny Edge Detection</b>"));
    layout->addWidget(new QLabel("Seuil bas :"));
    cannyThresh1_ = new QSlider(Qt::Horizontal);
    cannyThresh1_->setRange(0, 255);
    cannyThresh1_->setValue(50);
    layout->addWidget(cannyThresh1_);
    layout->addWidget(new QLabel("Seuil haut :"));
    cannyThresh2_ = new QSlider(Qt::Horizontal);
    cannyThresh2_->setRange(0, 255);
    cannyThresh2_->setValue(150);
    layout->addWidget(cannyThresh2_);
    layout->addWidget(new QLabel("Aperture :"));
    cannyApertureCombo_ = new QComboBox;
    cannyApertureCombo_->addItem("3");
    cannyApertureCombo_->addItem("5");
    cannyApertureCombo_->addItem("7");
    layout->addWidget(cannyApertureCombo_);
    auto *cannyButton = new QPushButton("Appliquer Canny");
    layout->addWidget(cannyButton);

    // Geometric transforms
    layout->addWidget(new QLabel("<b>Geometric Transforms</b>"));
    layout->addWidget(new QLabel("Angle de rotation :"));
    deskewAngleSlider_ = new QSlider(Qt::Horizontal);
    deskewAngleSlider_->setRange(-180, 180);
    deskewAngleSlider_->setValue(0);
    layout->addWidget(deskewAngleSlider_);
    auto *deskewButton = new QPushButton("Appliquer rotation");
    layout->addWidget(deskewButton);

    // Panorama
    layout->addWidget(new QLabel("<b>Panorama / Stitching</b>"));
    auto *panoramaButton = new QPushButton("Charger images et assembler");
    layout->addWidget(panoramaButton);

    // Video (time-lapse / slow-motion)
    layout->addWidget(new QLabel("<b>Video (time-lapse / slow-motion)</b>"));
    videoSpeedCombo_ = new QComboBox;
    videoSpeedCombo_->addItem("0.25x (ralenti fort)", 0.25);
    videoSpeedCombo_->addItem("0.5x (ralenti)", 0.5);
    videoSpeedCombo_->addItem("1x (normal)", 1.0);
    videoSpeedCombo_->addItem("2x (acceleration)", 2.0);
    videoSpeedCombo_->addItem("4x (time-lapse)", 4.0);
    videoSpeedCombo_->addItem("8x (time-lapse fort)", 8.0);
    videoSpeedCombo_->setCurrentIndex(2);
    layout->addWidget(videoSpeedCombo_);
    // Morphology
        layout->addWidget(new QLabel("<b>Morphology</b>"));
        morphologyModeCombo_ = new QComboBox;
        morphologyModeCombo_->addItem("Dilate");
        morphologyModeCombo_->addItem("Erode");
        morphologyModeCombo_->addItem("Opening");
        morphologyModeCombo_->addItem("Closing");
        morphologyModeCombo_->addItem("Gradient");
        layout->addWidget(morphologyModeCombo_);
        layout->addWidget(new QLabel("Kernel size:"));
        morphologyKernelSlider_ = new QSlider(Qt::Horizontal);
        morphologyKernelSlider_->setRange(1, 21);
        morphologyKernelSlider_->setValue(3);
        morphologyKernelSlider_->setSingleStep(2);
        layout->addWidget(morphologyKernelSlider_);
        auto *morphologyButton = new QPushButton("Appliquer Morphology");
        layout->addWidget(morphologyButton);

// Creative Effects
    layout->addWidget(new QLabel("<b>Creative Effects</b>"));
    auto *sketchButton = new QPushButton("Pencil Sketch");
    auto *cartoonButton = new QPushButton("Cartoon");
    layout->addWidget(sketchButton);
    layout->addWidget(cartoonButton);

    // Enhancement
    layout->addWidget(new QLabel("<b>Enhancement</b>"));
    layout->addWidget(new QLabel("Sharpening strength:"));
    unsharpStrengthSlider_ = new QSlider(Qt::Horizontal);
    unsharpStrengthSlider_->setRange(1, 30);
    unsharpStrengthSlider_->setValue(10);
    layout->addWidget(unsharpStrengthSlider_);
    auto *unsharpButton = new QPushButton("Unsharp Mask (Sharpen)");
    auto *bilateralButton = new QPushButton("Bilateral Denoise");
    layout->addWidget(unsharpButton);
    layout->addWidget(bilateralButton);

    layout->addStretch();

    // Bouton d'export commun : image (avec filtres) ou video (avec vitesse)
    auto *exportButton = new QPushButton("Exporter le fichier...");
    layout->addWidget(exportButton);

    //Panneau scrollable pour pouvoir defiler quand il y a beaucoup de modules
    auto *panelScroll = new QScrollArea;
    panelScroll->setWidget(panel);
    panelScroll->setWidgetResizable(true);
    panelScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dock->setWidget(panelScroll);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    // Connexions existantes
    connect(thresholdSlider_, &QSlider::valueChanged, this, &MainWindow::onThresholdChanged);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetImage);
    connect(thresholdModeCombo_, &QComboBox::currentIndexChanged, this, &MainWindow::onModeChanged);
    connect(equalizeButton, &QPushButton::clicked, this, &MainWindow::onEqualize);
    connect(claheButton, &QPushButton::clicked, this, &MainWindow::onClahe);

    // Nouvelles connexions
    connect(cannyButton, &QPushButton::clicked, this, &MainWindow::onCannyApply);
    connect(deskewButton, &QPushButton::clicked, this, &MainWindow::onDeskewApply);
    connect(panoramaButton, &QPushButton::clicked, this, &MainWindow::onPanoramaApply);
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::onExport);
    connect(sketchButton, &QPushButton::clicked, this, &MainWindow::onPencilSketch);
    connect(cartoonButton, &QPushButton::clicked, this, &MainWindow::onCartoon);
    connect(unsharpButton, &QPushButton::clicked, this, &MainWindow::onUnsharpMask);
    connect(bilateralButton, &QPushButton::clicked, this, &MainWindow::onBilateralDenoise);
    connect(morphologyButton, &QPushButton::clicked, this, &MainWindow::onMorphologyApply);
}

void MainWindow::openImage() {
    const QString path = QFileDialog::getOpenFileName(
        this, "Ouvrir un fichier", QString(),
        "Fichiers (*.png *.jpg *.jpeg *.bmp *.tif *.tiff *.mp4 *.mov *.avi *.mkv);;"
        "Images (*.png *.jpg *.jpeg *.bmp *.tif *.tiff);;"
        "Videos (*.mp4 *.mov *.avi *.mkv)");
    if (path.isEmpty()) return;

    const QString suffix = QFileInfo(path).suffix().toLower();
    const bool isVideo = (suffix == "mp4" || suffix == "mov"
                          || suffix == "avi" || suffix == "mkv");

    if (isVideo) {
        // On lit la premiere frame comme apercu
        cv::VideoCapture cap(path.toStdString());
        cv::Mat frame;
        if (!cap.isOpened() || !cap.read(frame)) {
            QMessageBox::warning(this, "Erreur",
                "Impossible de lire cette video.");
            return;
        }
        cap.release();
        videoPath_ = path;
        originalImage_ = frame;
        displayImage(frame);
    } else {
        cv::Mat image = cv::imread(path.toStdString(), cv::IMREAD_COLOR);
        if (image.empty()) {
            QMessageBox::warning(this, "Erreur",
                "Impossible de charger cette image.");
            return;
        }
        videoPath_.clear();
        originalImage_ = image;
        displayImage(image);
    }
}

void MainWindow::onThresholdChanged(int value) {
    thresholdValueLabel_->setText(QString("Seuil : %1").arg(value));
    applyThresholding();
}

void MainWindow::onModeChanged(int index) {
    thresholdSlider_->setEnabled(index == 0);
    applyThresholding();
}

void MainWindow::applyThresholding() {
    if (originalImage_.empty()) return;
    cv::Mat result;
    if (thresholdModeCombo_->currentIndex() == 1)
        result = processing::applyOtsu(originalImage_);
    else if (thresholdModeCombo_->currentIndex() == 2)
        result = processing::applyAdaptive(originalImage_);
    else
        result = processing::applyThreshold(originalImage_, thresholdSlider_->value());
    displayImage(result);
}

void MainWindow::resetImage() {
    if (originalImage_.empty()) return;
    displayImage(originalImage_);
}

void MainWindow::onEqualize() {
    if (originalImage_.empty()) return;
    displayImage(processing::equalize(originalImage_));
}

void MainWindow::onClahe() {
    if (originalImage_.empty()) return;
    displayImage(processing::applyClahe(originalImage_));
}

void MainWindow::onCannyApply() {
    if (originalImage_.empty()) return;
    int t1 = cannyThresh1_->value();
    int t2 = cannyThresh2_->value();
    int aperture = cannyApertureCombo_->currentIndex() * 2 + 3;
    displayImage(CannyEdge::apply(originalImage_, t1, t2, aperture));
}

void MainWindow::onDeskewApply() {
    if (originalImage_.empty()) return;
    double angle = deskewAngleSlider_->value();
    displayImage(GeometricTransforms::deskew(originalImage_, angle));
}

void MainWindow::onPanoramaApply() {
    QStringList paths = QFileDialog::getOpenFileNames(
        this, "Choisir les images pour le panorama", QString(),
        "Images (*.png *.jpg *.jpeg *.bmp)");
    if (paths.size() < 2) {
        QMessageBox::warning(this, "Erreur", "Selectionnez au moins 2 images.");
        return;
    }
    std::vector<cv::Mat> images;
    for (const QString &p : paths)
        images.push_back(cv::imread(p.toStdString(), cv::IMREAD_COLOR));

    bool success = false;
    cv::Mat result = Panorama::stitch(images, success);
    if (!success) {
        QMessageBox::warning(this, "Erreur", "Le panorama a echoue.");
        return;
    }
    displayImage(result);
}

void MainWindow::onExport() {
    // Cas video : on traite la video d'origine avec la vitesse choisie
    if (!videoPath_.isEmpty()) {
        QString output = QFileDialog::getSaveFileName(
            this, "Enregistrer la video", QString(),
            "Video MP4 (*.mp4)");
        if (output.isEmpty()) return;
        if (!output.endsWith(".mp4", Qt::CaseInsensitive)) output += ".mp4";

        const double speed = videoSpeedCombo_->currentData().toDouble();
        const bool ok = Video::processSpeed(
            videoPath_.toStdString(), output.toStdString(), speed);

        if (ok) {
            QMessageBox::information(this, "Termine",
                "Video exportee :\n" + output);
        } else {
            QMessageBox::warning(this, "Erreur",
                "Echec de l'export video.");
        }
        return;
    }

    // Cas image : on enregistre l'image affichee (avec les filtres appliques)
    if (currentImage_.empty()) {
        QMessageBox::information(this, "Rien a exporter",
            "Ouvre d'abord une image ou une video.");
        return;
    }
    QString output = QFileDialog::getSaveFileName(
        this, "Enregistrer l'image", QString(),
        "Images (*.png *.jpg *.jpeg *.bmp *.tif)");
    if (output.isEmpty()) return;
    if (!output.contains('.')) output += ".png";

    const bool ok = cv::imwrite(output.toStdString(), currentImage_);
    if (ok) {
        QMessageBox::information(this, "Termine",
            "Image exportee :\n" + output);
    } else {
        QMessageBox::warning(this, "Erreur",
            "Echec de l'export image.");
    }
}

void MainWindow::displayImage(const cv::Mat &image) {
    currentImage_ = image;
    currentPixmap_ = QPixmap::fromImage(matToQImage(image));
    updateScaledPixmap();
}

void MainWindow::updateScaledPixmap() {
    if (currentPixmap_.isNull()) return;
    const QSize viewSize = scrollArea_->viewport()->size();
    imageLabel_->setPixmap(currentPixmap_.scaled(
        viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    updateScaledPixmap();
}
    void MainWindow::onPencilSketch() {
        if (originalImage_.empty()) return;
        displayImage(CreativeEffects::pencilSketch(originalImage_));
    }

    void MainWindow::onCartoon() {
        if (originalImage_.empty()) return;
        displayImage(CreativeEffects::cartoon(originalImage_));
    }

    void MainWindow::onUnsharpMask() {
        if (originalImage_.empty()) return;
        double strength = unsharpStrengthSlider_->value() / 10.0;
        displayImage(Enhancement::unsharpMask(originalImage_, strength));
    }

    void MainWindow::onBilateralDenoise() {
        if (originalImage_.empty()) return;
        displayImage(Enhancement::bilateralDenoise(originalImage_));
    }

void MainWindow::onMorphologyApply() {
    if (originalImage_.empty()) return;
    int kernelSize = morphologyKernelSlider_->value();
    if (kernelSize % 2 == 0) kernelSize++;
    const int mode = morphologyModeCombo_->currentIndex();
    cv::Mat result;
    if (mode == 0)      result = Morphology::dilate(originalImage_, kernelSize);
    else if (mode == 1) result = Morphology::erode(originalImage_, kernelSize);
    else if (mode == 2) result = Morphology::opening(originalImage_, kernelSize);
    else if (mode == 3) result = Morphology::closing(originalImage_, kernelSize);
    else                result = Morphology::gradient(originalImage_, kernelSize);
    displayImage(result);
}