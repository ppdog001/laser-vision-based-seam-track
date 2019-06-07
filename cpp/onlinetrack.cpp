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
				sumUpList			//列表求和
				allowAutoTrack		//启用自动跟踪过程
				declineAutoTrack	//禁用自动跟踪过程
				updateHorizontalCalibrationRatio	//接收水平标定比率
				dip					//调用图像处理
				
				sendDIPResult_triggered				//发送DIP结果，结果是一幅处理图
				sendDIPCalculationResult_triggered	//发送DIP计算结果
				allowNextDIP_triggered				//下一帧的图像处理
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-21    添加onlinetrack.h头部注释
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
	
	dTorchSensorDistance=42;	//42mm
	dSensorFrameRate=5;
	intROffsetCount=0;
	intOffsetCount=0;
	dLastAbsOffset=0.0;
	dTinyOffset=0.0;
	timeoutFlag=true;
	isTestModeTriggered=true;

	//没有更新的情况下，按照以下参数运行
	//设置intBufferNum.帧率为10Hz,y轴移动速度为
	dWeldSpeed=2;	//焊接速度为2mm/s
	intBufferNum=dSensorFrameRate*dTorchSensorDistance/dWeldSpeed;
	qDebug()<<"intBufferNum="<<intBufferNum;
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
  Function:sumUpList
  Description: 将制定的list内容相加求和
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
double OnlineTrack::sumUpList(vector<double>lst,int start,int end)
{
	double value=0.0;
	for(int i=start;i<end;i++)
	{
		value=value+lst[i];
	}
	return value;
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
	dWeldSpeed=spParameter.weldingVelocity;
	intBufferNum=dSensorFrameRate*dTorchSensorDistance/dWeldSpeed;
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
		motionRectification.yMove(dWeldSpeed,dWeldSpeed*0.2,0.01,0);
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
void OnlineTrack::syncHorizontalCalibrationRatio(double hcr)
{
	dHorizontalCalibrationRatio=hcr;
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
}

/******************************************************************************
  Function:run
  Description: 
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void OnlineTrack::run()
{
	while(true)
	{
		if(isDIPAllowed && !isTestModeTriggered) //在线模式下，需要在图像处理完成后进行纠偏
		{
			Mat imgOnProcessing=DIP::qImage2Mat(_image);
			DIP::getROIPosition(imgOnProcessing,&DIP::roiX,&DIP::roiY);
			DIP dip(imgOnProcessing);
			
			//图像处理时间约为30~50ms
			
			emit sendDIPResult_triggered(dip.out);
			emit sendDIPCalculationResult_triggered(dip.seaminfo);

			dAbsOffset=dip.seaminfo.dWeldSeamOffset*dHorizontalCalibrationRatio;
			isDIPAllowed=false;

			/***********进行在线跟踪***************/
			if(isAutoTrackTriggered)
			{
				absOffsetList.push_back(dAbsOffset);	//添加绝对偏差
				qDebug()<<"adding "<<intOffsetCount<<"th absolute offset:"<<dAbsOffset;
				
				if(intOffsetCount<intBufferNum)
				{
					dNthROffset=dAbsOffset-dLastAbsOffset;
					rOffsetList.push_back(dNthROffset);
					qDebug()<<"adding "<<intOffsetCount<<"th"<<"rectifying offseta;"<<dNthROffset;
					dLastAbsOffset=dAbsOffset;
					intOffsetCount++;
				}
				else if(intOffsetCount>=intBufferNum)//开始纠偏
				{
					dNthROffset=dAbsOffset-sumUpList(rOffsetList,intROffsetCount+1,intOffsetCount);
					rOffsetList.push_back(dNthROffset);
					qDebug()<<"adding "<<intOffsetCount<<"th rectifying offset:"<<dNthROffset;
					
					dCROffset=rOffsetList[intROffsetCount]+dTinyOffset;					
					if(abs(dCROffset)>0.2 && abs(dCROffset)<3.0)
					{
						motionRectification.xMove(5*dCROffset,dCROffset,0.01,0);
						qDebug()<<"correcting "<<intROffsetCount<<"th. "<<dCROffset;
						dTinyOffset=0.0;
					}
					else if(abs(dCROffset)<=0.2)
					{
						dTinyOffset=dTinyOffset+dCROffset;
					}
					intROffsetCount++;
					intOffsetCount++;
				}
			} // end of if(isAutoTrackTriggered)

		} //end of if(isDIPAllowed && !testOrNot)
	}// end of while(true)
}
