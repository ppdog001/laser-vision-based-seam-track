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