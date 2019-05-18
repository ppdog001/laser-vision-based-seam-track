/*****************************************************************************
Copyright: 
Author: Li Yangjin
Date: 2018-8-21
Description:Motion类用来进行对平台进行手动控制，平台的坐标系如下图所示
                      ^z+
                      |
                      |-----> y+
                     /
                    /
                   <x+

*****************************************************************************/

#include "motion.h"
#include "manual.h"
#include "dmc1000.h"

Manual::Manual(QWidget *parent)
{
	ui.setupUi(this);
	manualMotionObject=new Motion;

	checkPass=0;
	initializeParameters();		//初始化设置manuanlControl ui相关内容
	fillDefaultParamters();		//填充
	manualMotionObject->start();
	d1000_out_bit(4,1);		//初始化设置为高电平,断开继电器
}

Manual::~Manual()
{
	
}

/*****************************************************************************
Function:void Manual::on_startPushButton_clicked()
Description:响应手动控制界面对“开始”按钮的点击事件
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::on_startPushButton_clicked()
{
	updateParameters();			//更新数据，载入各种LineEdit的内容


	//分单轴模式还是多轴模式作不同处理
	if(controlMode==0)
	{
		if(!checkPass)
		{
			QMessageBox msgBox;
			msgBox.setText(QStringLiteral("参数设置出错，未能开始运动"));
			msgBox.exec();
			return;
		}

		//连续运动
		if (ui.cntnsModeRadioButton->isChecked())
		{
			if (rotationDirectionA == 1)	maxSpeedA = abs(maxSpeedA);
			else if (rotationDirectionA == 2)	maxSpeedA = -abs(maxSpeedA);

			switch (axisA)
			{
			case 0:manualMotionObject->xRun(maxSpeedA,accTimeA,accelerationModeA);break;
			case 2:manualMotionObject->zRun(maxSpeedA,accTimeA,accelerationModeA);break;
			case 3:manualMotionObject->yRun(maxSpeedA,accTimeA,accelerationModeA);break;
			}
		}

		//定长运动
		if (ui.fixedModeRadioButton->isChecked())
		{
			//旋向
			if (rotationDirectionA == 1)	moveDistanceA = abs(moveDistanceA);//正向
			else if (rotationDirectionA == 2)	moveDistanceA = -abs(moveDistanceA);//负向

			switch (axisA)
			{
			case 0:manualMotionObject->xMove(maxSpeedA,moveDistanceA,accTimeA,accelerationModeA);break;
			case 2:manualMotionObject->zMove(maxSpeedA,moveDistanceA,accTimeA,accelerationModeA);break;
			case 3:manualMotionObject->yMove(maxSpeedA,moveDistanceA,accTimeA,accelerationModeA);break;
			}
		}
	}
	else if(controlMode==1)
	{
		if(!checkPass)
		{
			QMessageBox msgBox;
			msgBox.setText(QStringLiteral("参数设置出错，未能开始运动"));
			msgBox.exec();
			return;
		}

		
		manualMotionObject->xMove(xMaxSpeedB,xMoveDistanceB,xAccTimeB,xAccModeB);
		manualMotionObject->yMove(yMaxSpeedB,yMoveDistanceB,yAccTimeB,yAccModeB);
		manualMotionObject->zMove(zMaxSpeedB,zMoveDistanceB,zAccTimeB,zAccModeB);

	}
	
}


/*****************************************************************************
Function:voidManual::on_stopPushButton_clicked()
Description:响应手动控制界面对“停止”按钮的点击事件
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::on_stopPushButton_clicked()
{
	manualMotionObject->allStop();
}

/*****************************************************************************
Function:Manual::on_returnOriginPushButton_clicked()
Description:响应手动控制界面对“返回原点”按钮的点击事件
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::on_returnOriginPushButton_clicked()
{
	qDebug()<<"on_returnOriginPushButton_clicked"<<endl;
}


/*****************************************************************************
Function:void Manual::_fillDefaultParamters()
Description:初始化默认参数
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::initializeParameters()
{

	//*************************单轴模式下的默认参数设定**************************//
	axisA=3;					//默认轴号是3,对应选上X轴
	rotationDirectionA=1;		//默认正向
	motionModeA=2;			//默认定长运动
	accelerationModeA=1;		//默认梯形加速
	moveDistanceA=10.0;		//默认移动距离10mm
	maxSpeedA=5.0;		//默认最高速度5mm/s	
	accTimeA=0.2;				//默认加速时间0.5s

	//*********************************************************************//

	//*************************多轴模式下的默认参数设定**************************//
	xMoveDistanceB=0;
	xMaxSpeedB=5;
	xAccTimeB=0.1;
	xAccModeB=0;

	yMoveDistanceB=0;
	yMaxSpeedB=5;
	yAccTimeB=0.1;
	yAccModeB=0;

	zMoveDistanceB=0;
	zMaxSpeedB=5;
	zAccTimeB=0.1;
	zAccModeB=0;
	//************************************************************************//

	//*******************************焊接默认参数设定***************************//
	weldVoltage=25;
	weldCurrent=100;
	feedRate=2;
	gasFlow=0.5;
	weldSwitchMode=0;		//=0为默认手动打开焊接
	
	//************************************************************************//

	//*******************************单轴模式双轴模式设定***************************//
	controlMode=0;
	//************************************************************************//

	//设置_weldingSwitchOnFlag为false
	weldSwitchedOnFlag=false;
}



/*****************************************************************************
Function:void Manual::_fillDefaultParamters()
Description:往各种lineEdit里面填充默认参数。
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::fillDefaultParamters()
{
	QString str1,str2,str3,str4;
	//******************************填充单轴模式下GroupBox的参数**************//
	
	str1 = QString::number((double)moveDistanceA);
	str2 = QString::number((double)maxSpeedA);
	str3 = QString::number((double)accTimeA);

	ui.distanceSettinglineEdit->setText(str1);
	ui.speedSettingLineEdit->setText(str2);
	ui.acceleratTimeTextLabel->setText(str3);

	ui.yRadioButton->setChecked(true);
	ui.CWRadioButton->setChecked(true);
	ui.fixedModeRadioButton->setChecked(true);
	ui.trapeModeRadionButton->setChecked(true);

	//************************************************************************//

	//******************************填充多轴模式下GroupBox的参数**************//

	str1 = QString::number((double)xMoveDistanceB);
	str2 = QString::number((double)xMaxSpeedB);
	str3 = QString::number((double)xAccTimeB);
	ui.xDistanceSettingLineEdit->setText(str1);
	ui.xSpeedSettingLineEdit->setText(str2);
	ui.xAcceleratTimeLineEdit->setText(str3);
	ui.xTrapeModeRadionButton->setChecked(true);

	str1 = QString::number((double)yMoveDistanceB);
	str2 = QString::number((double)yMaxSpeedB);
	str3 = QString::number((double)yAccTimeB);
	ui.yDistanceSettingLineEdit->setText(str1);
	ui.ySpeedSettingLineEdit->setText(str2);
	ui.yAcceleratTimeLineEdit->setText(str3);
	ui.yTrapeModeRadionButton->setChecked(true);

	str1 = QString::number((double)zMoveDistanceB);
	str2 = QString::number((double)zMaxSpeedB);
	str3 = QString::number((double)zAccTimeB);
	ui.zDistanceSettingLineEdit->setText(str1);
	ui.zSpeedSettingLineEdit->setText(str2);
	ui.zAcceleratTimeLineEdit->setText(str3);
	ui.zTrapeModeRadionButton->setChecked(true);

	//************************************************************************//

	//******************************填充焊接参数设置groupbox的内容*****************//
	str1=QString::number((double)weldVoltage);
	str2=QString::number((double)weldCurrent);
	str3=QString::number((double)feedRate);
	str4=QString::number((double)gasFlow);
	ui.weldingVoltageSettingLineEdit->setText(str1);
	ui.weldingCurrentSettingLineEdit->setText(str2);
	ui.feedRateSettingLineEdit->setText(str3);
	ui.gasFlowSettingLineEdit->setText(str4);
	ui.manuallySwitchOnWeldingProcessRadioButton->setChecked(true);
	ui.automaticallySwitchOnWeldingProcessRadioButton->setChecked(false);

	//************************************************************************//

	ui.controlMode1RadioButton->setChecked(true);

}

/*****************************************************************************
Function:void Manual::_updateParameters()
Description:更新运动数据
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::updateParameters()
{
	QString str1,str2,str3,str4;
	double temp1,temp2,temp3,temp4;
	bool conversionState1=true;
	bool conversionState2=true;
	bool conversionState3=true;


	if(ui.controlMode1RadioButton->isChecked())//更新单轴模式下参数
	{
		//******************************更新单轴模式下GroupBox的参数**************//
	
	//轴号
	if (ui.xRadioButton->isChecked())			axisA = 0;
	else if (ui.yRadioButton->isChecked())		axisA = 3;
	else if (ui.zRadioButton->isChecked())		axisA = 2;
	
	//方向
	if (ui.CWRadioButton->isChecked())			rotationDirectionA = 1;	//正向
	else if (ui.CCWRatdioButton->isChecked())	rotationDirectionA = 2;	//负向
	
	//运动模式
	if (ui.cntnsModeRadioButton->isChecked())		motionModeA = 1;		//连续模式
	else if (ui.fixedModeRadioButton->isChecked())	motionModeA = 2;		//定长运动
	
	//加速模式
	if (ui.trapeModeRadionButton->isChecked())		accelerationModeA = 0;	//梯形加速
	else if (ui.sigmoidRadioButton->isChecked())	accelerationModeA = 1;	//S加速

	//距离、速度、加速时间等
	if(ui.distanceSettinglineEdit->isEnabled())	//连续模式下，distanceSettingLineEdit不可用
	{	
		str1 = ui.distanceSettinglineEdit->text();
		temp1 = str1.toDouble();
	}
	else
	{
		str1="";
		temp1=-1;
	}
	str2 = ui.speedSettingLineEdit->text();
	str3 = ui.acceleratTimeTextLabel->text();
	
	temp2 = str2.toDouble();
	temp3 = str3.toDouble();
	
	if (temp1 == 0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("移动距离输入错误，请更正！"));
		msgbox.exec();
		checkPass = 0;		//格式检查不通过
		return;
	}
	
	if (temp2 == 0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("最高速度输入错误，请更正！"));
		msgbox.exec();
		checkPass = 0;		//格式检查不通过
		return;
	}
	
	if (temp3 == 0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("加速时间输入错误，请更正！"));
		msgbox.exec();
		checkPass = 0;
		return;
	}
	
	moveDistanceA = temp1;
	maxSpeedA = temp2;
	accTimeA = temp3;
	
	}
	else if(ui.controlMode2RadioButton->isChecked())//更新多轴模式下参数
	{
		
		//********************************X轴*********************************//
		if(ui.xTrapeModeRadionButton->isChecked()) xAccModeB=0;
		else if(ui.xSigmoidRadioButton->isChecked()) xAccModeB=1;

		str1=ui.xDistanceSettingLineEdit->text();
		str2=ui.xSpeedSettingLineEdit->text();
		str3=ui.xAcceleratTimeLineEdit->text();
		
		temp1=str1.toDouble(&conversionState1);
		temp2 = str2.toDouble(&conversionState2);
		temp3 = str3.toDouble(&conversionState3);

		if(temp1==0.0 && conversionState1==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("移动距离输入错误，请更正！"));
			msgbox.exec();
			checkPass = 0;		//格式检查不通过
			return;
		}

		if(temp2==0.0 && conversionState2==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("最高速度输入错误，请更正！"));
			msgbox.exec();
			checkPass = 0;		//格式检查不通过
			return;
		}

		if(temp3==0.0 && conversionState3==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("加速时间输入错误，请更正！"));
			msgbox.exec();
			checkPass = 0;		//格式检查不通过
			return;
		}

		xMoveDistanceB=temp1;
		xMaxSpeedB=temp2;
		xAccTimeB=temp3;

		//***************************************************************************//

		//**********************************Y轴**************************************//
		if(ui.yTrapeModeRadionButton->isChecked()) yAccModeB=0;
		else if(ui.ySigmoidRadioButton->isChecked()) yAccModeB=1;

		str1=ui.yDistanceSettingLineEdit->text();
		str2=ui.ySpeedSettingLineEdit->text();
		str3=ui.yAcceleratTimeLineEdit->text();

		temp1=str1.toDouble(&conversionState1);
		temp2 = str2.toDouble(&conversionState2);
		temp3 = str3.toDouble(&conversionState3);

		if(temp1==0.0 && conversionState1==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("移动距离输入错误，请更正！"));
			msgbox.exec();
			checkPass = 0;		//格式检查不通过
			return;
		}

		if(temp2==0.0 && conversionState2 == false) 
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("最高速度输入错误，请更正！"));
			msgbox.exec();
			checkPass = 0;		//格式检查不通过
			return;
		}

		if(temp3==0.0 && conversionState3==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("加速时间输入错误，请更正！"));
			msgbox.exec();
			checkPass = 0;		//格式检查不通过
			return;
		}

		yMoveDistanceB=temp1;
		yMaxSpeedB=temp2;
		yAccTimeB=temp3;

		//***************************************************************************//


		//**********************************Z轴**************************************//
		if(ui.zTrapeModeRadionButton->isChecked()) zAccModeB=0;
		else if(ui.zSigmoidRadioButton->isChecked()) zAccModeB=1;

		str1=ui.zDistanceSettingLineEdit->text();
		str2=ui.zSpeedSettingLineEdit->text();
		str3=ui.zAcceleratTimeLineEdit->text();

		temp1 = str1.toDouble(&conversionState1);
		temp2 = str2.toDouble(&conversionState2);
		temp3 = str3.toDouble(&conversionState3);

		if(temp1==0.0 && conversionState1==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("移动距离输入错误，请更正！"));
			msgbox.exec();
			checkPass = 0;		//格式检查不通过
			return;
		}

		if(temp2==0.0 && conversionState2==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("最高速度输入错误，请更正！"));
			msgbox.exec();
			checkPass = 0;		//格式检查不通过
			return;
		}

		if(temp3==0.0 &&conversionState3==false)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("加速时间输入错误，请更正！"));
			msgbox.exec();
			checkPass = 0;		//格式检查不通过
			return;
		}

		zMoveDistanceB=temp1;
		zMaxSpeedB=temp2;
		zAccTimeB=temp3;

		//***************************************************************************//


	}

	//****************************更新焊接参数GroupBox的参数*************************//
	str1=ui.weldingVoltageSettingLineEdit->text();
	str2=ui.weldingCurrentSettingLineEdit->text();
	str3=ui.feedRateSettingLineEdit->text();
	str4=ui.gasFlowSettingLineEdit->text();

	temp1=str1.toDouble();
	temp2=str2.toDouble();
	temp3=str3.toDouble();
	temp4=str4.toDouble();

	if(ui.manuallySwitchOnWeldingProcessRadioButton->isChecked()) weldSwitchMode=0;
	else if(ui.automaticallySwitchOnWeldingProcessRadioButton->isChecked()) weldSwitchMode=1;

	//检查
	if(temp1==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("焊接电压输入错误，请更正！"));
		msgbox.exec();
		checkPass = 0;		//格式检查不通过
		return;
	}

	if(temp2==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("焊接电流输入错误，请更正！"));
		msgbox.exec();
		checkPass = 0;		//格式检查不通过
		return;
	}

	if(temp3==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("送丝速度输入错误，请更正！"));
		msgbox.exec();
		checkPass = 0;		//格式检查不通过
		return;
	}

	if(temp4==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("保护气流速输入错误，请更正！"));
		msgbox.exec();
		checkPass = 0;		//格式检查不通过
		return;
	}

	weldVoltage=temp1;
	weldCurrent=temp2;
	feedRate=temp3;
	gasFlow=temp4;

	//********************************************************************************//

	//**********************************更新控制模式***********************************//
	if(ui.controlMode1RadioButton->isChecked())	controlMode=0;
	else if(ui.controlMode2RadioButton->isChecked()) controlMode=1;
	//**********************************************************************************//

	checkPass = 1;		//检查通过

}

void Manual::on_cntnsModeRadioButton_toggled()
{
	ui.distanceSettinglineEdit->setEnabled(false);
}

void Manual::on_fixedModeRadioButton_toggled()
{
	ui.distanceSettinglineEdit->setEnabled(true);
}



/*****************************************************************************
Function:_switchOnWelding
Description:开启焊接。因为通用输出口4连接了继电器。所以是通过控制继电器4来控制焊接的启动
电平由高变低，开始焊接。
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::switchOnWeld()
{
	d1000_out_bit(4,0);
}


/*****************************************************************************
Function:_switchOffWelding
Description:电平由低到高，关闭焊接。
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::switchOffWeld()
{
	d1000_out_bit(4,1);
}


/*****************************************************************************
Function:on_switchOnWeldingProcessPushButton_clicked
Description:响应“开始焊接”pushButton的点击事件。
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::on_switchOnWeldPushButton_clicked()
{
	switchOnWeld();
}


/*****************************************************************************
Function:on_switchOffWeldingProcessPushButton_clicked
Description:响应“关闭焊接”pushButton的点击事件。
Input:
Output:
Return:
Others:
*****************************************************************************/
void Manual::on_switchOffWeldPushButton_clicked()
{
	switchOffWeld();
}
