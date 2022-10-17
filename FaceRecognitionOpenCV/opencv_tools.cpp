#include "opencv_tools.h"
#include <QtGui/qimage.h>

OpenCVTools::OpenCVTools(std::atomic<bool>* in_cameraIsWorkingm) :
    cameraIsWorking(in_cameraIsWorkingm)
{
    face_cascade = cv::CascadeClassifier();
    eyes_cascade = cv::CascadeClassifier();

    cv::String face_cascade_name = "../haarcascades/haarcascade_frontalface_alt.xml";
    cv::String eyes_cascade_name = "../haarcascades/haarcascade_eye_tree_eyeglasses.xml";

    if(!face_cascade.load(face_cascade_name))
        exit(1);
    if (!eyes_cascade.load(eyes_cascade_name))
        exit(1);
}

void OpenCVTools::detectAndDisplayOneShot(std::string imagePath, bool eyesDetecting, bool bw)
{
    cv::Mat frame = cv::imread(imagePath);
    detectAndDisplay(frame, eyesDetecting, bw);
}

void OpenCVTools::cameraRun(bool findEyes, bool bw)
{
    cameraIsWorking->store(true);
    cv::VideoCapture capture;
    capture.open(0);
    if (!capture.isOpened())
    {
        emit errorSignal(CAMERA_OPEN_ERROR);
        return;
    }

    cv::Mat frame;
    while (capture.read(frame) && *cameraIsWorking)
    {
        if (frame.empty())
        {
            emit errorSignal(CAMERA_CAPTURED_ERROR);
            return;
        }

        detectAndDisplay(frame, findEyes, bw);
    }

}

void OpenCVTools::detectAndDisplay(cv::Mat frame, bool eyesDetecting, bool bw)
{
    cv::Mat initFrame = frame.clone();
    cv::Mat eyesOnlyFrame = frame.clone();
    QList<QPixmap> zoomImages;
    QPixmap mainImage = cvMatToQPixmap(frame);
    cv::Mat frame_gray;
    cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);
    //-- Detect faces

    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(frame_gray, faces);
    cv::Point eye_center;
    int radius;
    for (size_t i = 0; i < faces.size(); i++)
    {
        cv::Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
        //rectangle(frame, faces[i], cv::Scalar(255, 0, 255), 16);
        //ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
        cv::Mat faceROI = frame_gray(faces[i]);

        if (eyesDetecting)
        {
            //-- In each face, detect eyes
            std::vector<cv::Rect> eyes;
            eyes_cascade.detectMultiScale(faceROI, eyes);
            for (size_t j = 0; j < eyes.size(); j++)
            {
                eye_center = cv::Point(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
                radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
                circle(eyesOnlyFrame, eye_center, radius, cv::Scalar(255, 0, 0), 6);
            }
        }
    }
    ////-- Show what you got
    for (int i = 0; i < faces.size(); ++i)
    {
        cv::Mat ROI(eyesOnlyFrame, cv::Rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height));
        if (bw)
            cvtColor(ROI, ROI, cv::COLOR_BGR2GRAY);
        zoomImages.append(cvMatToQPixmap(ROI));
    }
    //QPixmap zoomImage;
    //if (faces.size() > 0 && *numberOfFace < faces.size())
    //    zoomImage = zoomImages[*numberOfFace];
    //else
    //    zoomImage = QPixmap();

    mainImage = cvMatToQPixmap(frame);

    //cv::calcHist()

    emit updatePixmaps(mainImage, zoomImages, faces.size());
}

QPixmap OpenCVTools::cvMatToQPixmap(const cv::Mat& inMat)
{
    switch (inMat.type())
    {
        // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image(inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_ARGB32);

        return QPixmap::fromImage(image);
    }

    // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image(inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_RGB888);

        return QPixmap::fromImage(image.rgbSwapped());
    }

    // 8-bit, 1 channel
    case CV_8UC1:
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        QImage image(inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_Grayscale8);
#else
        static QVector<QRgb>  sColorTable;

        // only create our color table the first time
        if (sColorTable.isEmpty())
        {
            sColorTable.resize(256);

            for (int i = 0; i < 256; ++i)
            {
                sColorTable[i] = qRgb(i, i, i);
            }
        }

        QImage image(inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_Indexed8);

        image.setColorTable(sColorTable);
#endif

        return QPixmap::fromImage(image);
    }

    default:
        emit errorSignal(DEFAULT_ERROR);
        break;
    }

    return QPixmap();
}
