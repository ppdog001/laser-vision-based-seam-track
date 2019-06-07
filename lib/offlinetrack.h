/******************************************************************************
  File name: offlinetrack.h
  Author: WillLi99		Date:2019-5-20
  Description:
              定义了OfflineTrack类，此类继承于QWidget，负责离线焊接模式的所有内容
  Others: 
  Function List:
               OfflineTrack
			   ~OfflineTrack
			   start200msTiming		//200ms定时
			   stop200msTiming		//200ms定时终止
			   start100msTimming	//100ms定时
			   stop100msTiming		//100ms定时终止
			   start150msTiming		//150ms定时
			   stop150msTiming		//150ms定时
			   switchOnWeld			//开启焊接
			   switchOffWeld		//关闭焊接

			   synchoriWeldParameters				//同步焊接参数
			   syncHorizontalCalibrationRatio		//同步水平标定比率
			   on_pushButtonCollectData_clicked		//采集数据
			   on_pushButtonAnalyzeData_clicked		//数据处理
			   on_pushButtonRecoverSensorPosition_clicked	//复位传感器位置
			   on_pushButtonSaveData_clicked		//储存数据
			   on_pushButtonClearData_clicked		//清除数据
			   on_pushButtonLoadData_clicked		//加载数据
			   on_pushButtonStart_clicked			//启动
			   on_pushButtonStop_clicked			//停止
			   timeOut200ms							//定时结束的一些处理

			   saveImage_triggered
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-20     添加offlinetrack.h头部注释
******************************************************************************/

#ifndef _OFFLINETRACK_H
#define _OFFLINETRACK_H

#include "ui_offlinetrack.h"
#include "var.h"
#include "motion.h"
#include "dip.h"
#include "dmc1000.h"

extern bool g_isCameraWorking;

class OfflineTrack :public QWidget
{	
	Q_OBJECT
public:
	explicit OfflineTrack(QWidget *parent=0);
	~OfflineTrack(void);
private:
	Ui_offlineTrack  ui;

	Motion motionOffline;

	double dWeldDistance;
	double weldSpeed;
	bool isWeldCmdTriggered;
	int cycleCount;
	int trackTime;
	int trackCount;
	int dataNum;
	QString filePath;
	QString fileName;

	double horizontalCalibrationRatio;
	double ro;
	double lnro;

	int dataCount;
	QTimer sTimer200ms;

	bool isDataCollectionFinished;
	bool isDataAnalysisFinished;
	bool haveDataOrNot;
	bool hasTrackStarted;
	bool notAllowTrack;

	QVector<double>absOffsetList;
	QVector<double>relOffsetList;
	
	void start200msTiming();
	void stop200msTiming();
	void start100msTiming();
	void stop100msTiming();
	void start150msTiming();
	void stop150msTiming();
	void switchOnWeld();
	void switchOffWeld();

private slots:
	void syncWeldParameters(WeldParameter parameters);
	void syncHorizontalCalibrationRatio(double horizontalCalibrationRatio);
	void on_pushButtonCollectData_clicked();
	void on_pushButtonAnalyzeData_clicked();
	void on_pushButtonRecoverSensorPosition_clicked();
	void on_pushButtonStart_clicked();
	void on_pushButtonStop_clicked();
	void on_pushButtonClearData_clicked();
	void on_pushButtonSaveData_clicked();
	void on_pushButtonLoadData_clicked();
	void timeOut200ms();

signals:
	void saveImage_triggered(QString fileName);
};

#endif