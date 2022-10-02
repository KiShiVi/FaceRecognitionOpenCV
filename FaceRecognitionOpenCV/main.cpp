#include "facerecognitionopencv.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FaceRecognitionOpenCV w;
    w.show();
    return a.exec();
}
