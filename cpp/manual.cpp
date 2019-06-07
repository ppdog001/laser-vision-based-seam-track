/******************************************************************************
  File name: manual.cpp
  Author: WillLi99		Date:2019-5-20
  Description:
              定义了Manual类，此类继承于QWidget，负责运动平台的手动控制
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
           WillLi99    2019-5-20     添加manual.h头部注释
		   WillLi99	   2019-6-6      修改了部分变量的命名
******************************************************************************/

#include "motion.h"
#include "manual.h"
#include "dmc1000.h"

/******************************************************************************
  Function:Manual
  Description:初始化配置
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
Manual::Manual(QWidget *parent)
{
	ui.setupUi(this);
	motionManual=new Motion;

	isCheckQualified=false;
	initializeConfiguration();		//初始化设置manuanlControl ui相关内容
	fillDefaultConfiguration();		//填充
	motionManual->start();
	d1000_out_bit(4,1);		//初始化设置为高电平,断开继电器
}

/******************************************************************************
  Function:~Manual
  Description:析构函数
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
Manual::~Manual()
{
	
}

/******************************************************************************
  Function:initializeConfiguration
  Description: 设置运动参数的初始值。
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Manual::initializeConfiguration()
{

	intAxleMode1=3;
	intRotationDirectionMode1=1;
	intMotionMethodMode1=2;
	intAccelerationMethodMode1=2;
	dMoveDistanceMode1=10.0;
	dMaxSpeedMode1=5.0;		
	dAccelerationTimeMode1=0.2;

	dXMoveDistanceMode2=0.0;
	dXMaxSpeedMode2=5.0;
	dXAccelerationTimeMode2=0.1;
	intXAccelerationMethodMode2=0.0;

	dYMoveDistanceMode2=0.0;
	dYMaxSpeedMode2=5.0;
	dYAccelerationTimeMode2=0.1;
	intYAccelerationMethodMode2=0;

	dZMoveDistanceMode2=0.0;
	dZMaxSpeedMode2=5.0;
	dZAccelerationTimeMode2=0.1;
	intZAccelerationMethodMode2=0;
	
	dWeldVoltage=25.0;
	dWeldCurrent=100.0;
	dWireRate=2.0;
	dGasFlow=0.5;
	intWeldStartCondition=1;	

	intControlMode=1;

	isWeldSwitchedOn=false;
}


/******************************************************************************
  Function:on_pushButtonStart_clicked
  Description:开启运动
  Calls: checkConfiguration
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Manual::on_pushButtonStart_clicked()
{
	checkConfiguration();

	if(intControlMode==1)
	{
		if(!isCheckQualified)
		{
			QMessageBox msgBox;
			msgBox.setText(QStringLiteral("参数设置出错，未能开始运动"));
			msgBox.exec();
			return;
		}

		//连续运动
		if(ui.radioButtonContinousMotion->isChecked())
		{
			if (intRotationDirectionMode1 == 1)	
				dMaxSpeedMode1 = abs(dMaxSpeedMode1);
			else if (intRotationDirectionMode1 == 2)	
				dMaxSpeedMode1 = -abs(dMaxSpeedMode1);

			switch (intAxleMode1)
			{
			case 0:motionManual->xRun(dMaxSpeedMode1,dAccelerationTimeMode1,intAccelerationMethodMode1);break;
			case 2:motionManual->zRun(dMaxSpeedMode1,dAccelerationTimeMode1,intAccelerationMethodMode1);break;
			case 3:motionManual->yRun(dMaxSpeedMode1,dAccelerationTimeMode1,intAccelerationMethodMode1);break;
			}
		}

		//定长运动
		if (ui.radioButtonFixedMotion->isChecked())
		{
			//旋向
			if (intRotationDirectionMode1 == 1)	
				dMoveDistanceMode1 = abs(dMoveDistanceMode1);
			else if (intRotationDirectionMode1 == 2)	
				dMoveDistanceMode1 = -abs(dMoveDistanceMode1);

			switch (intAxleMode1)
			{
			case 0:motionManual->xMove(dMaxSpeedMode1,dMoveDistanceMode1,dAccelerationTimeMode1,intAccelerationMethodMode1);break;
			case 2:motionManual->zMove(dMaxSpeedMode1,dMoveDistanceMode1,dAccelerationTimeMode1,intAccelerationMethodMode1);break;
			case 3:motionManual->yMove(dMaxSpeedMode1,dMoveDistanceMode1,dAccelerationTimeMode1,intAccelerationMethodMode1);break;
			}
		}
	}
	else if(intControlMode==2)
	{
		if(!isCheckQualified)
		{
			QMessageBox msgBox;
			msgBox.setText(QStringLiteral("参数设置出错，未能开始运动"));
			msgBox.exec();
			return;
		}

		
		motionManual->xMove(dXMaxSpeedMode2,dXMoveDistanceMode2,
			dXAccelerationTimeMode2,intXAccelerationMethodMode2);
		motionManual->yMove(dYMaxSpeedMode2,dYMoveDistanceMode2,
			dYAccelerationTimeMode2,intYAccelerationMethodMode2);
		motionManual->zMove(dZMaxSpeedMode2,dZMoveDistanceMode2,
			dZAccelerationTimeMode2,intZAccelerationMethodMode2);
	}
}


/******************************************************************************
  Function:on_pushButtonStop_clicked
  Description: 停止运动
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Manual::on_pushButtonStop_clicked()
{
	motionManual->allStop();
}

/******************************************************************************
  Function:on_pushButtonStart_clicked
  Description:停止运动
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Manual::on_pushButtonRecoverWeldTorch_clicked()
{
	qDebug()<<"on_returnOriginPushButton_clicked"<<endl;
}

/******************************************************************************
  Function:fillDefaultConfiguration
  Description: 填充默认的配置
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Manual::fillDefaultConfiguration()
{
	QString str1,str2,str3,str4;
	
	str1 = QString::number((double)dMoveDistanceMode1);
	str2 = QString::number((double)dMaxSpeedMode1);
	str3 = QString::number((double)dAccelerationTimeMode1);

	ui.lineEditDistance->setText(str1);
	ui.lineEditSpeed->setText(str2);
	ui.lineEditAccelerationTime->setText(str3);

	ui.radioButtonY->setChecked(true);
	ui.radioButtonCW->setChecked(true);
	ui.radioButtonFixedMotion->setChecked(true);
	ui.radioButtonTrapezoidAcceleration->setChecked(true);

	str1 = QString::number((double)dXMoveDistanceMode2);
	str2 = QString::number((double)dXMaxSpeedMode2);
	str3 = QString::number((double)dXAccelerationTimeMode2);
	ui.lineEditXDistance->setText(str1);
	ui.lineEditXMaxSpeed->setText(str2);
	ui.lineEditXAccelerationTime->setText(str3);
	ui.radioButtonXTrapezoidAcceleration->setChecked(true);

	str1 = QString::number((double)dYMoveDistanceMode2);
	str2 = QString::number((double)dYMaxSpeedMode2);
	str3 = QString::number((double)dYAccelerationTimeMode2);
	ui.lineEditYDistance->setText(str1);
	ui.lineEditYMaxSpeed->setText(str2);
	ui.lineEditYAccelerationTime->setText(str3);
	ui.radioButtonYTrapezoidAcceleration->setChecked(true);

	str1 = QString::number((double)dZMoveDistanceMode2);
	str2 = QString::number((double)dZMaxSpeedMode2);
	str3 = QString::number((double)dZAccelerationTimeMode2);
	ui.lineEditZDistance->setText(str1);
	ui.lineEditZMaxSpeed->setText(str2);
	ui.lineEditZAccelerationTime->setText(str3);
	ui.radioButtonZTrapezoidAcceleration->setChecked(true);

	//*********************填充焊接参数GroupBox的参数**************//
	str1=QString::number((double)dWeldVoltage);
	str2=QString::number((double)dWeldCurrent);
	str3=QString::number((double)dWireRate);
	str4=QString::number((double)dGasFlow);
	ui.lineEditWeldVoltage->setText(str1);
	ui.lineEditWeldCurrent->setText(str2);
	ui.lineEditWireRate->setText(str3);
	ui.lineEditGasFlow->setText(str4);
	ui.radioButtonManuallySwitchOnWeld->setChecked(true);
	ui.radioButtonAutomaticallySwitchOnWeld->setChecked(false);

	ui.radioButtonSingleAxleMode->setChecked(true);
}

/******************************************************************************
  Function:checkConfiguration
  Description: 检查设置是否符号
  Calls: 
  Called By: manual.cpp/on_pushButtonStart_Clicked
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Manual::checkConfiguration()
{
	QString str1,str2,str3,str4;
	double temp1,temp2,temp3,temp4;
	bool conversionState1=true;
	bool conversionState2=true;
	bool conversionState3=true;


	if(ui.radioButtonSingleAxleMode->isChecked()) //单轴模式
	{
		//轴号
		if (ui.radioButtonX->isChecked())
			intAxleMode1 = 0;
		else if (ui.radioButtonY->isChecked())		
			intAxleMode1 = 3;
		else if (ui.radioButtonZ->isChecked())		
			intAxleMode1 = 2;
	
		//方向
		if (ui.radioButtonCW->isChecked())
			intRotationDirectionMode1 = 1;
		else if (ui.radioButtonCCW->isChecked())
			intRotationDirectionMode1 = 2;
	
		//运动模式
		if (ui.radioButtonContinousMotion->isChecked())		
			intAccelerationMethodMode1 = 1;
		else if (ui.radioButtonFixedMotion->isChecked())	
			intAccelerationMethodMode1 = 2;
	
		//加速模式
		if (ui.radioButtonTrapezoidAcceleration->isChecked())		
			intAccelerationMethodMode1 = 1;
		else if (ui.radioButtonSigmoidAcceleration->isChecked())	
			intAccelerationMethodMode1 = 2;	

		//运动参数
		if(ui.lineEditDistance->isEnabled())
		{	
			str1 = ui.lineEditDistance->text();
			temp1 = str1.toDouble();
		}
		else
		{
			str1="";
			temp1=-1;
		}
		str2 = ui.lineEditSpeed->text();
		str3 = ui.lineEditAccelerationTime->text();
	
		temp2 = str2.toDouble();
		temp3 = str3.toDouble();
	
		if (temp1 == 0.0)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("移动距离输入错误，请更正！"));
			msgbox.exec();
			isCheckQualified = false;
			return;
		}
	
		if (temp2 == 0.0)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("最高速度输入错误，请更正！"));
			msgbox.exec();
			isCheckQualified = false;
			return;
		}
	
		if (temp3 == 0.0)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("加速时间输入错误，请更正！"));
			msgbox.exec();
			isCheckQualified = false;
			return;
		}
	
		dMoveDistanceMode1 = temp1;
		dMaxSpeedMode1 = temp2;
		dAccelerationTimeMode1 = temp3;
	
	}// end of if(ui.controlMode1RadioButton->isChecked())
	else if(ui.radioButtonMultipleAxlesMode->isChecked())//多轴模式
	{
			//X轴
			if(ui.radioButtonXTrapezoidAcceleration->isChecked()) 
				intXAccelerationMethodMode2=0;
			else if(ui.radioButtonXSigmoidAcceleration->isChecked()) 
				intXAccelerationMethodMode2=1;

			str1=ui.lineEditXDistance->text();
			str2=ui.lineEditXMaxSpeed->text();
			str3=ui.lineEditXAccelerationTime->text();
		
			temp1=str1.toDouble(&conversionState1);
			temp2 = str2.toDouble(&conversionState2);
			temp3 = str3.toDouble(&conversionState3);

			if(temp1==0.0 && conversionState1==false)
			{
				QMessageBox msgbox;
				msgbox.setText(QStringLiteral("移动距离输入错误，请更正！"));
				msgbox.exec();
				isCheckQualified = false;
				return;
			}

			if(temp2==0.0 && conversionState2==false)
			{
				QMessageBox msgbox;
				msgbox.setText(QStringLiteral("最高速度输入错误，请更正！"));
				msgbox.exec();
				isCheckQualified = false;	
				return;
			}

			if(temp3==0.0 && conversionState3==false)
			{
				QMessageBox msgbox;
				msgbox.setText(QStringLiteral("加速时间输入错误，请更正！"));
				msgbox.exec();
				isCheckQualified = false;
				return;
			}

			dXMoveDistanceMode2=temp1;
			dXMaxSpeedMode2=temp2;
			dXAccelerationTimeMode2=temp3;


			//Y轴
			if(ui.radioButtonYTrapezoidAcceleration->isChecked()) 
				intYAccelerationMethodMode2=0;
			else if(ui.radioButtonYSigmoidAcceleration->isChecked()) 
				intYAccelerationMethodMode2=1;

			str1=ui.lineEditYDistance->text();
			str2=ui.lineEditYMaxSpeed->text();
			str3=ui.lineEditYAccelerationTime->text();
	
			temp1=str1.toDouble(&conversionState1);
			temp2 = str2.toDouble(&conversionState2);
			temp3 = str3.toDouble(&conversionState3);

			if(temp1==0.0 && conversionState1==false)
			{
				QMessageBox msgbox;
				msgbox.setText(QStringLiteral("移动距离输入错误，请更正！"));
				msgbox.exec();
				isCheckQualified = false;	
				return;
			}

			if(temp2==0.0 && conversionState2 == false) 
			{
				QMessageBox msgbox;
				msgbox.setText(QStringLiteral("最高速度输入错误，请更正！"));
				msgbox.exec();
				isCheckQualified = false;	
				return;
			}

			if(temp3==0.0 && conversionState3==false)
			{
				QMessageBox msgbox;
				msgbox.setText(QStringLiteral("加速时间输入错误，请更正！"));
				msgbox.exec();
				isCheckQualified = false;		
				return;
			}

			dYMoveDistanceMode2=temp1;
			dYMaxSpeedMode2=temp2;
			dYAccelerationTimeMode2=temp3;

			//Z轴
			if(ui.radioButtonZTrapezoidAcceleration->isChecked())
				intZAccelerationMethodMode2=1;
			else if(ui.radioButtonZSigmoidAcceleration->isChecked()) 
				intZAccelerationMethodMode2=2;

			str1=ui.lineEditZDistance->text();
			str2=ui.lineEditZMaxSpeed->text();
			str3=ui.lineEditZAccelerationTime->text();

			temp1 = str1.toDouble(&conversionState1);
			temp2 = str2.toDouble(&conversionState2);
			temp3 = str3.toDouble(&conversionState3);

			if(temp1==0.0 && conversionState1==false)
			{
				QMessageBox msgbox;
				msgbox.setText(QStringLiteral("移动距离输入错误，请更正！"));
				msgbox.exec();
				isCheckQualified = false;
				return;
			}

			if(temp2==0.0 && conversionState2==false)
			{
				QMessageBox msgbox;
				msgbox.setText(QStringLiteral("最高速度输入错误，请更正！"));
				msgbox.exec();
				isCheckQualified = false;
				return;
			}

			if(temp3==0.0 &&conversionState3==false)
			{
				QMessageBox msgbox;
				msgbox.setText(QStringLiteral("加速时间输入错误，请更正！"));
				msgbox.exec();
				isCheckQualified = false;
				return;
			}
			dZMoveDistanceMode2=temp1;
			dZMaxSpeedMode2=temp2;
			dZAccelerationTimeMode2=temp3;
	}//else if(ui.radioButton2AxlesMode->isChecked())

	//更新焊接参数
	str1=ui.lineEditWeldVoltage->text();
	str2=ui.lineEditWeldCurrent->text();
	str3=ui.lineEditWireRate->text();
	str4=ui.lineEditGasFlow->text();

	temp1=str1.toDouble();
	temp2=str2.toDouble();
	temp3=str3.toDouble();
	temp4=str4.toDouble();

	if(ui.radioButtonManuallySwitchOnWeld->isChecked()) 
		intWeldStartCondition=1;
	else if(ui.radioButtonAutomaticallySwitchOnWeld->isChecked()) 
		intWeldStartCondition=2;

	if(temp1==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("焊接电压输入错误，请更正！"));
		msgbox.exec();
		isCheckQualified = false;
		return;
	}

	if(temp2==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("焊接电流输入错误，请更正！"));
		msgbox.exec();
		isCheckQualified = false;
		return;
	}

	if(temp3==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("送丝速度输入错误，请更正！"));
		msgbox.exec();
		isCheckQualified = false;
		return;
	}

	if(temp4==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("保护气流速输入错误，请更正！"));
		msgbox.exec();
		isCheckQualified = false;
		return;
	}

	dWeldVoltage=temp1;
	dWeldCurrent=temp2;
	dWireRate=temp3;
	dGasFlow=temp4;


	if(ui.radioButtonSingleAxleMode->isChecked())	
		intControlMode=1;
	else if(ui.radioButtonMultipleAxlesMode->isChecked()) 
		intControlMode=2;

	isCheckQualified = true;	
}

/******************************************************************************
  Function:on_radioButtonContinousMotion_toggled
  Description: 切换到连续运动模式
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Manual::on_radioButtonContinousMotion_toggled()
{
	ui.lineEditDistance->setEnabled(false);
}

/******************************************************************************
  Function:on_radioButtonFixedMotion_toggled
  Description: 切换到定长运动模式
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Manual::on_radioButtonFixedMotion_toggled()
{
	ui.lineEditDistance->setEnabled(true);
}

/******************************************************************************
  Function:switchOnWeld
  Description: 开启焊接
  Calls: manual.cpp/switchOnWeld
  Called By: manual.cpp/on_pushButtonSwitchOnWeld_click
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Manual::switchOnWeld()
{
	d1000_out_bit(4,0);
}

/******************************************************************************
  Function:switchOffWeld
  Description: 关闭焊接
  Calls: 
  Called By: manual.cpp/on_pushButtonSwitchOffWeld_click
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Manual::switchOffWeld()
{
	d1000_out_bit(4,1);
}

/******************************************************************************
  Function:switchOffWeld
  Description: 
  Calls: manual.cpp/switchOffWeld
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Manual::on_pushButtonSwitchOnWeld_clicked()
{
	switchOnWeld();
}


/******************************************************************************
  Function:on_pushButtonSwitchOffWeld_clicked
  Description: 
  Calls: manual.cpp/switchOffWeld
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Manual::on_pushButtonSwitchOffWeld_clicked()
{
	switchOffWeld();
}