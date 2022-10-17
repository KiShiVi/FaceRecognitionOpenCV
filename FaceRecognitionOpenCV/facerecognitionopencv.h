#pragma once

#include <atomic>
#include <QtWidgets/QWidget>
#include <QGraphicsScene>

#include "barchart.h"

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
    void onRunButtonClicked();
    void onClearButtonClicked();
    void onExitButtonClicked();
    void onErrorSignal(int);

private:
    QString                 filepath;
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
    QPushButton*            p_runImage;
    QPushButton*            p_clearImage;
    QPushButton*            p_exit;
    QComboBox*              p_typeOfInputImage;
    QLabel*                 p_inputImage;
    QLabel*                 p_foundFace;
    //BarChart*               p_workPlace1;
    QLabel*               p_workPlace1;
    QLabel*                 p_workPlace2;
    QCheckBox*              p_findEyes;
    QCheckBox*              p_BW;

    QMessageBox*            p_msgBox;
};
