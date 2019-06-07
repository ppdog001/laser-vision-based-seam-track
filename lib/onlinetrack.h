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
				sumUpList			//列表求和
				enableAutoTrack		//启用自动跟踪过程
				disableAutoTrack	//禁用自动跟踪过程
				updateHorizontalCalibrationRatio	//接收水平标定比率
				dip					//调用图像处理
				
				sendDIPResult_triggered				//发送DIP结果，结果是一幅处理图
				sendDIPCalculationResult_triggered	//发送DIP计算结果
				enableNextDIP_triggered				//下一帧的图像处理

				sendDIPResult_triggered
				sendDIPCalculationResult_triggered
				enableNextDIP_triggered
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-21    添加onlinetrack.h头部注释
******************************************************************************/
#pragma once
#ifndef _ONLINETRACK_H_
#define _ONLINETRACK_H_

#include "var.h"
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
	double sumUpList(vector<double>lst,int start,int end);

private:
	bool isDIPAllowed;
	QImage _image;
	bool isAutoTrackTriggered;
	Motion _rectificationObject;
	Motion motionRectification;
	double dHorizontalCalibrationRatio;

	int intBufferNum;		//需要缓存的偏差数
	double dSensorFrameRate;
	double dWeldSpeed;
	double dTorchSensorDistance;

	vector<double> absOffsetList;	//绝对偏差序列
	vector<double> rOffsetList;		//纠偏偏差序列
	double dAbsOffset;	//绝对偏差
	double dLastAbsOffset;	//上一个绝对偏差
	double dNthROffset;	//第n个纠偏偏差
	double dCROffset;	//当前纠偏误差
	double dTinyOffset;	//偏差太小，留到下次纠偏

	int intROffsetCount;	//纠偏次数计数
	int intOffsetCount;	//绝对偏差数计数
	
	QTimer sTimer;
	bool timeoutFlag;

	bool isTestModeTriggered;	
signals:
	void sendDIPResult_triggered(QImage);
	void sendDIPCalculationResult_triggered(SeamInfo);
	void enableNextDIP_triggered();

private slots:
	void enableAutoTrack(SeamTrackParameter spParameter);
	void disableAutoTrack();
	void syncHorizontalCalibrationRatio(double hcr);
	void timeOut200ms();
	void enableTestingMode();
	void disableTestingMode();
};

#endif