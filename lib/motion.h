/******************************************************************************
  File name: motion.h
  Author: WillLi99		Date:2019-5-23
  Description:
              定义了Motion类，此类继承于QThread，负责对运动平台的三条轴进行控制
  Others: 
  Function List:
                Motion
				~Motion
				xRun	//X轴连续运动
				yRun	
				zRun	
				xMove	//X轴定长运动
				yMove	
				zMove	
				xStop	//X轴停止
				yStop	
				zStop	
				allStop	//所有轴停止
				run	
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-23     添加motion.h头部注释
******************************************************************************/


#ifndef MOTION_H
#define MOTION_H

#include "var.h"

#define AXISX 0
#define AXISY 3
#define AXISZ 2

//运动类
class Motion:public QThread
{
	Q_OBJECT
public:
	Motion(void);
	~Motion(void);
public:
	void xRun(double maxVec,double accTime,int accMode=0);
	void yRun(double maxVec,double accTime,int accMode=0);
	void zRun(double maxVec,double accTime,int accMode=0);
	void xMove(double maxVec,double distance,double accTime,int accMode=0);
	void yMove(double maxVec,double distance,double accTime,int accMode=0);
	void zMove(double maxVec,double distance,double accTime,int accMode=0);
	void xStop();
	void yStop();
	void zStop();
	void allStop();
	void run();

private:
	double _coeffX;	//x轴 脉冲频率运动速度转换系数，即pp/s->mm/s
	double _coeffZ;	//z轴 脉冲频率运动速度转换系数，即pp/s->mm/s
	double _coeffY;	//y轴 脉冲频率运动速度转换系数，即pp/s->mm/s

};

#endif