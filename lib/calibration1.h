#pragma once
#ifndef Calibration1_H
#define Calibration1_H

#include "qwidget.h"
#include "ui_calibration1.h"

extern bool hasCalibrated;

//简易校正类
class SimpleCalibration :
	public QWidget
{	
	Q_OBJECT
public:
	explicit SimpleCalibration(QWidget *parent=0);
	~SimpleCalibration(void);
private:
	Ui_CalibrationMethod1 ui;
	double rho;
private slots:
	void on_confirmPushButton_clicked();
signals:
	void updateRHO_triggered(double rho);
	void updateHorizontalCalibrationRatio_triggered(double rho);
};


#endif