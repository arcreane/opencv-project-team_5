#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("OpenCV Image Editor - Team 5");
    window.resize(1000, 700);

    auto *label = new QLabel(
        QString("Qt %1 + OpenCV %2 - skeleton OK")
            .arg(QT_VERSION_STR)
            .arg(CV_VERSION));
    label->setAlignment(Qt::AlignCenter);
    window.setCentralWidget(label);

    window.show();
    return app.exec();
}
