/******************************************************************************
  File name: calibration1.h
  Author: WillLi99		Date:2019-5-18
  Description:
              定义了SeamTrackingSys类，此类继承于QWidget，负责交易校正的所有功能
  Others: 
  Function List:
                1. SimpleCalibration	// 类初始化
                2. ~SimpleCalibration	// 类析构时
	            3. on_confirmPushButton_clicked // 计算rho--即水平标定比率
	            4. updateHorizontalCalibrationRatio_triggered 
	            
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-18     添加calbraiton1.h头部注释
******************************************************************************/

#pragma once
#ifndef Calibration1_H
#define Calibration1_H

#include "qwidget.h"
#include "ui_calibration1.h"

extern bool hasCalibrated;

class SimpleCalibration :
	public QWidget
{	
	Q_OBJECT
public:
	explicit SimpleCalibration(QWidget *parent=0);
	~SimpleCalibration(void);
private:
	Ui_Calibration1 ui;
	double dHorizontalCalibrationRatio;
private slots:
	void on_pushButtonConfirm_clicked();
signals:
	void updateHorizontalCalibrationRatio_triggered(double hcr);
	void updateCalibrationState_triggered();
};

#endif