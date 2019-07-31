/******************************************************************************
  File name: var.h
  Author: WillLi99		Date:2019-5-21
  Description:
              此文件包含了整个项目各模块共用的头文件，和自定义的数据类型。             
  Others: 
  Function List:
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-21    添加var.h头部注释
******************************************************************************/

#ifndef _VAR_H
#define _VAR_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <QWidget>
#include <QImage>
#include <QThread>
#include "windows.h"
#include <QWidget>
#include <QMessageBox>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QIODevice>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QTime>
#include <QtWidgets/QMainWindow>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene> 
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "time.h"
#include <QMetaType>

using namespace std;
using namespace cv;

#define TRAPEZOIDAL_ACCELERATION 1
#define SIGMOID_ACCELERATION 2

#define FILTER_MED_33 1
#define FILTER_MED_55 2
#define FILTER_OPEN_15 3
#define FILTER_OPEN_37 4

#define INTENSITY_GRAVITY 1
#define INTENSITY_MAXIMUM 2

#define EXTRACTION_ITERATION 1
#define	EXTRACTION_DIFFERENTIAL 2

#define TRACKMETHOD_REFERENCE 1
#define TRACKMETHOD_GLOBAL_COORDINATE 2
#define TRACKMETHOD_REFERENCE_KALMAN 3
#define TRACKMETHOD_GLOBAL_COORDINATE_KALMAN 3

/******************************************************************************
  Name:AcclerationMode
  Description: 控制卡的加速模式,S型加速比梯形加速更平滑.
  Used By: tracksys.h tracksys.cpp   
  Others: 
******************************************************************************/
enum AcclerationMode
{
	TRAPEZOIDAL,	//梯形加速
	SIGMOID			//S型加速
};

/******************************************************************************
  Name:TrackingDirection
  Description: 焊缝跟踪过程中,焊枪在Y轴上移动方向.
  Used By: tracksys.h tracksys.cpp
  Others: 
******************************************************************************/
enum TrackingDirection
{
	POSITIVE,
	NEGATIVE
};

/******************************************************************************
  Name:WeldParameter
  Description: 焊接工艺参数
  Used By: tracksys.h tracksys.cpp weld.h weld.cpp
  Others: 
******************************************************************************/
struct WeldParameter
{
	double dWeldSpeed;			//焊接速度
	double dWireRate;			//送丝速度
	double dGasFlow;			//保护气流量
	double dWeldVoltage;		//焊接电压
	double dWeldCurrent;		//焊接电流
	double dWeldDistance;		//焊接距离
	double dGasPressure;		//气体压力
};

/******************************************************************************
  Name:SeamType
  Description: 焊缝类型
  Used By: tracksys.h tracksys.cpp var.h
  Others: 
******************************************************************************/
enum SeamType
{
	BUTT_WELD_SEAM,		//I型对接焊缝
	LAP_WELD_SEAM,		//搭接焊缝
	GROOVE_WELD_SEAM	//V型对接焊缝
};

/******************************************************************************
  Name:SeamInfo
  Description: 焊缝信息
  Used By: tracksys.h tracksys.cpp dip.h dip.cpp
  Others: 
******************************************************************************/
struct SeamInfo
{
	double dWeldSeamOffset;
	double dWeldSeamWidth;
	double dRemainingHeight;	//焊缝余高
	SeamType weldSeamType;
};

typedef vector<vector<Point2i>> PointMat;
typedef vector<Point2i> Points;
typedef Vec4f Line;

typedef vector<Point2d> CoordinateSet;


/******************************************************************************
  Name:SeamTrackParameter
  Description: 焊缝跟踪参数
  Used By: tracksys.h tracksys.cpp dip.h dip.cpp
  Others: 
******************************************************************************/
struct SeamTrackParameter
{
	double trackingVelocity;
	double trackingDistance;
	char trackingDirection;
};

#endif