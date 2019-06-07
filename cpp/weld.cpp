/******************************************************************************
  File name: weld.h
  Author: WillLi99		Date:2019-5-21
  Description:
              定义了Weld类，此类继承于QWidget，负责配置焊接参数。此文件为类Weld的实现。               
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
#include "weld.h"

/******************************************************************************
  Function:Weld
  Description:
  Calls: 
  Called By:
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
Weld::Weld(QWidget *parent)
{
	ui.setupUi(this);
	setDefaultParameters();
}

/******************************************************************************
  Function:~Weld
  Description:
  Calls: 
  Called By:
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
Weld::~Weld(void)
{
}

/******************************************************************************
  Function:setDefaultParameters
  Description:设置默认的焊接参数。
  Calls: 
  Called By:
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Weld::setDefaultParameters()
{
	dWeldSpeed=2.0;
	dWireRate=2.0;
	dGasFlow=0.5;
	dWeldVoltage=25.0;
	dWeldCurrent=100.0;

	ui.lineEditWeldSpeed->setText(QString::number(dWeldSpeed));
	ui.lineEditWireRate->setText(QString::number(dWireRate));
	ui.lineEditGasFlow->setText(QString::number(dGasFlow));
	ui.lineEditWeldVoltage->setText(QString::number(dWeldVoltage));
	ui.lineEditWeldCurrent->setText(QString::number(dWeldCurrent));
}

/******************************************************************************
  Function:checkWeldParameters
  Description:检查焊接参数合理性。返回true表示检查通过，返回false表示检查不通过
  Calls: 
  Called By:
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
bool Weld::checkWeldParameters()
{
	QString str1,str2,str3,str4,str5,str6;

	str1=ui.lineEditWeldSpeed->text();
	str2=ui.lineEditWireRate->text();
	str3=ui.lineEditWeldCurrent->text();
	str4=ui.lineEditWeldVoltage->text();
	str5=ui.lineEditWeldCurrent->text();
	str6=ui.lineEditWeldDistance->text();

	dWeldSpeed=str1.toDouble();
	dWireRate=str2.toDouble();
	dGasFlow=str3.toDouble();
	dWeldVoltage=str4.toDouble();
	dWeldCurrent=str5.toDouble();
	dWeldDistance=str6.toDouble();

	//检查
	if(dWeldSpeed==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("焊接速度格式不正确，请更正！"));
		msgbox.exec();
		return false;
	}

	if(dWireRate==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("送丝速度格式不正确，请更正！"));
		msgbox.exec();
		return false;
	}

	if(dGasFlow==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("保护气流量格式不正确，请更正！"));
		msgbox.exec();
		return false;
	}

	if(dWeldVoltage==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("焊接电压格式不正确，请更正！"));
		msgbox.exec();
		return false;
	}

	if(dWeldCurrent==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("焊接电流格式不正确，请更正！"));
		msgbox.exec();
		return false;
	}

	if(dWeldDistance==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("焊接长度格式不正确，请更正！"));
		msgbox.exec();
		return false;
	}
	
	//经过排查，没有问题，即可保存相关的参数
	wpWeldParameter.dWeldSpeed=dWeldSpeed;
	wpWeldParameter.dWireRate=dWireRate;
	wpWeldParameter.dGasFlow=dGasFlow;
	wpWeldParameter.dWeldVoltage=dWeldVoltage;
	wpWeldParameter.dWeldCurrent=dWeldCurrent;
	wpWeldParameter.dWeldDistance=dWeldDistance;

	return true;
}

/******************************************************************************
  Function:on_confirmPushButton_clicked
  Description:
  Calls: 
  Called By:
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Weld::on_confirmPushButton_clicked()
{
	bool checkResult=checkWeldParameters();
	
	if(checkResult==true)	//检查通过，emit焊接参数
	{
		emit updateWeldParameters_triggered(wpWeldParameter);
		close();
	}
	else if(checkResult==false)
	{
		return;
	}
}

/******************************************************************************
  Function:on_cancelPushButton_clicked
  Description:退出设置界面
  Calls: 
  Called By:
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Weld::on_cancelPushButton_clicked()
{
	close();
}