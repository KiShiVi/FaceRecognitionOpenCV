#include "facerecognitionopencv.h"
#include "opencv_tools.h"

#include <QtGui/qpixmap.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qslider.h>
#include <QtCore/qthread.h>

FaceRecognitionOpenCV::FaceRecognitionOpenCV(QWidget *parent)
    : QWidget(parent)
{
    initGui();
    //mainImage = new QPixmap();
    //zoomImages = new QList<QPixmap>();
    m_cameraIsWorking.store(false);
    m_numberOfFace.store(0);
    p_openCvTools = new OpenCVTools(&m_cameraIsWorking, &m_numberOfFace);
    connects();
}

FaceRecognitionOpenCV::~FaceRecognitionOpenCV()
{}



void FaceRecognitionOpenCV::updateImages(QPixmap mainPixmap, QPixmap zoomPixmap, int amountOfFoundFaces)
{
    p_zoomImageChoise->setMaximum(amountOfFoundFaces > 0 ? amountOfFoundFaces - 1 : 0);

    p_inputImage->setPixmap(mainPixmap);
    if (!zoomPixmap.isNull())
        p_foundFace->setPixmap(zoomPixmap);
}



void FaceRecognitionOpenCV::connects()
{
    connect(p_cameraStart, SIGNAL(clicked()), this, SLOT(onStartCameraClicked()));
    connect(p_cameraStop, SIGNAL(clicked()), this, SLOT(onStopCameraClicked()));
    connect(p_openCvTools, SIGNAL(updatePixmaps(QPixmap, QPixmap, int)), this, SLOT(updateImages(QPixmap, QPixmap, int)));
    connect(p_zoomImageChoise, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged()));
}



void FaceRecognitionOpenCV::onSliderValueChanged()
{
    m_numberOfFace.store(p_zoomImageChoise->value());
}



void FaceRecognitionOpenCV::onStartCameraClicked()
{
    if (m_cameraIsWorking)
        return;
    thread = new QThread();
    moveToThread(thread);
    connect(thread, SIGNAL(started()), p_openCvTools, SLOT(cameraRun()), Qt::DirectConnection);
    thread->start();
}


void FaceRecognitionOpenCV::onStopCameraClicked()
{
    m_cameraIsWorking.store(false);
}



void FaceRecognitionOpenCV::initGui()
{
    p_typeOfInputImage = new QComboBox();
    p_typeOfInputImage->addItem("Default camera");
    p_typeOfInputImage->addItem("Image from file system");
    QHBoxLayout* typeOfInputImageLayout = new QHBoxLayout();
    typeOfInputImageLayout->addWidget(new QLabel("Type of input image"));
    typeOfInputImageLayout->addWidget(p_typeOfInputImage);

    QPixmap defaultImage("D:/QtProjects/FaceRecognitionOpenCV/FaceRecognitionOpenCV/Lenna.png");
    p_inputImage = new QLabel();
    p_inputImage->setScaledContents(true);
    p_inputImage->setMaximumSize(256, 256);
    //p_inputImage->setMaximumSize(512, 512);
    p_inputImage->setPixmap(defaultImage);


    QVBoxLayout* leftSideLayout = new QVBoxLayout();
    leftSideLayout->addLayout(typeOfInputImageLayout);
    leftSideLayout->addWidget(p_inputImage);

    QHBoxLayout* cameraButtonsLayout = new QHBoxLayout();
    p_cameraStart   = new QPushButton("Start camera");
    p_cameraStop    = new QPushButton("Stop camera");
    cameraButtonsLayout->addWidget(p_cameraStart);
    cameraButtonsLayout->addWidget(p_cameraStop);

    leftSideLayout->addLayout(cameraButtonsLayout);


    p_zoomImageChoise = new QSlider();
    p_zoomImageChoise->setOrientation(Qt::Horizontal);
    p_zoomImageChoise->setMinimum(0);
    p_zoomImageChoise->setMaximum(0);
    p_zoomImageChoise->setValue(0);


    p_foundFace = new QLabel();
    p_foundFace->setScaledContents(true);
    p_foundFace->setMaximumSize(256, 256);
    p_foundFace->setPixmap(defaultImage);
    QVBoxLayout* rightSideLayout = new QVBoxLayout();
    rightSideLayout->addWidget(p_zoomImageChoise);
    rightSideLayout->addWidget(p_foundFace);


    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftSideLayout);
    mainLayout->addLayout(rightSideLayout);

    setLayout(mainLayout);
}
