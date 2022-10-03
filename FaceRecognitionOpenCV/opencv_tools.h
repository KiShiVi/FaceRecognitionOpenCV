#pragma once

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <QtCore/qobject.h>
#include <QtGui/qpixmap.h>
#include <atomic>
#include <string>

enum ErrorID
{
	DEFAULT_ERROR,
	CAMERA_OPEN_ERROR,
	CAMERA_CAPTURED_ERROR
};

class OpenCVTools : public QObject
{
	Q_OBJECT
public:
	OpenCVTools(std::atomic<bool>* in_cameraIsWorking);

public slots:
	void cameraRun(bool findEyes);
	void detectAndDisplayOneShot(std::string imagePath, bool eyesDetecting);

signals:
	void errorSignal(int error);
	void updatePixmaps(QPixmap mainPixmap, QList<QPixmap> zoomPixmap, int amountOfFoundFaces);

private:
	void detectAndDisplay(cv::Mat frame, bool eyesDetecting);
	QPixmap cvMatToQPixmap(const cv::Mat & inMat);

	std::atomic<bool>*		cameraIsWorking;

	cv::CascadeClassifier	face_cascade;
	cv::CascadeClassifier	eyes_cascade;
};

