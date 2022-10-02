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

class FaceRecognitionOpenCV : public QWidget
{
    Q_OBJECT

public:
    FaceRecognitionOpenCV(QWidget *parent = nullptr);
    ~FaceRecognitionOpenCV();

public slots:
    void updateImages(QPixmap mainPixmap, QPixmap zoomPixmap, int amountOfFoundFaces);

private:
    void initGui();
    void connects();

private slots:
    void onStopCameraClicked();
    void onStartCameraClicked();
    void onSliderValueChanged();

private:
    QThread*                thread;
    QSlider*                p_zoomImageChoise;
    //QPixmap*                mainImage;
    //QList<QPixmap>*         zoomImages;
    std::atomic<bool>       m_cameraIsWorking;
    std::atomic<int>       m_numberOfFace;
    OpenCVTools*            p_openCvTools;
    QPushButton*            p_cameraStart;
    QPushButton*            p_cameraStop;
    QComboBox*              p_typeOfInputImage;
    QLabel*                 p_inputImage;
    QLabel*                 p_foundFace;

};
