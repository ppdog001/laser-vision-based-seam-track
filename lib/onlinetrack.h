/******************************************************************************
  File name: onlinetrack.h
  Author: WillLi99		Date:2019-5-21
  Description:
              此文件定义了onlinetrack类. onlinetrack类负责在线图像处理和在线跟踪。            
  Others: 
  Function List:
                OnlineTrack			//基本配置
				~OnlineTrack		//
				start100msTiming	//开启100ms定时
				start200msTiming	//开启200ms定时
				timeOut200ms		//定时达到200ms后的处理
				stopTiming			//关闭定时
				enableAutoTrack		//启用自动跟踪过程
				disableAutoTrack	//禁用自动跟踪过程
				updateHorizontalCalibrationRatio	//接收水平标定比率
				dip					//调用图像处理
				receiveReferenctOffset	//接收参考偏差
				checkRecitificationAvailable		//检查是否需要进行纠偏
				recitifyWeldTorch					//纠正焊枪位置
				moveWeldTorch						//横向移动焊枪（即非纠偏的方向）
				recordSeamCenterCoordinate			//记录焊缝中心位置的绝对坐标
				recordWeldTorchCoordinate			//记录焊枪位置
				resetWeldTrackConfiguration			//重设

				sendDIPResult_triggered				//发送DIP结果，结果是一幅处理图
				sendDIPCalculationResult_triggered	//发送DIP计算结果
				enableNextDIP_triggered				//下一帧的图像处理
				sendDIPResult_triggered
				sendDIPCalculationResult_triggered
				enableNextDIP_triggered
				tirggerTestingMode
				detirggerTestingMode

  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-21    添加onlinetrack.h头部注释
		   WillLi99    2019-6-21    添加checkRecitificationAvailable、recordSeam
		                            CenterCoordinate等函数. 删除sumuplist
		   WillLi99    2019-7-18    添加resetWeldTrackConfiguration
		   WillLi99    2019-7-22     添加referenceTrack函数和globalCoordinateTrack
		                            函数，即是将相关的实现封装起来
******************************************************************************/
#pragma once
#ifndef _ONLINETRACK_H_
#define _ONLINETRACK_H_

#include "common.h"
#include "dip.h"
#include "motion.h"

/******************************************************************************
  Name:OnlineTrack
  Description: 在线跟踪和在线图像处理的类
  Used By: tracksys.h tracksys.cpp
  Others: 
******************************************************************************/
class OnlineTrack :
	public QThread
{
	Q_OBJECT
public:
	explicit OnlineTrack(QObject* parent=0);
	~OnlineTrack(void);
public:
	void run();
	void dip(QImage image);

private:
	void start100msTiming();
	void start200msTiming();
	void stopTiming();

	bool checkRecitificationAvailable(double deviation);
	void recitifyWeldTorch(double dSpeed,double dDistance,double dAccelerationTime,double dAccelerationMethod);
	void moveWeldTorch(double dSpeed,double dDistance,double dAccelerationTime,double dAccelerationMethod);
	void recordSeamCenterCoordinate(double coordinateX,double coordinateY);
	void recordWeldTorchCoordinate(double coordinateX,double coordinateY);
	void resetWeldTrackConfiguration();
	void referenceTrack(double dDeviation);
	void globalCoordinateTrack();

private:
	bool isDIPAllowed;
	QImage _image;
	int nthFrame;
	bool isAutoTrackTriggered;
	Motion _rectificationObject;
	Motion motionRectification;
	double dHorizontalCalibrationRatio;

	int intBufferNum;
	int intTotalTrackPeriodNum;
	double dSensorFrameRate;
	double dWeldSpeed;
	double dTrackDistance;
	double dTorchSensorDistance;

	double dOffset;	
	
	int intOffsetCount;
	int intNthPeriod;

	int intTrackMethod;
	double dReferenceOffset;
	double dLastPeriodRemain;
	
	CoordinateSet seamCenterCoordinateSet;
	CoordinateSet weldTorchCoordinateSet;
	double dLastWeldTorchCoordinateX;
	double dWeldTorchCoordinateX;
	double dWeldTorchCoordinateY;
	double dWeldSeamCoordinateX;
	double dWeldSeamCoordinateY;

	QTimer sTimer;
	bool timeoutFlag;

	bool isTestModeTriggered;	
signals:
	void sendDIPResult_triggered(QImage);
	void sendDIPCalculationResult_triggered(SeamInfo);
	void sendDIPResult2_triggered(QImage);
	void enableNextDIP_triggered();

private slots:
	void enableAutoTrack(SeamTrackParameter spParameter);
	void disableAutoTrack();
	void syncHorizontalCalibrationRatio(double dHorizontalCalibrationRatio);
	void timeOut200ms();
	void enableTestingMode();
	void disableTestingMode();
	void syncTrackMethod(int intTrackMethod);
	void receiveReferenceOffset(double dReferenceOffset);
	void tirggerTestingMode();
	void detriggerTestingMode();
};
#endif