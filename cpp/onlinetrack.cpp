/******************************************************************************
  File name: onlinetrack.cpp
  Author: WillLi99		Date:2019-5-21
  Description:
              此文件实现onlinetrack类. onlinetrack类负责在线图像处理和在线跟踪。            
  Others: 
  Function List:
                OnlineTrack			//基本配置
				~OnlineTrack		//
				start100msTiming	//开启100ms定时
				start200msTiming	//开启200ms定时
				timeOut200ms		//定时达到200ms后的处理
				stopTiming			//关闭定时
				allowAutoTrack		//启用自动跟踪过程
				declineAutoTrack	//禁用自动跟踪过程
				updateHorizontalCalibrationRatio	//接收水平标定比率
				dip					//调用图像处理
				receiveReferenctOffset	//接收参考偏差
				checkRecitificationAvailable		//检查是否需要进行纠偏
				recitifyWeldTorch					//纠正焊枪位置
				moveWeldTorch						//横向移动焊枪（即非纠偏的方向）
				recordSeamCenterCoordinate			//记录焊缝中心位置的绝对坐标
				
				sendDIPResult_triggered				//发送DIP结果，结果是一幅处理图
				sendDIPCalculationResult_triggered	//发送DIP计算结果
				allowNextDIP_triggered				//下一帧的图像处理
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-21    添加onlinetrack.h头部注释
		   WillLi99    2019-6-21    添加checkRecitificationAvailable、recordSeam
		                            CenterCoordinate等函数. 删除sumuplist
		   WillLi99    2019-7-18    添加resetWeldTrackConfiguration
******************************************************************************/
#include "dip.h"
#include "onlinetrack.h"

/******************************************************************************
  Function:OnlineTrack
  Description: 初始化变量和创建信号连接
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
OnlineTrack::OnlineTrack(QObject* parent)
{
	isDIPAllowed=false;
	isAutoTrackTriggered=false;
	isTestModeTriggered=false;

	dTorchSensorDistance=60;
	dSensorFrameRate=5;
	intOffsetCount=0;
	intNthPeriod=0;
	dReferenceOffset=0.0;
	dWeldSpeed=2.0;
	intTrackMethod=2;
	dLastPeriodRemain=0.0;
	dLastWeldTorchCoordinateX=0.0;

	timeoutFlag=true;
	nthFrame=0;

	connect(&sTimer, SIGNAL(timeout()), this, SLOT(timeOut200ms()));
}

/******************************************************************************
  Function:~OnlineTrack
  Description: 
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
OnlineTrack::~OnlineTrack(void)
{
}

/******************************************************************************
  Function:start100msTiming
  Description: 启动100ms定时
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::start100msTiming()
{
	sTimer.start(100);	//启动100ms定时
}

/******************************************************************************
  Function:start200msTiming
  Description: 启动200ms定时
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::start200msTiming()
{
	sTimer.start(200);	//启动200ms定时
}

/******************************************************************************
  Function:stopTiming
  Description: 关闭定时
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::stopTiming()
{
	sTimer.stop();
}

/******************************************************************************
  Function:timeOut200ms
  Description: 启动200ms定时
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::timeOut200ms()
{
	timeoutFlag=true;
	stopTiming();
	emit enableNextDIP_triggered();
}


/******************************************************************************
  Function:enableAutoTrack
  Description: 使能自动跟踪
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::enableAutoTrack(SeamTrackParameter spParameter)
{
	isAutoTrackTriggered=true;
	dWeldSpeed=spParameter.trackingVelocity;
	dTrackDistance=spParameter.trackingDistance;
	intBufferNum=dSensorFrameRate*dTorchSensorDistance/dWeldSpeed;
	intTotalTrackPeriodNum=dSensorFrameRate*spParameter.trackingDistance/dWeldSpeed;

	qDebug()<<QStringLiteral("总跟踪周期")<<intTotalTrackPeriodNum;
}

/******************************************************************************
  Function:disableAutoTrack
  Description: 关闭自动跟踪
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::disableAutoTrack()
{
	isAutoTrackTriggered=false;
	nthFrame=0;
	resetWeldTrackConfiguration();
}

/******************************************************************************
  Function:dip
  Description: 允许执行一次图像处理
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::dip(QImage image)
{
	//深拷贝
	_image=image.copy();
	start200msTiming();		//开启一个周期200ms的定时
	isDIPAllowed=true;
	if(isAutoTrackTriggered) 
	{
		qDebug()<<"weld torch moving";
		moveWeldTorch(dWeldSpeed*1.2,dWeldSpeed*0.2,0.01,2);
	}
}

/******************************************************************************
  Function:syncHorizontalCalibrationRatio
  Description: 同步水平标定率
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::syncHorizontalCalibrationRatio(double dHorizontalCalibrationRatio)
{
	this->dHorizontalCalibrationRatio=dHorizontalCalibrationRatio;
}

/******************************************************************************
  Function:enableTestingMode
  Description: 允许测试模式
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::enableTestingMode()
{
	isTestModeTriggered=true;
}

/******************************************************************************
  Function:disableTestingMode
  Description: 禁止测试模式
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::disableTestingMode()
{
	isTestModeTriggered=false;
	nthFrame=0;
}

/******************************************************************************
  Function:run
  Description: 
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 注意！在线跟踪过程的参考点法只是适用于直线焊缝的跟踪。
******************************************************************************/
void OnlineTrack::run()
{
	while(true)
	{
		if(isDIPAllowed && !isTestModeTriggered) //在线模式下，需要在图像处理完成后进行纠偏
		{
			//图像处理时间约为30~50ms

			Mat image=DIP::qImage2Mat(_image);
			if(nthFrame==0)
			{
				DIP::setROISize(400,200);
				DIP::locateROI(image);
			}
			DIP dip(image);
			
			emit sendDIPResult_triggered(dip.qOutputImage);
			emit sendDIPCalculationResult_triggered(dip.seamProfileInfo);

			dOffset=dip.seamProfileInfo.dWeldSeamOffset*dHorizontalCalibrationRatio;
			isDIPAllowed=false;

			double dDeviation;
			/***********进行在线跟踪***************/
			if(isAutoTrackTriggered)
			{
				if(intTrackMethod == TRACKMETHOD_REFERENCE)	//参考点法
				{
					dDeviation=dOffset-dReferenceOffset+dLastPeriodRemain;
					referenceTrack(dDeviation);
				} //end of if intTrackMethod=TRACKMETHOD_REFERENCE

				if(intTrackMethod == TRACKMETHOD_GLOBAL_COORDINATE)
				{
					globalCoordinateTrack();
				}// end of if intTrackMethod=TRACKMETHOD_GLOBAL_COORDINATE

			} // end of if(isAutoTrackTriggered)
			nthFrame++;
		}
		else if(isDIPAllowed && isTestModeTriggered)
		{
			//qDebug()<<"nthFrame="<<nthFrame;
			Mat image=DIP::qImage2Mat(_image);
			if(nthFrame==0)
			{
				DIP::setROISize(400,200);
				DIP::locateROI(image);
			}
			DIP dip(image);

			emit sendDIPResult2_triggered(dip.qOutputImage);
			isDIPAllowed=false;
			nthFrame++;
		}
	}//end of while
}

/******************************************************************************
  Function:receiveReferenceOffset
  Description: 更新参考偏差
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: dReferenceOffset来自tracksys
******************************************************************************/
void OnlineTrack::receiveReferenceOffset(double dReferenceOffset)
{
	this->dReferenceOffset=dReferenceOffset;
	qDebug()<<QStringLiteral("参考偏差")<<dReferenceOffset;
}

/******************************************************************************
  Function:checkRecitificationAvailable
  Description: 判断偏差量大小，若偏差量小于thresh,则无需进行纠偏，返回false，否则需要
               进行纠偏控制，返回true
  Calls: 
  Called By: 
  Input: deviation-偏差量
  Output:
  Return:       
  Others: 
******************************************************************************/
bool OnlineTrack::checkRecitificationAvailable(double deviation)
{
	double dThreshold=0.2;
	if(abs(deviation)<dThreshold)
	{
		dLastPeriodRemain=deviation;
		return false;
	}
	else
	{
		dLastPeriodRemain=0.0;
		return true;
	}
}

/******************************************************************************
  Function:recitifyWeldTorch
  Description: 纠正焊枪的位置
  Calls: 
  Called By: 
  Input: dSpeed-纠偏速度；dDistance-纠偏距离；dAccTime-加速时间；dAccMethod-加速方法
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::recitifyWeldTorch(double dSpeed,double dDistance,
	double dAccelerationTime,double dAccelerationMethod)
{
	motionRectification.xMove(dSpeed,dDistance,dAccelerationTime,dAccelerationMethod);
}

/******************************************************************************
  Function:moveWeldTorch
  Description: 移动焊枪
  Calls: 
  Called By: 
  Input: dSpeed-纠偏速度；dDistance-纠偏距离；dAccTime-加速时间；dAccMethod-加速方法
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::moveWeldTorch(double dSpeed,double dDistance,
	double dAccelerationTime,double dAccelerationMethod)
{
	motionRectification.yMove(dSpeed,dDistance,0.01,dAccelerationMethod);
}

/******************************************************************************
  Function:recordSeamCenterCoordinate
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::recordSeamCenterCoordinate(double coordinateX,double coordinateY)
{
	Point2d p=Point2d(coordinateX,coordinateY);
	seamCenterCoordinateSet.push_back(p);
}

/******************************************************************************
  Function:recordWeldTorchCoordinate
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::recordWeldTorchCoordinate(double coordinateX,double coordinateY)
{
	Point2d p=Point2d(dWeldTorchCoordinateX,dWeldTorchCoordinateY);
	weldTorchCoordinateSet.push_back(p);
}

/******************************************************************************
  Function:syncTrackMethod
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::syncTrackMethod(int intTrackMethod)
{
	this->intTrackMethod=intTrackMethod;
}

/******************************************************************************
  Function:resetWeldTrackConfiguration
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::resetWeldTrackConfiguration()
{
	dWeldSpeed=0.0;
	dReferenceOffset=0.0;
	dTrackDistance=0.0;
	intBufferNum=0;
	intNthPeriod=0;
	intTotalTrackPeriodNum=0;

	//清除焊缝坐标集和焊枪位置坐标
	weldTorchCoordinateSet.clear();
	seamCenterCoordinateSet.clear();
}

/******************************************************************************
  Function:tirggerTestingMode
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::tirggerTestingMode()
{
	isTestModeTriggered=true;
}

/******************************************************************************
  Function:detriggerTestingMode
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::detriggerTestingMode()
{
	isTestModeTriggered=false;
}

/******************************************************************************
  Function:referenceTrack
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::referenceTrack(double dDeviation)
{
	//qDebug()<<intNthPeriod<<"nth track";
	dDeviation=dOffset-dReferenceOffset+dLastPeriodRemain;
	if(checkRecitificationAvailable(dDeviation))
	{
		recitifyWeldTorch(5*dDeviation,dDeviation,0.01,SIGMOID_ACCELERATION);
	}
	intOffsetCount++;
	intNthPeriod++;

	if(intNthPeriod>intTotalTrackPeriodNum)	//跟踪完毕
	{
		isAutoTrackTriggered=false;
	}
}

/******************************************************************************
  Function:globalCoordinateTrack
  Description: 
  Calls: 
  Called By: 
  Input:
  Output:
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::globalCoordinateTrack()
{
	if(intNthPeriod<intBufferNum)	//只记录，不纠偏
	{
		dWeldTorchCoordinateX=0;
		dWeldTorchCoordinateY=intNthPeriod*dWeldSpeed/dSensorFrameRate-dTorchSensorDistance;

		dWeldSeamCoordinateX=dOffset;
		dWeldSeamCoordinateY=intNthPeriod*dWeldSpeed/dSensorFrameRate;

		recordSeamCenterCoordinate(dWeldSeamCoordinateX,dWeldSeamCoordinateY);
		recordWeldTorchCoordinate(dWeldTorchCoordinateX,dWeldTorchCoordinateY);
	}
	else if(intNthPeriod>=intBufferNum) //边记录，边纠偏
	{
		dWeldSeamCoordinateX = dLastWeldTorchCoordinateX + dOffset;
		dWeldSeamCoordinateY = intNthPeriod*dWeldSpeed/dSensorFrameRate;
		dWeldTorchCoordinateX = dLastWeldTorchCoordinateX;
		dWeldTorchCoordinateY = intOffsetCount*dWeldSpeed/dSensorFrameRate-dTorchSensorDistance;

		recordSeamCenterCoordinate(dWeldSeamCoordinateX,dWeldSeamCoordinateY);
		recordWeldTorchCoordinate(dWeldTorchCoordinateX,dWeldTorchCoordinateY);

		double dDeviation=seamCenterCoordinateSet[intNthPeriod-intBufferNum].x-
			weldTorchCoordinateSet[intNthPeriod].x + dLastPeriodRemain;

		if(!checkRecitificationAvailable(dDeviation))
		{
			dLastPeriodRemain=dOffset;
		}
		else
		{
			recitifyWeldTorch(5*dOffset,dOffset,0.01,2);
			dLastWeldTorchCoordinateX=dWeldSeamCoordinateX + dOffset;
			dLastPeriodRemain=0.0;
		}
	}
	else if(intNthPeriod>=intTotalTrackPeriodNum-intBufferNum) //只纠偏，不记录
	{
		dWeldTorchCoordinateX = dLastWeldTorchCoordinateX;
		dWeldTorchCoordinateY = intNthPeriod*dWeldSpeed/dSensorFrameRate-dTorchSensorDistance;

		recordWeldTorchCoordinate(dWeldTorchCoordinateX,dWeldTorchCoordinateX);

		double dOffset=seamCenterCoordinateSet[2*intNthPeriod-intTotalTrackPeriodNum].x-
			weldTorchCoordinateSet[intNthPeriod].x;

		if(!checkRecitificationAvailable(dOffset))
		{
			dLastPeriodRemain=dOffset;
		}
		else
		{
			recitifyWeldTorch(5*dOffset,dOffset,0.01,2);
			dLastWeldTorchCoordinateX=dWeldSeamCoordinateX + dOffset;
			dLastPeriodRemain=0.0;
		}
	}

	if(intNthPeriod==intTotalTrackPeriodNum+intBufferNum)	//跟踪结束
	{
		isAutoTrackTriggered=false;
	}

	intOffsetCount++;
	intNthPeriod++;
}