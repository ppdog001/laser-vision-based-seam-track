/*****************************************************************************
Copyright: Guangdong Provincial Welding Technology Research Center
Author: Li Yangjin
Date: 2018-8-27
Description:ImagesAquisition类封装了关于图片的获取的相关内容。图片获取的硬件是JH300m
相机，图片获取的入口是调用stream函数。
*****************************************************************************/
#include "JHCap.h"
#include "dip.h"
#include "image.h"

bool testOrNot=true;

int ImagesAcqusition::imageCount=1;

ImagesAcqusition::ImagesAcqusition(QObject *parent)
{
	workState=false;
	terminateState=false;
	cameraIndex=0;
	savingImagesSwitch=false;
	QString dir="..\\Images";

	if(!directory.exists(dir))
	{
		directory.mkdir(dir);
	}
}

/*****************************************************************************
Function:void ImagesAcqusition::run()
Description:线程运行的执行的函数
Call:
Input:
Output:
Return:
Others:ImagesAcquisition继承于QThread，当线程开启时，此函数体内的内容被一直执行。当
摄取到一帧图像时，emit出一个信号，待SeamTrackingSys模块处理
*****************************************************************************/
void ImagesAcqusition::run()
{
	while(true)
	{
		if(testOrNot)	//处于测试模式
		{
			int i;
			int testingImageNum=39;
			cv::string testingFilePath="..//Images//testing images//";
			for(i=1;i<=testingImageNum;i++)
			{

				//qDebug()<<"Now the "<<i<<"th image is on process..."<<endl;

				cv::string newfilePath=testingFilePath+std::to_string((long long)i)+".bmp";
				cv::Mat img=cv::imread(newfilePath);
				
				QImage image=DIP::mat2QImage(img);

				emit captured2(image);

				if(!testOrNot)	break;

				Sleep(500);;
			}
			
			if(i==testingImageNum)	i=1;

		}
		else if(!testOrNot && cameraIsRunning)
		{
			QTime runtime;
			runtime.start();
			int width, height, len, size;

			if(workState)
			{

				CameraGetImageSize(cameraIndex,&width, &height);
				CameraGetImageBufferSize(cameraIndex,&len, CAMERA_IMAGE_RAW8);
				CameraGetISPImageBufferSize(cameraIndex,&size,width,height,CAMERA_IMAGE_RGB24);
				unsigned char *buffer = new unsigned char[len];
				unsigned char *ISPbuf = new unsigned char[size];
				if(CameraQueryImage(cameraIndex,buffer, &len, CAMERA_IMAGE_RAW8)==API_OK)
				{
					if(terminateState) break;
					if(CameraISP(cameraIndex,buffer,ISPbuf,width,height,CAMERA_IMAGE_RGB24)==API_OK)
					{
						QImage img(ISPbuf, width, height, QImage::Format_RGB888);
						if(savingImagesSwitch)
						{
							QString filePath="..\\Images\\"+QString::number(imageCount++,10) + ".bmp";
							img.save(filePath);
						}
						emit captured(img, ISPbuf);
					}
				}
				else
				{
					delete [] ISPbuf;
				}
				delete [] buffer;

			} else usleep(1000);
			if(terminateState) break;
		}
	}
}

/*****************************************************************************
Function:void ImagesAcqusition::stream()
Description:相当于开关功能
Call:
Input:
Output:
Return:
Others:
*****************************************************************************/
void ImagesAcqusition::stream()
{
	workState=true;
	terminateState=false;
}

/*****************************************************************************
Function:ImagesAcqusition::setCameraIndex(int index)
Description:设计相机Index号
Call:
Input:index-相机序号，若有n部京航相机连接到电脑，序号依次为0,1,...n-1
Output:
Return:
Others:
*****************************************************************************/
void ImagesAcqusition::setCameraIndex(int index)
{
	this->cameraIndex=index;
}

/*****************************************************************************
Function:void ImagesAcqusition::pause()
Description:暂停摄取图片
Call:
Input:
Output:
Return:
Others:
*****************************************************************************/
void ImagesAcqusition::pause()
{
	workState=false;
}

/*****************************************************************************
Function:void ImagesAcqusition::stop()
Description:停止摄取图片
Call:
Input:
Output:
Return:
Others:
*****************************************************************************/
void ImagesAcqusition::stop()
{
	workState=false;
	terminateState=true;
}

/*****************************************************************************
Function:void ImagesAcqusition::setSavingPicturesSwitch()
Description:自动保存图片开
Call:
Input:
Output:
Return:
Others:保存路径在"..\\Images"目录，序号从1开始
*****************************************************************************/
void ImagesAcqusition::setSavingPicturesSwitch()
{
	if(!testOrNot)
		savingImagesSwitch=true;
}

/*****************************************************************************
Function:void ImagesAcqusition::setSavingPicturesSwitch()
Description:自动保存图片关
Call:
Input:
Output:
Return:
Others:
*****************************************************************************/
void ImagesAcqusition::resetSavingPicturesSwitch()
{
	savingImagesSwitch=false;
}