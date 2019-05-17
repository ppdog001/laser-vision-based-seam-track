#include "calibration1.h"
#include "var.h"


/***********************************************************************
@函数名：SimpleCalibration
@参数：
@返回值：
@功能：SimpleCalibration类的初始化函数
@修改信息：
***********************************************************************/
SimpleCalibration::SimpleCalibration(QWidget *parent)
{
	ui.setupUi(this);
}

/***********************************************************************
@函数名：~SimpleCalibration
@参数：
@返回值：
@功能：SimpleCalibration类的析构函数
@修改信息：
***********************************************************************/
SimpleCalibration::~SimpleCalibration(void)
{
}

/***********************************************************************
@函数名：on_confirmPushButton_clicked
@参数：
@返回值：
@功能：响应对“confirmPushButton"的点击事件
@修改信息：
***********************************************************************/
void SimpleCalibration::on_confirmPushButton_clicked()
{
	QString aswString,pswString;
	double asw,psw,rho;
	aswString=ui.actualSeamWidthLineEdit->text();
	pswString=ui.pictorialSeamWidthLineEdit->text();

	//检测kString和bString是否合法
	asw=aswString.toDouble();
	psw=pswString.toDouble();

	if(0==asw)
	{
		QMessageBox msgbox;
		msgbox.setWindowTitle(QStringLiteral("提示"));
		msgbox.setText(QStringLiteral("输入非法，请更正！"));
		msgbox.exec();
		return;
	}
	
	if(0==psw)
	{
		QMessageBox msgbox;
		msgbox.setWindowTitle(QStringLiteral("提示"));
		msgbox.setText(QStringLiteral("输入非法，请更正！"));
		msgbox.exec();
		return;
	}

	rho=asw/psw;
	qDebug()<<"rho from simple calibration is "<<rho;
	emit updateHorizontalCalibrationRatio_triggered(rho);
	hasCalibrated=true;

	close();
}
