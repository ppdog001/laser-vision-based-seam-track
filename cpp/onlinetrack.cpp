/*****************************************************************************
Copyright: Guangdong Provincial Welding Technology Research Center
Author: Li Yangjin
Date: 2018-8-27
Description:Dip_AutoTracking类封装了图像处理的功能和实时跟踪的功能。调用Dip_Auto
			Tracking实例的processImage开启图像识别处理，完成识别后emit输出图片和
			偏差bias. 自动纠偏功能的开机依赖于_autoTrackingSwitch的开启，由set-
			AutoTrackingSwitch函数负责。
*****************************************************************************/
#include "dip.h"
#include "onlinetrack.h"

OnlineTrack::OnlineTrack(QObject* parent)
{
	dipSwitch=false;
	_autoTrackingSwitch=false;
	torchSensorDistance=42;
	sensorFrameRate=5;
	nROffsetCount=0;
	offsetCount=0;
	lAbsOffset=0.0;
	tinyOffset=0.0;
	timeoutFlag=true;

	//没有更新的情况下，按照以下参数运行
	//设置bufferNum.帧率为10Hz,y轴移动速度为
	weldVelocity=2;	//焊接速度为2mm/s
	bufferNum=sensorFrameRate*torchSensorDistance/weldVelocity;
	qDebug()<<"bufferNum="<<bufferNum;
	connect(&sTimer, SIGNAL(timeout()), this, SLOT(timeOut200ms()));

}


OnlineTrack::~OnlineTrack(void)
{
}

void OnlineTrack::run()
{
	while(true)
	{
		if(dipSwitch && !testOrNot) //在线模式下，需要在图像处理完成后进行纠偏
		{
			Mat imgOnProcessing=DIP::qImage2Mat(_image);
			DIP::getROIPosition(imgOnProcessing,&DIP::roiX,&DIP::roiY);
			DIP dip(imgOnProcessing);
			
			//图像处理时间约为30~50ms
			
			emit sendDIPResult_triggered(dip.out);
			emit sendDIPCalculationResult_triggered(dip.seaminfo);

			absOffset=dip.seaminfo.weldSeamOffset*_rho;
			dipSwitch=false;

			/***********进行在线跟踪***************/
			if(AutoTrackingTriggledFlag)
			{
				absOffsetList.push_back(absOffset);	//添加绝对偏差
				qDebug()<<"adding "<<offsetCount<<"th absolute offset:"<<absOffset;
				
				if(offsetCount<bufferNum)
				{
					nROffset=absOffset-lAbsOffset;
					rOffsetList.push_back(nROffset);
					qDebug()<<"adding "<<offsetCount<<"th"<<"rectifying offseta;"<<nROffset;
					lAbsOffset=absOffset;
					offsetCount++;
				}
				else if(offsetCount>=bufferNum)//开始纠偏
				{
					nROffset=absOffset-sumUpList(rOffsetList,nROffsetCount+1,offsetCount);
					rOffsetList.push_back(nROffset);
					qDebug()<<"adding "<<offsetCount<<"th rectifying offset:"<<nROffset;
					
					cROffset=rOffsetList[nROffsetCount]+tinyOffset;					
					if(abs(cROffset)>0.2 && abs(cROffset)<3.0)
					{
						_rectificationObject.xMove(5*cROffset,cROffset,0.01,0);
						qDebug()<<"correcting "<<nROffsetCount<<"th. "<<cROffset;
						tinyOffset=0.0;
					}
					else if(abs(cROffset)<=0.2)
					{
						tinyOffset=tinyOffset+cROffset;
					}
					nROffsetCount++;
					offsetCount++;
				}
			}

		}
	}
}

void OnlineTrack::dip(QImage image)
{
	//深拷贝
	_image=image.copy();
	start200msTiming();		//开启一个周期200ms的定时
	dipSwitch=true;
	if(AutoTrackingTriggledFlag)
	{
		_rectificationObject.yMove(weldVelocity,weldVelocity*0.2,0.01,0);
	}
}

void OnlineTrack::setAutoTrackingSwitch(SeamTrackParameter seamTrackingParas)
{
	_autoTrackingSwitch=true;
	weldVelocity=seamTrackingParas.weldingVelocity;
	bufferNum=sensorFrameRate*torchSensorDistance/weldVelocity;
}


void OnlineTrack::updateHorizontalCalibrationRatio(double rho)
{
	_rho=rho;
}

void OnlineTrack::timeOut200ms()
{
	timeoutFlag=true;
	stopTiming();
	emit allowNextDIP_triggered();
}

void OnlineTrack::start100msTiming()
{
	sTimer.start(100);	//启动100ms定时
}

void OnlineTrack::start200msTiming()
{
	sTimer.start(200);	//启动100ms定时
}

void OnlineTrack::stopTiming()
{
	sTimer.stop();
}

void OnlineTrack::stopAutoTracking()
{
	_autoTrackingSwitch=false;
}

double OnlineTrack::sumUpList(vector<double>lst,int start,int end)
{
	double value=0.0;
	for(int i=start;i<end;i++)
	{
		value=value+lst[i];
	}
	return value;
}