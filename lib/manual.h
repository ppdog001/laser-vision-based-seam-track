#ifndef _MANUAL_H
#define _MANUAL_H

#include "var.h"
#include "ui_manual.h"
#include "motion.h"


//手动控制类
class Manual:public QWidget
{
	Q_OBJECT

public:
	explicit Manual(QWidget* parent=0);
	~Manual();

public:
	Motion* manualMotionObject;	//运动控制对象

private:
	Ui_Manual ui;			//界面

	//以下参数定义中，后缀为A表示对应单轴模式下的变量；后缀为B表示对应多轴模式下的变量

	//***************单轴模式下的参数******************************//
	int axisA;					//轴选择
	int rotationDirectionA;		//转向选择：1正向，2-负向
	int motionModeA;			//运动模式. 1-连续模式；2-定长模式
	int accelerationModeA;		//加速模式。分为0-梯形加速和1-S型加速

	double moveDistanceA;		//移动距离	单位mm
	double maxSpeedA;			//最高速度	单位：mm/s
	double accTimeA;			//加速时间
	//*************************************************************//

	//***************多轴模式下的参数********************************//
	double xMoveDistanceB;	//x移动距离	单位mm
	double xMaxSpeedB;		//x最高速度	单位：mm/s
	double xAccTimeB;		//x加速时间	单位 s
	int xAccModeB;			//x加速模式	0-梯形加速;1-S型加速

	double yMoveDistanceB;	//y移动距离	单位mm
	double yMaxSpeedB;		//y最高速度	单位：mm/s
	double yAccTimeB;		//y加速时间	单位 s
	int yAccModeB;			//y加速模式	0-梯形加速;1-S型加速

	double zMoveDistanceB;	//z移动距离	单位mm
	double zMaxSpeedB;		//z最高速度	单位：mm/s
	double zAccTimeB;		//z加速时间	单位 s
	int zAccModeB;			//z加速模式	0-梯形加速;1-S型加速

	//*************************************************************//

	//**********************焊接参数********************************//
	double weldVoltage;			//焊接电压	单位V
	double weldCurrent;			//焊接电流	单位A
	double feedRate;			//送丝速度	单位m/min
	double gasFlow;				//保护气流量	单位L/min
	int weldSwitchMode;			//焊接启动的模式 0-手动启动；1-随运动启动

	//****************************控制模式****************************//
	int controlMode;			//控制模式，=0单轴模式；=1双轴模式
	//*****************************************************************//
	bool checkPass;				//检测，如果是1表示通过，0表示不通过
	bool weldSwitchedOnFlag;	//焊接启动标志


	void initializeParameters();
	void fillDefaultParamters();
	void updateParameters();
	void switchOnWeld();
	void switchOffWeld();

private slots:
	void on_switchOnWeldPushButton_clicked();
	void on_switchOffWeldPushButton_clicked();
	void on_startPushButton_clicked();
	void on_stopPushButton_clicked();
	void on_returnOriginPushButton_clicked();
	void on_cntnsModeRadioButton_toggled();
	void on_fixedModeRadioButton_toggled();



};

#endif