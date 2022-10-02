#pragma once

#include <QtWidgets/QWidget>

class QComboBox;
class QLabel;

class FaceRecognitionOpenCV : public QWidget
{
    Q_OBJECT

public:
    FaceRecognitionOpenCV(QWidget *parent = nullptr);
    ~FaceRecognitionOpenCV();

private:
    void initGui();

    QComboBox*      p_typeOfInputImage;
    QLabel*         p_inputImage;
    QLabel*         p_foundFace;

};
