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

enum AcclerationMode
{
	TRAPEZOIDAL,
	SIGMOID
};

enum TrackingDirection
{
	POSITIVE,
	NEGATIVE
};

struct WeldParameter
{
	double weldSpeed;				//焊接速率
	double wireFeedRate;			//送丝速度
	double sheildingGasFlow;		//保护气流量
	double weldVoltage;				//焊接电压
	double weldCurrent;				//焊接电流
	int trackingDistance;			//跟踪距离
	double gasPressure;				//气体压力
};


enum SeamType
{
	BUTT_WELD,
	LAP_WElD,
	GROOVE_WELD
};

struct SeamInfo
{
	double weldSeamOffset;
	double weldSeamWidth;
	double torchHeight;
	SeamType weldSeamType;
};

typedef vector<vector<Point2i>> PointMat;
typedef vector<Point2i> Points;
typedef Vec4f Line;
typedef vector<Point2d> PointsD;

struct SeamTrackParameter
{
	double weldingVelocity;
	double trackingDirection;
	double trackingDistance;
};




#endif