#pragma once
#ifndef _ONLINETRACK_H_
#define _ONLINETRACK_H_

#include "var.h"
#include "dip.h"
#include "motion.h"

extern bool testOrNot;
extern bool AutoTrackingTriggledFlag;


//图像处理和自动跟踪类
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
	bool dipSwitch;		//处理图像开关
	QImage _image;
	bool _autoTrackingSwitch;
	Motion _rectificationObject;
	double _rho;

	int bufferNum;		//需要缓存的偏差数
	double sensorFrameRate;
	double weldVelocity;
	double torchSensorDistance;

	vector<double> absOffsetList;	//绝对偏差序列
	vector<double> rOffsetList;		//纠偏偏差序列
	double absOffset;	//绝对偏差
	double lAbsOffset;	//上一个绝对偏差
	double nROffset;	//第n个纠偏偏差
	double cROffset;	//当前纠偏误差
	double tinyOffset;	//偏差太小，留到下次纠偏

	int nROffsetCount;	//纠偏次数计数
	int offsetCount;	//绝对偏差数计数
	
	QTimer sTimer;
	bool timeoutFlag;
	
signals:
	void sendDIPResult_triggered(QImage);
	void sendDIPCalculationResult_triggered(SeamInfo);
	void allowNextDIP_triggered();

private slots:
	void setAutoTrackingSwitch(SeamTrackParameter seamTrackingParas);
	void updateHorizontalCalibrationRatio(double rho);
	void stopAutoTracking();
	void timeOut200ms();
};

#endif