#include "facerecognitionopencv.h"

#include <QtGui/qpixmap.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>

FaceRecognitionOpenCV::FaceRecognitionOpenCV(QWidget *parent)
    : QWidget(parent)
{
    initGui();
}

FaceRecognitionOpenCV::~FaceRecognitionOpenCV()
{}

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




    p_foundFace = new QLabel();
    p_foundFace->setScaledContents(true);
    p_foundFace->setMaximumSize(256, 256);
    p_foundFace->setPixmap(defaultImage);
    QVBoxLayout* rightSideLayout = new QVBoxLayout();
    rightSideLayout->addWidget(p_foundFace);


    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftSideLayout);
    mainLayout->addLayout(rightSideLayout);

    setLayout(mainLayout);
}
