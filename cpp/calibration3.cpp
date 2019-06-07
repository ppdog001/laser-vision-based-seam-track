/******************************************************************************
  File name: calibration3.cpp
  Author: WillLi99		Date:2019-5-18
  Description:
              FittingCalibration类的实现，此类继承于QWidget，负责拟合标定中的关于采集
			  原始图像和检验精度的图像功能。另外，也可以采集焊缝的激光条纹图像，便于用来
			  进行焊缝表面的3d重建。
  Others: 
  Function List:
                1. on_collectCalibrationDataPushButton_clicked	// 采集标定图像
                2. on_collectValidationDataPushButton_clicked	// 采集验证图片
                3. on_collectSeamLaserStripeDataPushButton_clicked // 采集焊缝激光
				条纹图
                4. on_returnSensorPushButton_clicked	// 使传感器恢复到开始采集的
				位置
                5. timeOut200ms();	// 定时200ms
                6. on_terminatePushButton_clicked();	// 紧急停止
                7. start200msTiming();	// 开启定时200ms
                8. stop200msTiming();	// 停止定时200ms
                9. saveCalibrationData_triggered	// 信号
                10. saveValidationData_triggered	// 信号
                11. saveSeamLaserStripeData_triggered	// 信号        
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-18     添加calbraiton3.cpp头部注释和各函数的头部注释
******************************************************************************/
#include "calibration3.h"

/******************************************************************************
  Function:FittingCalibration
  Description:
              1. 初始化
			  2. 配置信号-槽
  Calls: 
  Called By: FittingCalibration类
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
FittingCalibration::FittingCalibration(QWidget *parent)
{
	ui.setupUi(this);
	strCalibrationDataPath="..//images//calibdata//";
	strValidationDataPath="..//images//validata//";
	strLaserStripeDataPath="..//images//seamdata//";
	intFileNo=0;
	intIterationNum=0;
	intIterationNum=0;
	ucharCurrentMode=0x00;
	isEmergencyStopTriggered=false;
	
	connect(&qTimer200ms, SIGNAL(timeout()), this, SLOT(timeOut200ms()));
}

/******************************************************************************
  Function:FittingCalibration
  Description:
			传感器往Z轴正向以恒定速度1m/s移动40mm,传感器以5frame/s的速率采集图像。
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::on_pushButtonCollectCalibrationData_clicked()
{
	ucharCurrentMode=0x01;
	double dHalfDistance=20;
	double dZVelocity=1;	
	double dFrameRate=5;

	intIterationNum=(2*dHalfDistance/dZVelocity)*dFrameRate;
	intIterationCount=0;
	intFileNo=0;

	qTimer200ms.start(200);
}

/******************************************************************************
  Function:on_collectValidationDataPushButton_clicked
  Description:
			传感器往Z轴负向以恒定速度1m/s移动40mm,传感器以5frame/s的速率采集图像。
			要确保采集标定数据先与采集精度检验数据
  Calls: 
  Called By: FittingCalibration类
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::on_pushButtonCollectValidationData_clicked()
{
	ucharCurrentMode=0x02;
	double dHalfDistance=20;
	double dZVelocity=1;	
	double dFrameRate=5;

	intIterationNum=(2*dHalfDistance/dZVelocity)*dFrameRate;
	intIterationCount=0;
	intFileNo=0;
	qTimer200ms.start(200);
}

/******************************************************************************
  Function:on_pushButtonCollectSeamLaserStripeData_clicked
  Description:
			传感器往Y轴正向以恒定速度0.5mmps移动100mm,传感器以5frame/s的速率采集图像。
			要确保采集标定数据和采集精度检验数据先与采集焊缝激光条纹图像
  Calls: 
  Called By: FittingCalibration类
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::on_pushButtonCollectSeamLaserStripeData_clicked()
{
	ucharCurrentMode=0x04;
	double dHalfDistance;
	double yVelocity=0.5;	//设定y轴的移动速率为0.5mmps
	double frameRate=5;

	intIterationNum=(2*dHalfDistance/yVelocity)*frameRate;
	intIterationNum=0;
	intFileNo=0;
	qTimer200ms.start(200);
}

/******************************************************************************
  Function:on_pushButtonRecoverSensorPosition_clicked
  Description:
			恢复传感器的位置，即令传感器恢复到采集焊缝激光条纹图像之前
  Calls: 
  Called By: FittingCalibration类
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::on_pushButtonRecoverSensorPosition_clicked()
{
	ucharCurrentMode=0x02;	//确保已经采集了标定数据和验证数据
	motionCalibration3.yMove(10,-100,0.5,1);
}

/******************************************************************************
  Function:timeOut200ms
  Description:
			200ms定时结束时，进行一些处理
  Calls: 
  Called By: FittingCalibration类
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::timeOut200ms()
{
	intFileNo++;
	intIterationNum++;

	stop200msTiming();

	//**保存用来进行标定的数据*//
	if((ucharCurrentMode & 0x01)&& (!isEmergencyStopTriggered))
	{
		QString fileName="..//images//calibdata//"+QString::number(intFileNo)+".bmp";
		emit saveCalibrationData_triggered(fileName);

		if(intIterationNum<intIterationNum)
		{
			start200msTiming();
		}
		else
		{
			intIterationNum=0;
			intFileNo=0;
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("标定数据采集完毕！"));
			msgbox.exec();
			return;
		}
	}

	//**保存用来进行精度检验的数据*//
	if((ucharCurrentMode & 0x02) && (!isEmergencyStopTriggered))
	{
		QString fileName="..//images//validata//"+QString::number(intFileNo)+".bmp";
		emit saveValidationData_triggered(fileName);

		if(intIterationNum<intIterationNum)
		{
			start200msTiming();
		}
		else
		{
			intIterationNum=0;
			intFileNo=0;
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("检验数据采集完毕！"));
			msgbox.exec();
			return;
		}
	}

	//**保存焊缝激光条纹图像*//
	if((ucharCurrentMode & 0x04) && !(isEmergencyStopTriggered))
	{
		QString fileName="..//images//seamdata//"+QString::number(intFileNo)+".bmp";
		emit saveSeamLaserStripeData_triggered(fileName);

		if(intIterationNum<intIterationNum)
		{
			start200msTiming();
		}
		else
		{
			intIterationNum=0;
			intFileNo=0;
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("焊缝数据采集完毕！"));
			msgbox.exec();
			return;
		}
	}
	
}

/******************************************************************************
  Function:start200msTiming
  Description:
			开启200ms定时
  Calls: 
  Called By: FittingCalibration类
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::start200msTiming()
{
	qTimer200ms.start(200);
	if(ucharCurrentMode & 0x01)
		motionCalibration3.zMove(1.2,0.2,0.01,1);
	if(ucharCurrentMode & 0x02)
		motionCalibration3.zMove(1.2,-0.2,0.01,1);
	if(ucharCurrentMode & 0x04)
		motionCalibration3.yMove(0.8,0.1,0.01,1);
}

/******************************************************************************
  Function:stop200msTiming
  Description:
			停止200ms定时
  Calls: 
  Called By: FittingCalibration类
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::stop200msTiming()
{
	qTimer200ms.stop();
}

/******************************************************************************
  Function:on_pushButtonTerminate_clicked
  Description:
			紧急停止
  Calls: 
  Called By: FittingCalibration类
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void FittingCalibration::on_pushButtonTerminate_clicked()
{
	isEmergencyStopTriggered=true;
	motionCalibration3.allStop();
	intIterationNum=0;
	intIterationNum=0;
	intFileNo=0;
}

/******************************************************************************
  Function:FittingCalibration
  Description: 析构函数
  Calls: 
  Called By: FittingCalibration类
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
FittingCalibration::~FittingCalibration()
{

}