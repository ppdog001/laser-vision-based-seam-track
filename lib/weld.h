/******************************************************************************
  File name: weld.h
  Author: WillLi99		Date:2019-5-21
  Description:
              定义了Weld类，此类继承于QWidget，负责配置焊接参数。               
  Others: 
  Function List:
             Weld
			 ~Weld
			 on_confirmPushButton_clicked	//检查焊接参数合法性，无误后传给mainwindow
			 on_cancelPushButton_clicked	//退出界面
			 setDefaultParameters			//设置默认的焊接参数
			 checkParameters				//检测填入lineEdit的参数是否合理

			 updateWeldParameters_triggled	//传递焊接参数
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-21    添加weld.h头部注释
******************************************************************************/
#pragma once

#include "ui_weld.h"
#include "var.h"

//焊接参数类
class Weld :
	public QWidget
{	
	Q_OBJECT
public:
	explicit Weld(QWidget *parent=0);
	~Weld(void);
private:
	Ui_Weld  ui;
	double dWeldSpeed;
	double dWireRate;
	double dGasFlow;
	double dWeldVoltage;
	double dWeldCurrent;
	double dWeldDistance;
	WeldParameter wpWeldParameter;

	void setDefaultParameters();
	bool checkWeldParameters();

private slots:
	void on_confirmPushButton_clicked();
	void on_cancelPushButton_clicked();
signals:
	void updateWeldParameters_triggered(WeldParameter paramters);
};
