#include "weld.h"


Weld::Weld(QWidget *parent)
{
	ui.setupUi(this);
	setDefaultParameters();
}


Weld::~Weld(void)
{
}

/***********************************************************************
@函数名：on_confirmPushButton_clicked
@参数：无
@返回值：无
@功能：响应焊接参数设置界面的“确认”按钮，并将焊接参数传给主UI
@修改信息：
***********************************************************************/
void Weld::on_confirmPushButton_clicked()
{
	int checkResult=checkParameters();
	
	if(checkResult==1)	//检查通过，emit焊接参数
	{
		emit updateWeldParameters_triggled(parameters);
		close();
	}
	else if(checkResult==-1)
	{
		return;
	}
}

void Weld::on_cancelPushButton_clicked()
{
	close();
}

/***********************************************************************
@函数名：setDefaultParameters
@参数：无
@返回值：无
@功能：设置默认的焊接参数，并填充响相应的LineEdit
@修改信息：
***********************************************************************/
void Weld::setDefaultParameters()
{
	weldSpeed=2.0;
	wireFeedRate=2.0;
	sheildingGasFlow=0.5;
	weldVoltage=25.0;
	weldCurrent=100.0;

	ui.weldSpeedLineEdit->setText(QString::number(weldSpeed));
	ui.wireFeedRateLineEdit->setText(QString::number(wireFeedRate));
	ui.sheildingGasFlowLineEdit->setText(QString::number(sheildingGasFlow));
	ui.weldVoltageLineEdit->setText(QString::number(weldVoltage));
	ui.weldCurrentLineEdit->setText(QString::number(weldCurrent));
}

/***********************************************************************
@函数名：checkParameters
@参数：无
@返回值：返回1表示检查通过，返回-1表示检查不通过
@功能：检查参数是否合格，若合格就存入相应的数值，不合格则弹出窗口提醒
@修改信息：
***********************************************************************/
int Weld::checkParameters()
{
	QString str1,str2,str3,str4,str5,str6;
	
	str1=ui.weldSpeedLineEdit->text();
	str2=ui.wireFeedRateLineEdit->text();
	str3=ui.sheildingGasFlowLineEdit->text();
	str4=ui.weldVoltageLineEdit->text();
	str5=ui.weldCurrentLineEdit->text();
	str6=ui.trackingDistanceLineEdit->text();
	
	weldSpeed=str1.toDouble();
	wireFeedRate=str2.toDouble();
	sheildingGasFlow=str3.toDouble();
	weldVoltage=str4.toDouble();
	weldCurrent=str5.toDouble();
	trackingDistance=str6.toDouble();

	//检查
	if(weldSpeed==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("焊接速度格式不正确，请更正！"));
		msgbox.exec();
		return -1;
	}

	if(wireFeedRate==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("送丝速度格式不正确，请更正！"));
		msgbox.exec();
		return -1;
	}

	if(sheildingGasFlow==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("保护气流量格式不正确，请更正！"));
		msgbox.exec();
		return -1;
	}

	if(weldVoltage==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("焊接电压格式不正确，请更正！"));
		msgbox.exec();
		return -1;
	}

	if(weldCurrent==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("焊接电流格式不正确，请更正！"));
		msgbox.exec();
		return -1;
	}

	if(trackingDistance==0.0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("焊接长度格式不正确，请更正！"));
		msgbox.exec();
		return -1;
	}
	//经过排查，没有问题，即可保存相关的参数
	parameters.weldSpeed=weldSpeed;
	parameters.wireFeedRate=wireFeedRate;
	parameters.sheildingGasFlow=sheildingGasFlow;
	parameters.weldVoltage=weldVoltage;
	parameters.weldCurrent=weldCurrent;
	parameters.trackingDistance=trackingDistance;
	
	return 1;
}