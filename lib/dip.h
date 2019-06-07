/******************************************************************************
  File name: dip.h
  Author: WillLi99		Date:2019-6-6
  Description: 定义图像处理类，其任务是处理结构光图像，获得ROI细化图像和计算一些焊缝数据。
  Others: 
  Function List:
				DIP				//DIP类构造函数
				getImageSize	//获得原始图像尺寸
				getGrayImage	//获得原始图的灰度图
				getROI			//截取ROI
  				getWeldSeamType				//焊缝类型分类
  				processLaserStripeImage		//处理激光条纹图像
  				filterLaserStripeImage		//激光条纹图像滤波
  				thinLaserStripeImage		//激光条纹图像细化
  				fitLaserStripeImage			//激光条纹图像拟合
  				findFittingData				//查找用来拟合的数据
  				findFeaturePoints			//定位焊缝特征点
  				updateLastFrame				//更新lastFrame
				generateOutImage			//产生输出图像
  				generateOffset				//产出焊缝偏差
  				generateDIPResult			//产出图像计算结果
  				qImage2Mat					
  				mat2QImage
				getROIPosition				//计算ROI位置
				det							//求两个向量组成矩阵的行列式
				drawDashLine				//画虚线
				drawAsterisk				//标星

  History: 
          <author>		<time>       <desc>
           WillLi99    2019-6-6     添加dip.h头部注释
******************************************************************************/

#pragma once

#ifndef _DIP_H_
#define _DIP_H_
#include <vector>
#include <QImage>
#include "dip.h"
#include "var.h"

//图像处理类
class DIP
{
private:
	enum SeamType seam;
	
	Mat image;
	Mat gray;
	int width;
	int height;

	Mat roi;
	int roiWidth;
	int roiHeight;


	Mat filteredImage;
	Mat filteredImageDoubleType;
	Mat binImageOTSU;
	Mat thinnedImage;

	Mat thinnedImageInfo;	

	Points linePoints;	//用来做直线拟合的点
	Line fittedLineEquation;				//带4个元素的一个向量

	PointsD featurePoints;
	Point2i centerPoint;

private:
	void getImageSize();
	void getGrayImage();
	void getROI();
	void getWeldSeamType();
	
	void processLaserStripeImage();	
	void filterLaserStripeImage(int method=0);
	void thinLaserStripeImage();
	void fitLaserStripeImage();
	PointMat findFittingData();
	void findFeaturePoints();
	void updateLastFrame();

	void generateOutImage();
	void generateOffset();
	void generateDIPResult();
public:
	QImage out;	
	Mat outputImage;
	int offset;
	SeamInfo seaminfo;

	static void getROIPosition(Mat& image, int* ROIX, int* ROIY);

	DIP(Mat receivedImage);

	static Mat qImage2Mat(QImage in);
	static QImage mat2QImage(Mat& in);
	static int roiX;		//ROI在gray中的横轴位置
	static int roiY;		//ROI在gray中的纵轴位置
	static Mat lastFrame;			//上一帧图像
	static int nth;

	double det(Point2i a,Point2i b);
	void drawDashLine(Mat& image,Point pt1, Point pt2,Scalar& color,int thickness);
	void drawAsterisk(Mat& image,Point pt,Scalar& color,int thickness);
};

#endif