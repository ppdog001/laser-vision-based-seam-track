/******************************************************************************
  File name: image.h
  Author: WillLi99		Date:2019-5-18
  Description:
              定义了ImageAcquisition类，此类继承于QThread，负责从相机中读取图像
  Others: 
  Function List:
                1. ImageAcquisition
				2. ~ImageAcquisiton
				3. run	// 读取相机图像
				4. setCameraIndex	// 设定相机号
				5. pause	//暂停摄取图片
				6. stop		//停止摄取图片
				7. allowSavingMultipleImages	// 开自动保存图片
				8. declineSavingMultipleImages //	关自动保存图片
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-18     添加calbraiton3.h头部注释
******************************************************************************/

#ifndef _IMAGE_H
#define _IMAGE_H

#include "var.h"

extern bool g_isCameraWorking;


//图片获取类
class ImageAcquisition:public QThread
{
	Q_OBJECT;
public:
	explicit ImageAcquisition(QObject* parent=0);
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
	int intCameraIndex;
	bool isCapturingImagesAllowed;
	bool isPauseCmdTriggered;
	QDir qdirImagesDirectory;
	bool isSavingMultipleImagesCmdTriggered;
	bool isTestingModeTriggered;
	static int INT_IMAGE_COUNT;
private slots:
	void allowSavingMultipleImages();
	void declineSavingMultipleImages();
	void tirggerTestingMode();
	void detriggerTestingMode();
};

#endif
