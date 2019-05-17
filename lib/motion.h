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
	void yRun(double maxVec,double accTime,int accMode=0);
	void xRun(double maxVec,double accTime,int accMode=0);
	void zRun(double maxVec,double accTime,int accMode=0);
	void yMove(double maxVec,double distance,double accTime,int accMode=0);
	void xMove(double maxVec,double distance,double accTime,int accMode=0);
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