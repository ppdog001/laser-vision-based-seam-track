/******************************************************************************
  File name: calibration3.h
  Author: WillLi99		Date:2019-5-18
  Description:
              定义了FittingCalibration类，此类继承于QWidget，负责拟合标定中的关于采集
			  原始图像和检验精度的图像功能。另外，也可以采集焊缝的激光条纹图像，便于用来
			  进行焊缝表面的3d重建。
  Others: 
  Function List:
                1. on_pushButtonCollectCalibrationData_clicked	// 采集标定图像
                2. on_pushButtonCollectValidationData_clicked	// 采集验证图片
                3. on_pushButtonCollectSeamLaserStripeData_clicked // 采集焊缝激光
				条纹图
                4. on_pushButtonRecoverSensorPosition_clicked	// 使传感器恢复到开始采集的
				位置
                5. timeOut200ms();	// 定时200ms
                6. on_terminatePushButton_clicked();	// 紧急停止
                7. start200msTiming();	// 开启定时200ms
                8. stop200msTiming();	// 停止定时200ms
                9. saveCalibrationData_triggered	// 信号
                10. saveValidationData_triggered	// 信号
                11. saveSeamLaserStripeData_triggered	// 信号        
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-18     添加calbraiton3.h头部注释
******************************************************************************/

#pragma once
#ifndef Calibration3_H
#define Calibration3_H
#include "common.h"
#include "ui_calibration3.h"
#include "motion.h"

class FittingCalibration:public QWidget
{
	Q_OBJECT
public:
	explicit FittingCalibration(QWidget *parent=0);
	~FittingCalibration();

private:
	Ui_Calibration3 ui;
	Motion motionCalibration3;
	String strCalibrationDataPath;
	String strValidationDataPath;
	String strLaserStripeDataPath;
	int intFileNo;
	int intIterationNum;
	int intIterationCount;
	unsigned char ucharCurrentMode;

	QTimer qTimer200ms;
	bool isEmergencyStopTriggered;

private slots:
	void on_pushButtonCollectCalibrationData_clicked();
	void on_pushButtonCollectValidationData_clicked();
	void on_pushButtonCollectSeamLaserStripeData_clicked();
	void on_pushButtonRecoverSensorPosition_clicked();
	void on_pushButtonTerminate_clicked();
	void timeOut200ms();

private:
	void start200msTiming();
	void stop200msTiming();

signals:
	void saveCalibrationData_triggered(QString fileName);
	void saveValidationData_triggered(QString fileName);
	void saveSeamLaserStripeData_triggered(QString fileName);
};

#endif