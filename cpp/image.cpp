/******************************************************************************
  File name: image.cpp
  Author: WillLi99		Date:2019-5-18
  Description:
              ImagesAquisition类封装了关于图片的获取的相关内容。图片获取的硬件是JH300m
              相机，图片获取的入口是调用stream函数。
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
           WillLi99    2019-5-18     添加calbraiton3.cpp头部注释和各函数的头部注释
******************************************************************************/
#include "JHCap.h"
#include "dip.h"
#include "image.h"

bool isTestingModeTriggered=true;
int ImageAcquisition::INT_IMAGE_COUNT=1;


/******************************************************************************
  Function:ImageAcquisition
  Description:初始化变量值，创建../Images目录
  Calls: 
  Called By: ImageAcquisition类对象
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
ImageAcquisition::ImageAcquisition(QObject *parent)
{
	isCapturingImagesAllowed=false;
	isPauseCmdTriggered=false;
	isTestingModeTriggered=false;
	intCameraIndex=0;
	isSavingMultipleImagesCmdTriggered=false;
	QString qstrFileDirectory="..\\Images";

	if(!qdirImagesDirectory.exists(qstrFileDirectory))
	{
		qdirImagesDirectory.mkdir(qstrFileDirectory);
	}
}

/******************************************************************************
  Function:run
  Description: 用于从数据源抓取数据，数据源有两个：../Images/testing images的图片和
               相机摄取的图片。
  Calls: JHCap.h/CameraGetImageSize JHCap.h/CameraGetImageBufferSize
         JHCap.h/CameraGetISPImageBufferSize JHCap.h/CameraQueryImage
  Called By: ImageAcquisition类对象
  Input:          
  Output: 
  Return:       
  Others: ImagesAcquisition继承于QThread，当线程开启时，此函数体内的内容被一直执行。当
  摄取到一帧图像时，emit出一个信号，待SeamTrackingSys模块处理
******************************************************************************/
void ImageAcquisition::run()
{
	while(true)
	{
		if(isTestingModeTriggered)	//isTestingModeTriggerd=true为测试模式
		{
			int intNthImageNo;
			int intTestingImageNum=39;
			cv::string strTestingFilePath="..//Images//testing images//";
			for(intNthImageNo=1;intNthImageNo<=intTestingImageNum;intNthImageNo++)
			{
				cv::string strNewfilePath=strTestingFilePath+std::to_string((long long)intNthImageNo)+".bmp";
				cv::Mat matRawImage=cv::imread(strNewfilePath);
				
				QImage image=DIP::mat2QImage(matRawImage);

				emit captured2(image);

				if(!isTestingModeTriggered)	break;

				Sleep(500);;
			}
			if(intNthImageNo==intTestingImageNum)	intNthImageNo=1;
		}
		else if(!isTestingModeTriggered && g_isCameraWorking) // isTestingModeTriggered=true g_isCameraWorking=ture, 为正常模式
		{
			QTime runtime;
			runtime.start();
			int intWidth, intHeight, intLen, intSize;

			if(isCapturingImagesAllowed)
			{

				CameraGetImageSize(intCameraIndex,&intWidth, &intHeight);
				CameraGetImageBufferSize(intCameraIndex,&intLen, CAMERA_IMAGE_RAW8);
				CameraGetISPImageBufferSize(intCameraIndex,&intSize,intWidth,intHeight,CAMERA_IMAGE_RGB24);
				unsigned char *buffer = new unsigned char[intLen];
				unsigned char *ISPbuf = new unsigned char[intSize];
				if(CameraQueryImage(intCameraIndex,buffer, &intLen, CAMERA_IMAGE_RAW8)==API_OK)
				{
					if(isPauseCmdTriggered) break;
					if(CameraISP(intCameraIndex,buffer,ISPbuf,intWidth,intHeight,CAMERA_IMAGE_RGB24)==API_OK)
					{
						QImage img(ISPbuf, intWidth, intHeight, QImage::Format_RGB888);
						if(isSavingMultipleImagesCmdTriggered)
						{
							QString filePath="..\\Images\\"+QString::number(INT_IMAGE_COUNT++,10) + ".bmp";
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

			} // end of "else if(!isTestingModeTriggered && g_isCameraWorking)"
			else usleep(1000);
			if(isPauseCmdTriggered) break;
		}
	}
}

/******************************************************************************
  Function: stream
  Description: 启动抓取相机图像内容
  Calls: 
  Called By: ImageAcquisition类对象
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::stream()
{
	isCapturingImagesAllowed=true;
	isPauseCmdTriggered=false;
}

/******************************************************************************
  Function: setCameraIndex
  Description: 设置相机index号
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::setCameraIndex(int index)
{
	this->intCameraIndex=index;
}

/******************************************************************************
  Function: setCameraIndex
  Description: 暂停摄取图像
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::pause()
{
	isCapturingImagesAllowed=false;
}

/******************************************************************************
  Function: setCameraIndex
  Description: 停止摄取图像
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::stop()
{
	isCapturingImagesAllowed=false;
	isPauseCmdTriggered=true;
}

/******************************************************************************
  Function: allowSavingMultipleImages
  Description: 允许保存多张图像
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::allowSavingMultipleImages()
{
	if(!isTestingModeTriggered)
		isSavingMultipleImagesCmdTriggered=true;
}

/******************************************************************************
  Function: declineSavingMultipleImages
  Description: 停止保存多张图像
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::declineSavingMultipleImages()
{
	isSavingMultipleImagesCmdTriggered=false;
}

/******************************************************************************
  Function: tirggerTestingMode
  Description: 开启测试模式
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::tirggerTestingMode()
{
	isTestingModeTriggered=true;
}

/******************************************************************************
  Function: detriggerTestingMode
  Description: 关闭测试模式
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void ImageAcquisition::detriggerTestingMode()
{
	isTestingModeTriggered=false;
}