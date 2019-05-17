/*****************************************************************************
Copyright: Guangdong Provincial Welding Technology Research Center
Author: Li Yangjin
Date: 2018-8-22
Description:DIP类对传感图像进行图像处理和特征点识别。其输入时Mat类型图形
，输出焊缝中心和预设中心得偏差。由于openCV和QT包含了不同的图像类型，DIP类
还提供了Mat和QImage格式转换的相关接口。

特征点提取算法:
	1.裁剪ROI
	2.滤波（包括图像滤波、弧光信息去除等）
	3.直线拟合（要包括细化）
	4.图像变换（前提是结构光条纹线大体保持水平）
	5.特征点提取

*****************************************************************************/
#include "time.h"
#include "dip.h"

Mat DIP::lastFrame;
int DIP::roiX=0;
int DIP::roiY=0;
int DIP::nth=0;


/*****************************************************************************
Function:void DIP(Mat& recImg)
Description:对传入图形进行处理
Call:
Input:recImg-待处理的图像
Output:
Return:
Others:
*****************************************************************************/
DIP::DIP(Mat receivedImage)
{
	nth=nth+1;
	receivedImage.copyTo(image);
	processLaserStripeImage();
}

/*****************************************************************************
Function:void DIP::getImageSize()
Description:获得原图尺寸
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::getImageSize()
{
	height = image.rows;
	width = image.cols;
}

/*****************************************************************************
Function:void DIP::getROI()
Description:裁剪原图,获得ROI区域，是为了降低运算量。根据投影自适应地裁剪原图，产生大小
	为400*100的ROI图。
Input:
Output:总体上，获得了roi,roiWidth，roiHeight
	这些内容。
Return:
Others:
*****************************************************************************/
void DIP::getROI()
{
	roiWidth=400;
	roiHeight=100;
	roi=gray(Range(roiY,roiY+roiHeight),
		Range(roiX,roiX+roiWidth));
}

/*****************************************************************************
Function:void DIP::getGrayImage()
Description:生成灰度图
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::getGrayImage()
{
	gray = Mat::zeros(Size(image.cols, image.rows), CV_8UC1);
	cvtColor(image, gray, CV_BGR2GRAY);
}


/*****************************************************************************
Function:void DIP::thinLaserStripeImage()
Description:细化图像
Input:filteredImage
Output:thinnedImage，其为对filteredImage进行细化后的图像。binImageOTSU为利用OTSU
	自动阈值分割得到的二值图像
Return:
Others:
*****************************************************************************/
void DIP::thinLaserStripeImage()
{
	filteredImage.convertTo(filteredImageDoubleType,CV_64FC1);
	
	//灰度重心法获得细化图像
	Mat assistA=Mat::ones(Size(roiHeight,1),CV_64FC1);
	vector<double> assist;for(int i=0;i<roiHeight;i++) assist.push_back(i);
	Mat assistB=Mat(assist); 
	Mat assistBT=assistB.t();
	
	Mat num=assistBT*filteredImageDoubleType;
	Mat den=assistA*filteredImageDoubleType;
	thinnedImageInfo=num/den; 
	thinnedImageInfo.convertTo(thinnedImageInfo,CV_8UC1);	//转成UINT8类型

	thinnedImage=Mat::zeros(Size(roiWidth,roiHeight),CV_8UC1);
	uchar* ptr=thinnedImageInfo.ptr<uchar>(0);
	for (int i=0;i<roiWidth;i++)
	{
		if(ptr[i])
		{
			thinnedImage.at<uchar>(ptr[i],i)=255;
		}
	}
		
}

/*****************************************************************************
Function:findFittingData
Description:从thinnedImage中寻找用来做直线拟合的数据点。注意：具体返回多少组数据点，和焊缝类型有关
Input:linePoints
Output:fittedLineEquation，即为拟合后的直线方程
Return:
Others:
*****************************************************************************/
PointMat DIP::findFittingData()
{
	PointMat pointsSet;
	Points points;
	switch(seam)
	{
	case BUTT_WELD:
		for(int i=roiHeight/2-5;i<=roiHeight/2+5;i++)
		{
			uchar* ptr=thinnedImage.ptr<uchar>(i);
			for(int j=0;j<roiWidth;j++)
			{
				if(ptr[j])
				{
					points.push_back(Point2i(i,j));		//找到用来做直线拟合的数据点
				}
			}
		}

		pointsSet.push_back(points);	
		break;
	case LAP_WElD:
		break;
	case GROOVE_WELD:
		break;
	}

	return pointsSet;
}





/*****************************************************************************
Function:void DIP::fitLaserStripeImage()
Description:直线拟合。注意：具体要拟合多少条直线，和焊缝类型有关
Input:linePoints
Output:fittedLineEquation，即为拟合后的直线方程
Return:
Others:
*****************************************************************************/
void DIP::fitLaserStripeImage()
{
	PointMat dataSet;
	// 针对不同的焊缝类型，拟合直线的线段亦有所不同
	dataSet=findFittingData();
	cv::fitLine(dataSet[0], fittedLineEquation, CV_DIST_L2, 0, 0.01, 0.01);

}

/*****************************************************************************
Function:void DIP::findFeaturePoints()
Description:找焊缝特征点。利用道格拉斯算法从thinnedImageInfo获得焊缝特征点
Input:
Output:
Return:返回找到的焊缝特征点，对于对接焊缝，返回两个特征点
Others:
*****************************************************************************/
void DIP::findFeaturePoints()
{
	//用Douglas–Peucker算法获得焊缝特征点
	if(seam==BUTT_WELD)
	{
		int width=thinnedImageInfo.cols;
		Point2i vertexPoint;
		Point2i startPoint;
		Point2i endPoint;

		//找到vertexPoint
		uchar* ptr=thinnedImageInfo.ptr<uchar>();
		uint yValue=thinnedImageInfo.ptr<uchar>()[0];
		for(int i=0;i<width;i++)
		{
			if(i==0)	startPoint=Point2i(0,ptr[i]);
			if(i==width-1)	endPoint=Point2i(width-1,ptr[width-1]);
			if(yValue>ptr[i])
			{
				yValue=ptr[i];
				vertexPoint=Point2i(i,yValue);
			}
		}

		//qDebug()<<"vertex Point is "<<vertexPoint.x<<","<<vertexPoint.y<<endl;
		//qDebug()<<"start Point is "<<startPoint.x<<","<<startPoint.y<<endl;
		//qDebug()<<"endPoint is"<<endPoint.x<<","<<endPoint.y<<endl;

		double dist1=sqrt((double)(vertexPoint.x-startPoint.x)*(vertexPoint.x-startPoint.x)+
			(vertexPoint.y-startPoint.y)*(vertexPoint.y-startPoint.y));

		double dist2=sqrt((double)(vertexPoint.x-endPoint.x)*(vertexPoint.x-endPoint.x)+
			(vertexPoint.y-endPoint.y)*(vertexPoint.y-endPoint.y));

		Point2i featurePointA;
		Point2i featurePointB;

		double maxDist=0;
		double temp=0;
		double tempDist=0;
		Point2i vecTemp;
		Point2i vecA=vertexPoint-startPoint;
		ptr=thinnedImageInfo.ptr<uchar>();
		for(int i=1;i<vertexPoint.x;i++)
		{
			if(ptr[i]==0)
			{
				temp=0;
				tempDist=0;
				continue;
			}
			else{
				vecTemp=Point2i(i,ptr[i])-startPoint;
				temp=abs(det(vecTemp,vecA));
				tempDist=temp/dist1;
			}
			if(maxDist<tempDist)
			{
				maxDist=tempDist;
				featurePointA=Point2i(i,ptr[i]);
			}
		}

		Point2i vecB=vertexPoint-endPoint;
		temp=0;
		tempDist=0;
		maxDist=0;
		for(int i=vertexPoint.x+1;i<endPoint.x;i++)
		{
			if(ptr[i]==0)
			{
				temp=0;
				tempDist=0;
				continue;
			}
			else{
				vecTemp=Point2i(i,ptr[i])-endPoint;
				temp=abs(det(vecTemp,vecB));
				tempDist=temp/dist2;
			}
			if(maxDist<tempDist)
			{
				maxDist=tempDist;
				featurePointB=Point2i(i,ptr[i]);
			}
		}
	
		featurePoints.push_back(featurePointA);
		featurePoints.push_back(featurePointB);
	}
		/*qDebug()<<"featurePointA"<<featurePoints[0].x<<","<<featurePoints[0].y<<endl;
		qDebug()<<"featurePointB"<<featurePoints[1].x<<","<<featurePoints[1].y<<endl;*/
}

/*****************************************************************************
Function:void DIP::generateOutImage()
Description:产生Out图，此图片含有拟合出来的直线和焊缝特征点
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::generateOutImage()
{

	//用小十字架标出特征点的位置
	int horizontalShift=(width-roiWidth)/2;
	Point2i leftFeaturePoint=Point2i(horizontalShift+featurePoints[0].x,featurePoints[0].y+roiY);
	Point2i rightFeaturePoint=Point2i(horizontalShift+featurePoints[1].x,featurePoints[1].y+roiY);
	drawAsterisk(image,leftFeaturePoint,Scalar(0,255,255),2);
	drawAsterisk(image,rightFeaturePoint,Scalar(0,255,255),2);

	//用小十字架标出焊缝中心点的位置
	Point2i absWeldSeamCenterPoint=0.5*(leftFeaturePoint+rightFeaturePoint);
	drawAsterisk(image,absWeldSeamCenterPoint,Scalar(255,0,0),2);

	//用蓝色线框标出结构光条纹图像ROI的位置
	rectangle(image,Point(roiX,roiY),Point(roiX+roiWidth,
		roiY+roiHeight),Scalar(255,0,0),2,8);

	//用蓝色实线标出焊枪中心位置
	line(image,Point2i(roiX+roiWidth/2,roiY),Point2i(roiX+roiWidth/2,roiY+roiHeight),Scalar(255,0,0),2,8);
	
	//标示“ROI”字样和焊缝偏差信息
	String ROITitle="ROI";
	putText(image,ROITitle,Point(roiX,roiY-5), FONT_HERSHEY_SIMPLEX,1.5,
		Scalar(0,0,255),2,8);

	String offsetInfo="Offset:"+std::to_string((long double)offset)+"pixels";
	putText(image,offsetInfo,Point(20,height-40), FONT_HERSHEY_SIMPLEX,1.5,Scalar(0,0,255),2,8);

	//标示焊缝宽度
	double weldSeamWidth=abs(featurePoints[0].x-featurePoints[1].x);
	string weldSeamWidthInfo="Seam Width:"+std::to_string((long double)weldSeamWidth)+"pixels";
	putText(image,weldSeamWidthInfo,Point(20,height-80), FONT_HERSHEY_SIMPLEX,1.5,Scalar(0,0,255),2,8);

	outputImage=image;
	this->out=DIP::mat2QImage(image);
}


/*****************************************************************************
Function:generateOffset()
Description:计算偏差,此偏差用于进行运动控制
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::generateOffset()
{
	double centerx;
	centerx=(featurePoints[0].x+featurePoints[1].x)/2;
	offset=roiWidth/2 - centerx;
}


/*****************************************************************************
Function:Mat DIP:: qImage2Mat(QImage in)
Description:QImage to Mat
Input:in-QImage类型的图像
Output:
Return:matImg-Mat类型的图像
Others:
*****************************************************************************/
Mat DIP:: qImage2Mat(QImage in)
{
	Mat matImage;
	//qDebug()<<in.format();
	//matImage=Mat(in.height(),in.width(),CV_8UC3,(void*)in.constBits(), in.bytesPerLine());
	switch(in.format())
	{
	case QImage::Format_RGB888:
		matImage=Mat(in.height(),in.width(),CV_8UC3,(void*)in.constBits(),in.bytesPerLine());
		cvtColor(matImage,matImage,CV_BGR2RGB);
		break;
	case QImage::Format_Grayscale8:
		matImage = cv::Mat(in.height(), in.width(), CV_8UC1, (void*)in.constBits(), in.bytesPerLine());
		break;
	}

	return matImage;
}

/*****************************************************************************
Function:QImage DIP::mat2QImage(Mat& in)
Description:Mat to QImage
Input:in-Mat类型的图像
Output:
Return:out-QImage类型的图像
Others:
*****************************************************************************/
QImage DIP::mat2QImage(Mat& in)
{
	//mat to QImage
	//QImage out;
	if(in.type()==CV_8UC1)
	{
		QImage img((const unsigned char *)(in.data), in.cols, in.rows, in.cols, QImage::Format_Grayscale8); 
		return img;
	}
	else if(in.type()==CV_8UC3)
	{
		const uchar *pSrc=(const uchar *)in.data;
		QImage out(pSrc,in.cols,in.rows,in.step,QImage::Format_RGB888);
		return out.rgbSwapped();
	}
}


/*****************************************************************************
Function:getWeldSeamType()
Description:焊缝图像分类
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::getWeldSeamType()
{
	seam=BUTT_WELD;
}

/*****************************************************************************
Function:DIP::processLaserStripeImage()
Description:根据焊缝的不同类型，采用不同的处理办法

	包括1. 裁剪ROI、2.滤波、3.直线拟合、4.图像变换、5.特征点提取

Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::processLaserStripeImage()
{
	getImageSize();
	getGrayImage();	//图像处理算法都是基于灰度图
	getROI(); 
	getWeldSeamType();	//分类
	switch(seam)
	{
		case BUTT_WELD:
			filterLaserStripeImage(0);
			thinLaserStripeImage();	
			findFeaturePoints();	
			generateOffset();
			generateOutImage();
			generateDIPResult();
			updateLastFrame();
			break;
		case LAP_WElD:
			break;
		case GROOVE_WELD:
			break;
	}
}

/*****************************************************************************
Function:void DIP::filterLaserStripeImage()
Description：先对相邻两帧结构光传感图像进行“与”操作，再对结构光条纹图像进行滤波
Input:method=0---33中值滤波；=1---55中值滤波；=2---15开运算；=3---37开运算
Output:filteredImage，其表示滤波后的图像
Return:
Others:
*****************************************************************************/
void DIP::filterLaserStripeImage(int method)
{
	//比较相邻两幅图像，两者对应像素的最小值作为图像结果
	Mat lastFrameROI;
	if(nth!=1)
	lastFrameROI=lastFrame(Range(roiY,roiY+roiHeight),Range(
		roiX,roiX+roiWidth));
	else if(nth==1)
		lastFrameROI=roi;

	roi.copyTo(filteredImage);
	for(int i=0;i<roiHeight;i++)
	{
		uchar* ptrA=lastFrameROI.ptr<uchar>();
		uchar* ptrB=roi.ptr<uchar>();
		uchar* ptrC=filteredImage.ptr<uchar>();

		for(int j=0;j<roiWidth;j++)
		{
			if(ptrA[j]<=ptrB[j])
				ptrC[j]=ptrA[j];
			else
				ptrC[j]=ptrB[j];	
		}
	}
	

	Mat ele;
	switch(method)
	{
	case 0:
		medianBlur(roi,filteredImage,3);
		break;
	case 1:
		medianBlur(roi,filteredImage,5);
		break;
	case 2:
		ele = getStructuringElement(MORPH_RECT, Size(5, 1));
		morphologyEx(roi,filteredImage,MORPH_OPEN,ele);
		break;
	case 3:
		ele = getStructuringElement(MORPH_RECT, Size(7, 3));
		morphologyEx(roi,filteredImage,MORPH_OPEN,ele);
		break;
	}

}



void DIP::generateDIPResult()
{
	seaminfo.weldSeamOffset=offset;
	seaminfo.torchHeight=-1;
	seaminfo.weldSeamType=seam;
	seaminfo.weldSeamWidth=abs(featurePoints[0].x-featurePoints[1].x);
}

/*****************************************************************************
Function:getROIPosition
Description:用于获取图像ROI截取位置
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::getROIPosition(Mat& image,int* ROIX,int* ROIY)
{
	*ROIX=image.cols/2-200;

	Mat gray=Mat::zeros(Size(image.cols, image.rows), CV_8UC1);
	cvtColor(image, gray, CV_BGR2GRAY);

	//投影法确定ROIY的位置
	int maxValueAddr=0;
	Mat assist=Mat::ones(Size(1,gray.cols),CV_64FC1);

	/***对接焊缝的I型接口和V型接口可用如下方法***/
	Mat imageClone;gray.copyTo(imageClone);
	imageClone.convertTo(imageClone,CV_64FC1);
	Mat project2Y=imageClone*assist;

	//遍历project2Y,找出其最大值的地址
	int temp=0,i=0;
	for(;i<gray.rows;i++)
	{
		double* pt=project2Y.ptr<double>(i);
		if(temp<*pt)
		{
			maxValueAddr=i;
			temp=*pt;
		}
	}

	*ROIY=maxValueAddr-50;
}

/*****************************************************************************
Function:updateLastFrame
Description:用于获取上一帧的结构光条纹图像
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::updateLastFrame()
{
	lastFrame=gray;
}

/*****************************************************************************
Function:det
Description:求两个向量的行列式
Input:
Output:
Return:
Others:
*****************************************************************************/
double DIP::det(Point2i a,Point2i b)
{
	double res;
	res=a.x*b.y-a.y*b.x;
	return res;
}

/*****************************************************************************
Function:drawDashLine
Description:画虚线
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::drawDashLine(Mat& image,Point pt1, Point pt2,Scalar& color,int thickness)
{
	int singleDashLineDist=15;
	double absDist=sqrt((double)(pt1.x-pt2.x)*(pt1.x-pt2.x)+(pt1.y-pt2.y)*(pt1.y-pt2.y));
	int singleDistLineAmount;
	singleDistLineAmount=(int)(absDist/singleDashLineDist);
	int xDiv=abs(pt2.x-pt1.x)/singleDistLineAmount;
	int yDiv=abs(pt2.y-pt1.y)/singleDistLineAmount;
	
	for(int i=0;i<singleDistLineAmount;i++)
	{
		line(image,Point(pt1.x+i*xDiv,pt1.y+i*yDiv),
			Point(pt1.x+(i+1)*xDiv*9/15,pt1.y+(i+1)*yDiv*9/15),color,thickness);
		line(image,Point(pt1.x+(i+1)*xDiv*11/15,pt1.y+(i+1)*yDiv*11/15),
			Point(pt1.x+(i+1)*xDiv*13/15,pt1.y+(i+1)*yDiv*13/15),color,thickness);
	}
}

/*****************************************************************************
Function:drawAsterisk
Description:标星
Input:
Output:
Return:
Others:
*****************************************************************************/
void DIP::drawAsterisk(Mat& image,Point pt,Scalar& color,int thickness)
{
	line(image,Point2i(pt.x-3,pt.y),Point2i(pt.x+3,pt.y),color,thickness);
	line(image,Point2i(pt.x,pt.y-5),Point2i(pt.x,pt.y+5),color,thickness);
}