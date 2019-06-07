/******************************************************************************
  File name: motion.cpp
  Author: WillLi99		Date:2019-5-23
  Description:
              Motion类封装了弧焊平台各轴运动需要用到的函数，Run类函数是连续运动函数，
			  Move类函数是定长运动函数。运动控制基于laisai dmc1000控制卡控制，其中
			  控制卡0轴接弧焊平台x轴，控制卡2轴接弧焊平台z轴，控制卡3轴接弧焊平台y轴。
			  弧焊平台上各轴和方向如下所示：
                                 ^z+
                                 |
                                 |-----> y+
                                 /
                                /
                             <x+
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
           WillLi99    2019-5-23     添加motion.cpp头部注释
******************************************************************************/
#include "motion.h"
#include "dmc1000.h"
#define minVecPPS 100	//控制卡运动起步速度，单位：脉冲数每秒

/******************************************************************************
  Function:Motion::Motion
  Description: 初始化配置
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
Motion::Motion(void)
{
	_coeffX=0.00625;		
	_coeffY=0.0125;		
	_coeffZ=0.00625;	

	//设置脉冲输出模式
	d1000_set_pls_outmode(0,1);
	d1000_set_pls_outmode(2,0);
	d1000_set_pls_outmode(3,0);
}

/******************************************************************************
  Function:Motion::~Motion(void)
  Description: 
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
Motion::~Motion(void)
{

}

/******************************************************************************
  Function:xRun
  Description: x轴连续运动
  Calls: 
  Called By: 
  Input:maxVec-运行最高速度（单位mm/s）;accTime-加速时间（单位s);accMode-加速类型，=1
		为梯形加速，=2为S型加速
  Output: 
  Return:       
  Others: 由于物理接线的差异，有时要对maxVec取负数
******************************************************************************/
void Motion::xRun(double maxVec,double accTime,int accMode)
{
	//maxVec=-maxVec;	
	switch(accMode)
	{
	case 1:d1000_start_tv_move(AXISX,minVecPPS,(long)(maxVec/_coeffX),accTime);break;
	case 2:d1000_start_sv_move(AXISX,minVecPPS,(long)(maxVec/_coeffX),accTime);break;
	}
}

/******************************************************************************
  Function:yRun
  Description: y轴连续运动
  Calls: 
  Called By: 
  Input:  maxVec-运行最高速度（单位mm/s）;accTime-加速时间（单位s);accMode-加速类型，=1
		为梯形加速，=2为S型加速        
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::yRun(double maxVec,double accTime,int accMode)
{
	//maxVec=-maxVec;
	switch(accMode){
	case 1:d1000_start_tv_move(AXISY,minVecPPS,(long)(maxVec/_coeffY),accTime);break;
	case 2:d1000_start_sv_move(AXISY,minVecPPS,(long)(maxVec/_coeffY),accTime);break;
	}	
}

/******************************************************************************
  Function:zRun
  Description: z轴连续运动
  Calls: 
  Called By: 
  Input:  maxVec-运行最高速度（单位mm/s）;accTime-加速时间（单位s);accMode-加速类型，=1
		为梯形加速，=2为S型加速        
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::zRun(double maxVec,double accTime,int accMode)
{
	maxVec=-maxVec;
	switch(accMode)
	{
	case 1:d1000_start_tv_move(AXISZ,minVecPPS,(long)(maxVec/_coeffZ),accTime);break;
	case 2:d1000_start_sv_move(AXISZ,minVecPPS,(long)(maxVec/_coeffZ),accTime);break;
	}
}


/******************************************************************************
  Function:xMove
  Description: x轴定长运动
  Calls: 
  Called By: 
  Input: maxVec-运行最高速度（单位mm/s）;distance-运动距离，正负代表方向（单位mm）;
		accTime-加速时间（单位s);accMode-加速类型         
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::xMove(double maxVec,double distance,double accTime,int accMode)
{
	//distance=-distance;
	switch(accMode)
	{
	case 1:d1000_start_t_move(AXISX,long(distance/_coeffX),minVecPPS,(long)(maxVec/_coeffX),accTime);break;
	case 2:d1000_start_s_move(AXISX,long(distance/_coeffX),minVecPPS,(long)(maxVec/_coeffX),accTime);break;
	}
}

/******************************************************************************
  Function:yMove
  Description: y轴定长运动
  Calls: 
  Called By: 
  Input: maxVec-运行最高速度（单位mm/s）;distance-运动距离，正负代表方向（单位mm）;
		accTime-加速时间（单位s);accMode-加速类型     
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::yMove(double maxVec,double distance,double accTime,int accMode)
{
	switch(accMode)
	{
	case 1:d1000_start_t_move(AXISY,long(distance/_coeffY),minVecPPS,(long)(maxVec/_coeffY),accTime);break;
	case 2:d1000_start_s_move(AXISY,long(distance/_coeffY),minVecPPS,(long)(maxVec/_coeffY),accTime);break;
	}
}

/******************************************************************************
  Function:zMove
  Description: z轴定长运动
  Calls: 
  Called By: 
  Input:   maxVec-运行最高速度（单位mm/s）;distance-运动距离，正负代表方向（单位mm）;
  accTime-加速时间（单位s);accMode-加速类型       
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::zMove(double maxVec,double distance,double accTime,int accMode)
{
	distance=-distance;
	switch(accMode)
	{
	case 1:d1000_start_t_move(AXISZ,long(distance/_coeffZ),minVecPPS,(long)(maxVec/_coeffZ),accTime);break;
	case 2:d1000_start_s_move(AXISZ,long(distance/_coeffZ),minVecPPS,(long)(maxVec/_coeffZ),accTime);break;
	}
}

/******************************************************************************
  Function:xStop
  Description: x轴停止动作
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::xStop()
{
	d1000_decel_stop(AXISX);
}

/******************************************************************************
  Function:yStop
  Description: y轴停止动作
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::yStop()
{
	d1000_decel_stop(AXISY);
}

/******************************************************************************
  Function:zStop
  Description: z轴停止动作
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::zStop()
{
	d1000_decel_stop(AXISZ);
}

/******************************************************************************
  Function:allStop
  Description: 所有轴停止动作
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::allStop()
{
	d1000_decel_stop(0);
	d1000_decel_stop(1);
	d1000_decel_stop(2);
	d1000_decel_stop(3);
}

/******************************************************************************
  Function:run
  Description: 线程启动
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Motion::run()
{
	while(true)
	{
		sleep(500);
	}
}