#include "facerecognitionopencv.h"
#include "opencv_tools.h"

#include <QtGui/qpixmap.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qslider.h>
#include <QtCore/qthread.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qcheckbox.h>

FaceRecognitionOpenCV::FaceRecognitionOpenCV(QWidget *parent)
    : QWidget(parent)
{
    initGui();
    //mainImage = new QPixmap();
    //zoomImages = new QList<QPixmap>();
    m_cameraIsWorking.store(false);
    m_numberOfFace = 0;
    p_openCvTools = new OpenCVTools(&m_cameraIsWorking);
    connects();

    p_typeOfInputImage->setCurrentIndex(0);
    onCurrentIndexChanged();


    p_openCvTools->detectAndDisplayOneShot("..\\TestImages\\Lenna.png", 
        false);

    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
}

FaceRecognitionOpenCV::~FaceRecognitionOpenCV()
{}



void FaceRecognitionOpenCV::updateImagesContainers(QPixmap mainPixmap, QList<QPixmap> zoomPixmap, int amountOfFoundFaces)
{
    p_zoomImageChoise->setMaximum(amountOfFoundFaces > 0 ? amountOfFoundFaces - 1 : 0);
    m_numberOfFace = amountOfFoundFaces < m_numberOfFace ? 0 : m_numberOfFace;

    mainImage = mainPixmap;
    zoomImages.clear();
    foreach (QPixmap elem, zoomPixmap)
    {
        zoomImages.append(elem);
    }
    updateImages();
}



void FaceRecognitionOpenCV::connects()
{
    connect(p_cameraStart, SIGNAL(clicked()), this, SLOT(onStartCameraClicked()));
    connect(p_cameraStop, SIGNAL(clicked()), this, SLOT(onStopCameraClicked()));
    connect(p_openCvTools, SIGNAL(updatePixmaps(QPixmap, QList<QPixmap>, int)), this, SLOT(updateImagesContainers(QPixmap, QList<QPixmap>, int)));
    connect(p_zoomImageChoise, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged()));
    connect(p_typeOfInputImage, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged()));
    connect(p_loadImage, SIGNAL(clicked()), this, SLOT(onLoadButtonClicked()));
}

void FaceRecognitionOpenCV::updateImages()
{
    p_inputImage->setPixmap(mainImage.scaled(p_inputImage->width(), p_inputImage->height(), Qt::KeepAspectRatio));
    if (zoomImages.size() > 0)
        p_foundFace->setPixmap(zoomImages[m_numberOfFace].scaled(p_foundFace->width(), p_foundFace->height(), Qt::KeepAspectRatio));
}



void FaceRecognitionOpenCV::onSliderValueChanged()
{
    m_numberOfFace = p_zoomImageChoise->value();
    updateImages();
}

void FaceRecognitionOpenCV::onCurrentIndexChanged()
{
    p_zoomImageChoise->setValue(0);
    m_numberOfFace = 0;
    switch (p_typeOfInputImage->currentIndex())
    {
    case 0:
        p_cameraStart   ->setEnabled(false);
        p_cameraStop    ->setEnabled(false);
        p_loadImage     ->setEnabled(true);
        break;
    case 1:
        p_cameraStart   ->setEnabled(true);
        p_cameraStop    ->setEnabled(true);
        p_loadImage     ->setEnabled(false);
        break;
    }
}

void FaceRecognitionOpenCV::onLoadButtonClicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Open Image", "..\\TestImages", "Other Files (*);; PNG Files (*.png);;JPG Files (*.JPG);;");

    if (filepath != "")
        p_openCvTools->detectAndDisplayOneShot(filepath.toStdString(), p_findEyes->isChecked());
}



void FaceRecognitionOpenCV::onStartCameraClicked()
{
    p_findEyes->setEnabled(false);
    p_typeOfInputImage->setEnabled(false);
    if (m_cameraIsWorking)
        return;
    thread = new QThread();
    moveToThread(thread);
    connect(thread, SIGNAL(started()), this, SLOT(viaRunCamera()), Qt::DirectConnection);
    thread->start();
}

void FaceRecognitionOpenCV::viaRunCamera()
{
    p_openCvTools->cameraRun(p_findEyes->isChecked());
}


void FaceRecognitionOpenCV::onStopCameraClicked()
{
    p_findEyes->setEnabled(true);
    p_typeOfInputImage->setEnabled(true);
    m_cameraIsWorking.store(false);
}



void FaceRecognitionOpenCV::initGui()
{
    p_typeOfInputImage = new QComboBox();
    p_typeOfInputImage->addItem("Image from file system");
    p_typeOfInputImage->addItem("Default camera");
    QHBoxLayout* typeOfInputImageLayout = new QHBoxLayout();
    typeOfInputImageLayout->addWidget(new QLabel("Type of input image"));
    typeOfInputImageLayout->addWidget(p_typeOfInputImage);

    p_inputImage = new QLabel();
    p_inputImage->setMaximumSize(256, 256);

    QVBoxLayout* leftSideLayout = new QVBoxLayout();
    leftSideLayout->addLayout(typeOfInputImageLayout);
    leftSideLayout->addWidget(p_inputImage);

    QHBoxLayout* cameraButtonsLayout = new QHBoxLayout();
    p_cameraStart   = new QPushButton("Start camera");
    p_cameraStop    = new QPushButton("Stop camera");
    cameraButtonsLayout->addWidget(p_cameraStart);
    cameraButtonsLayout->addWidget(p_cameraStop);

    leftSideLayout->addLayout(cameraButtonsLayout);
    p_loadImage = new QPushButton("Load Image");
    leftSideLayout->addWidget(p_loadImage);

    p_findEyes = new QCheckBox("Find eyes");
    leftSideLayout->addWidget(p_findEyes);

    p_zoomImageChoise = new QSlider();
    p_zoomImageChoise->setOrientation(Qt::Horizontal);
    p_zoomImageChoise->setMinimum(0);
    p_zoomImageChoise->setMaximum(0);
    p_zoomImageChoise->setValue(0);
    p_zoomImageChoise->setPageStep(1);


    p_foundFace = new QLabel();
    //p_foundFace->setScaledContents(true);
    p_foundFace->setMaximumSize(256, 256);
    //p_foundFace->setPixmap(defaultImage);
    QVBoxLayout* rightSideLayout = new QVBoxLayout();
    rightSideLayout->addWidget(p_zoomImageChoise);
    rightSideLayout->addWidget(p_foundFace);
    rightSideLayout->addStretch(0);


    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftSideLayout);
    mainLayout->addLayout(rightSideLayout);

    setLayout(mainLayout);
}
