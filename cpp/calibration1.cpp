/******************************************************************************
  FileName: calibration1.cpp
  Author: WillLi		Date:2019-5-18
  Description:
	          SimpleCalibration类的的实现
  Function List:
                1. SimpleCalibration	// 类初始化
                2. ~SimpleCalibration	// 类析构时
                3. on_confirmPushButton_clicked // 计算rho--即水平标定比率
                4. updateHorizontalCalibrationRatio_triggered
  History:
         <author>		<time>     <desc>
          WillLi99    2019-5-18     添加clibration.cpp头部注释和各函数的头部注释  
******************************************************************************/

#include "calibration1.h"
#include "var.h"


/******************************************************************************
  Function:SimpleCalibration
  Description: 
  Calls: 
  Called By: SimpleCalibration类
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
SimpleCalibration::SimpleCalibration(QWidget *parent)
{
	ui.setupUi(this);
}

/******************************************************************************
  Function:~SimpleCalibration
  Description: 
  Calls: 
  Called By: SimpleCalibration类
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
SimpleCalibration::~SimpleCalibration(void)
{
}

/******************************************************************************
  Function:on_pushButtonConfirm_clicked
  Description: 
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SimpleCalibration::on_pushButtonConfirm_clicked()
{
	QString aswString,pswString;
	double asw,psw;
	aswString=ui.lineEditRealCalibrationBlockWidth->text();
	pswString=ui.lineEditPhotographicBlockWidth->text();

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

	dHorizontalCalibrationRatio=asw/psw;
	qDebug()<<"horizontalCalibrationRatio from simple calibration is "<<dHorizontalCalibrationRatio;
	emit updateHorizontalCalibrationRatio_triggered(dHorizontalCalibrationRatio);
	emit updateCalibrationState_triggered();;

	close();
}