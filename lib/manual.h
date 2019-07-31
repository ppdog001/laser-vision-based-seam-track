/******************************************************************************
  File name: manual.h
  Author: WillLi99		Date:2019-5-19
  Description:
              定义了Manual类,此类继承于QWidget,负责负责运动平台的手动控制.在变量名字的
			  后缀是1的表示单轴运动模式的变量;在变量名字的后缀是2的表示多轴运动模式的变量;
  Others: 
  Function List:
                Manual
				~Manual
				initializeConfiguration		//初始化运动配置
				fillDefaultConfiguration	//填充默认的运动配置
				updateConfiguration			//更新运动配置
				switchOnWeld				//开焊接
				switchOffWeld				//关焊接
				on_pushButtonSwitchOnWeld_clicked
				on_pushButtonSwitchOffWeld_clicked
				on_radioButtonContinousMotion_toggled	//切换到连续运动模式
				on_radioButtonFixedMotion_toggled		//切换到固定运动模式
				on_pushButtonStart_clicked				//启动
				on_pushButtonStop_clicked				//制动
				on_pushButtonRecoverWeldTorch_clicked	//焊枪返回原来位置
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-19     添加manual.h头部注释
		   WillLi99	   2019-6-6      修改了部分变量的命名
******************************************************************************/

#ifndef _MANUAL_H
#define _MANUAL_H

#include "common.h"
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
	Motion* motionManual;

private:
	Ui_Manual ui;

	/*单轴模式下的变量*/
	int intAxleMode1;				// 轴号.0-x;2-z;3-y
	int intRotationDirectionMode1;	// 轴移动方向.1-正向;2-负向
	int intMotionMethodMode1;		// 运动模式.1-连续运动;2-定长运动
	int intAccelerationMethodMode1;	// 加速模式.1-梯形加速;2-S型加速

	double dMoveDistanceMode1;	
	double dMaxSpeedMode1;	
	double dAccelerationTimeMode1;	

	/*多轴模式下的变量*/
	double dXMoveDistanceMode2;	
	double dXMaxSpeedMode2;	
	double dXAccelerationTimeMode2;	
	int intXAccelerationMethodMode2;	// 加速模式.1-梯形加速;2-S型加速

	double dYMoveDistanceMode2;	
	double dYMaxSpeedMode2;
	double dYAccelerationTimeMode2;
	int intYAccelerationMethodMode2;	// 加速模式.1-梯形加速;2-S型加速

	double dZMoveDistanceMode2;
	double dZMaxSpeedMode2;	
	double dZAccelerationTimeMode2;
	int intZAccelerationMethodMode2;	// 加速模式.1-梯形加速;2-S型加速


	/*焊接参数*/
	double dWeldVoltage;
	double dWeldCurrent;
	double dWireRate;
	double dGasFlow;
	int intWeldStartCondition;		// 焊接开启条件.1-手动开启;2-运动时自动开启

	/*控制模式*/
	int intControlMode;				// 运动模式.1-单轴模式;2-多轴模式
	
	bool isCheckQualified;
	bool isWeldSwitchedOn;
	
	void initializeConfiguration();
	void fillDefaultConfiguration();
	void checkConfiguration();
	void switchOnWeld();
	void switchOffWeld();

private slots:
	void on_pushButtonSwitchOnWeld_clicked();
	void on_pushButtonSwitchOffWeld_clicked();
	void on_pushButtonStart_clicked();
	void on_pushButtonStop_clicked();
	void on_pushButtonRecoverWeldTorch_clicked();
	void on_radioButtonContinousMotion_toggled();
	void on_radioButtonFixedMotion_toggled();
};

#endif