#pragma once

#include <atomic>
#include <QtWidgets/QWidget>

class QThread;
class QComboBox;
class QLabel;
class QPushButton;
class OpenCVTools;
class QPixmap;
class QSlider;
class QCheckBox;
class QMessageBox;

class FaceRecognitionOpenCV : public QWidget
{
    Q_OBJECT

public:
    FaceRecognitionOpenCV(QWidget *parent = nullptr);
    ~FaceRecognitionOpenCV();

public slots:
    void updateImagesContainers(QPixmap mainPixmap, QList<QPixmap> zoomPixmap, int amountOfFoundFaces);

private:
    void initGui();
    void connects();
    void updateImages();

private slots:
    void onStopCameraClicked();
    void onStartCameraClicked();
    void viaRunCamera();
    void onSliderValueChanged();
    void onCurrentIndexChanged();
    void onLoadButtonClicked();
    void onErrorSignal(int);

private:
    QThread*                thread;
    QSlider*                p_zoomImageChoise;
    QPixmap                 mainImage;
    QList<QPixmap>          zoomImages;
    std::atomic<bool>       m_cameraIsWorking;
    int                     m_numberOfFace;
    OpenCVTools*            p_openCvTools;
    QPushButton*            p_cameraStart;
    QPushButton*            p_cameraStop;
    QPushButton*            p_loadImage;
    QComboBox*              p_typeOfInputImage;
    QLabel*                 p_inputImage;
    QLabel*                 p_foundFace;
    QCheckBox*              p_findEyes;

    QMessageBox*            p_msgBox;
};
