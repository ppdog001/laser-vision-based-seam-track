#pragma once
#ifndef Calibration3_H
#define Calibration3_H
#include "var.h"
#include "ui_calibration3.h"
#include "motion.h"

//拟合校正类
class FittingCalibration:public QWidget
{
	Q_OBJECT
public:
	explicit FittingCalibration(QWidget *parent=0);
	~FittingCalibration();

private:
	Ui_CalibrationMethod3 ui;
	Motion calibrationMethod3MotionObject;
	String calibrationDataPath;
	String validationDataPath;
	String laserStripeDataPath;
	int fileNo;
	int iterationNum;
	int iterationCount;
	unsigned char current_mode;	//当前的工作模式

	Mat bufferLaserStripeImage;

	QTimer sTimer200ms;

	bool isAbruptStopTriggered;

private slots:
	void on_collectCalibrationDataPushButton_clicked();
	void on_collectValidationDataPushButton_clicked();
	void on_collectSeamLaserStripeDataPushButton_clicked();
	void on_returnSensorPushButton_clicked();
	void timeOut200ms();
	void on_terminatePushButton_clicked();

private:
	void start200msTiming();
	void stop200msTiming();

signals:
	void saveCalibrationData_triggered(QString fileName);
	void saveValidationData_triggered(QString fileName);
	void saveSeamLaserStripeData_triggered(QString fileName);
};

#endif