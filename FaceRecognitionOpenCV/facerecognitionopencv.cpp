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
#include <QtWidgets/qmessagebox.h>
#include <QPainter>

FaceRecognitionOpenCV::FaceRecognitionOpenCV(QWidget *parent)
    : QWidget(parent)
{
    p_msgBox = new QMessageBox();
    initGui();
    //mainImage = new QPixmap();
    //zoomImages = new QList<QPixmap>();
    m_cameraIsWorking.store(false);
    m_numberOfFace = 0;
    p_openCvTools = new OpenCVTools(&m_cameraIsWorking);
    connects();

    p_typeOfInputImage->setCurrentIndex(0);
    onCurrentIndexChanged();


    //p_openCvTools->detectAndDisplayOneShot("..\\TestImages\\Lenna.png", 
    //    false);

    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
}

FaceRecognitionOpenCV::~FaceRecognitionOpenCV()
{
}



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
    connect(p_openCvTools, SIGNAL(errorSignal(int)), this, SLOT(onErrorSignal(int)));
    connect(p_zoomImageChoise, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged()));
    connect(p_typeOfInputImage, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged()));
    connect(p_loadImage, SIGNAL(clicked()), this, SLOT(onLoadButtonClicked()));
    connect(p_runImage, SIGNAL(clicked()), this, SLOT(onRunButtonClicked()));
    connect(p_clearImage, SIGNAL(clicked()), this, SLOT(onClearButtonClicked()));
    connect(p_exit, SIGNAL(clicked()), this, SLOT(onExitButtonClicked()));
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
        p_runImage      ->setEnabled(true);
        p_clearImage    ->setEnabled(true);
        break;
    case 1:
        p_cameraStart   ->setEnabled(true);
        p_cameraStop    ->setEnabled(true);
        p_loadImage     ->setEnabled(false);
        p_runImage      ->setEnabled(false);
        p_clearImage    ->setEnabled(false);
        break;
    }
}

void FaceRecognitionOpenCV::onLoadButtonClicked()
{
    filepath = QFileDialog::getOpenFileName(this, "Open Image", "..\\TestImages", "Other Files (*);; PNG Files (*.png);;JPG Files (*.JPG);;");

    onRunButtonClicked();
}

void FaceRecognitionOpenCV::onRunButtonClicked()
{
    if (filepath != "")
        p_openCvTools->detectAndDisplayOneShot(filepath.toStdString(), p_findEyes->isChecked(), p_BW->isChecked());
}

void FaceRecognitionOpenCV::onClearButtonClicked()
{
    p_foundFace->clear();
    p_workPlace1->clear();
    p_workPlace2->clear();
}

void FaceRecognitionOpenCV::onExitButtonClicked()
{
    exit(0);
}

void FaceRecognitionOpenCV::onErrorSignal(int errorID)
{
    p_msgBox->setWindowTitle("Error");
    switch (errorID)
    {
    case DEFAULT_ERROR:
        p_msgBox->setText("Unknown Error!");
        break;
    case CAMERA_OPEN_ERROR:
        p_msgBox->setText("Camera open error!");
        p_cameraStart->setEnabled(true);
        p_cameraStop->setEnabled(true);
        p_loadImage->setEnabled(false);
        m_cameraIsWorking.store(false);
        p_findEyes->setEnabled(true);
        p_BW->setEnabled(false);
        p_typeOfInputImage->setEnabled(true);
        break;
    case CAMERA_CAPTURED_ERROR:
        p_msgBox->setText("Camera captured error");
        p_cameraStart->setEnabled(true);
        p_cameraStop->setEnabled(true);
        p_loadImage->setEnabled(false);
        m_cameraIsWorking.store(false);
        p_findEyes->setEnabled(true);
        p_BW->setEnabled(true);
        p_typeOfInputImage->setEnabled(true);
        break;
    }
    p_msgBox->show();
}



void FaceRecognitionOpenCV::onStartCameraClicked()
{
    p_BW->setEnabled(false);
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
    p_openCvTools->cameraRun(p_findEyes->isChecked(), p_BW->isChecked());
}


void FaceRecognitionOpenCV::onStopCameraClicked()
{
    p_BW->setEnabled(true);
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
    p_inputImage->setFixedSize(300, 300);

    QVBoxLayout* leftSideLayout = new QVBoxLayout();
    leftSideLayout->addLayout(typeOfInputImageLayout);
    leftSideLayout->addWidget(p_inputImage);

    QHBoxLayout* cameraButtonsLayout = new QHBoxLayout();
    p_cameraStart   = new QPushButton("Start camera");
    p_cameraStop    = new QPushButton("Stop camera");
    cameraButtonsLayout->addWidget(p_cameraStart);
    cameraButtonsLayout->addWidget(p_cameraStop);

    leftSideLayout->addLayout(cameraButtonsLayout);
    p_loadImage     = new QPushButton("Load Image");
    p_runImage      = new QPushButton("Run");
    p_clearImage    = new QPushButton("Clear");
    p_exit          = new QPushButton("Exit");
    leftSideLayout->addWidget(p_loadImage);
    leftSideLayout->addWidget(p_runImage);
    leftSideLayout->addWidget(p_clearImage);
    leftSideLayout->addWidget(p_exit);

    p_findEyes  = new QCheckBox("Find eyes");
    p_BW        = new QCheckBox("BW");
    leftSideLayout->addWidget(p_findEyes);
    leftSideLayout->addWidget(p_BW);

    p_zoomImageChoise = new QSlider();
    p_zoomImageChoise->setOrientation(Qt::Horizontal);
    p_zoomImageChoise->setMinimum(0);
    p_zoomImageChoise->setMaximum(0);
    p_zoomImageChoise->setValue(0);
    p_zoomImageChoise->setPageStep(1);


    p_foundFace = new QLabel();
    //p_foundFace->setScaledContents(true);
    p_foundFace->setFixedSize(300, 300);
    //p_foundFace->setPixmap(defaultImage);
    QVBoxLayout* rightSideLayout = new QVBoxLayout();
    rightSideLayout->addWidget(p_zoomImageChoise);
    rightSideLayout->addWidget(p_foundFace);
    rightSideLayout->addStretch(0);

    QVBoxLayout* secondRightSideLayout = new QVBoxLayout();
    p_workPlace1 = new QLabel();
    //p_workPlace1 = new BarChart();
    p_workPlace1->setFixedSize(300, 300);
    //p_workPlace1->DRAW(0, 0, 300, 300);
    //p_workPlace1->DRAW(0, 300, 0, 250);

    secondRightSideLayout->addWidget(p_workPlace1);

    QVBoxLayout* thirdRightSideLayout = new QVBoxLayout();
    p_workPlace2 = new QLabel();
    p_workPlace2->setFixedSize(300, 300);
    thirdRightSideLayout->addWidget(p_workPlace2);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftSideLayout);
    mainLayout->addLayout(rightSideLayout);
    mainLayout->addLayout(secondRightSideLayout);
    mainLayout->addLayout(thirdRightSideLayout);

    setLayout(mainLayout);
}
