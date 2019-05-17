#ifndef _IMAGE_H
#define _IMAGE_H

#include "var.h"

extern bool cameraIsRunning;

//Õº∆¨ªÒ»°¿‡
class ImagesAcqusition:public QThread
{
	Q_OBJECT;
public:
	explicit ImagesAcqusition(QObject* parent=0);
public:
	void run();
	void setCameraIndex(int index);
	void stream();
	void pause();
	void stop();
signals:
	void captured(QImage img,unsigned char * buffer);
	void captured2(QImage image);
private:
	int cameraIndex;
	bool workState;
	bool terminateState;
	QDir directory;
	bool savingImagesSwitch;

	static int imageCount;
private slots:
	void setSavingPicturesSwitch();
	void resetSavingPicturesSwitch();
};

#endif
