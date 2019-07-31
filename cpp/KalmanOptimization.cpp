/******************************************************************************
  File name: KalmanOptimization.cpp
  Author: WillLi99		Date:2019-7-3
  Description:
              KalmanOptimization类的实现。            
  Others: 
  Function List:
				KalmanOptimization			//基本配置
				 ~KalmanOptimization
				initialize
				predict
				correct
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-7-3    KalmanOptimization类的实现
******************************************************************************/
#include "KalmanOptimization.h"


KalmanOptimization::KalmanOptimization(void)
{
	initialize();
}


KalmanOptimization::~KalmanOptimization(void)
{
}

/******************************************************************************
  Function:Weld
  Description:
  Calls: 
  Called By:
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void KalmanOptimization::initialize()
{
	kf.init(stateNum,measurementNum,1);	
	stateNum=2;
	measurementNum=1;
	processNoise = Mat::zeros(stateNum,1,CV_32FC1);
	measurementNoise = Mat::zeros(measurementNum,1,CV_32FC1);
	transitionMatrix = Mat::zeros(stateNum,stateNum,CV_32FC1);
	controlMatrix = Mat::zeros(stateNum,1,CV_32FC1);
	measurementMatrix = Mat::zeros(measurementNum,stateNum,CV_32FC1);

	setIdentity(transitionMatrix);
	kf.transitionMatrix=transitionMatrix;
	
	measurementMatrix=(Mat_<float>(1, 2) << 1, 0);
	kf.measurementMatrix=measurementMatrix;

	setIdentity(kf.processNoiseCov,Scalar::all(1e-5));
	setIdentity(kf.measurementNoiseCov,Scalar::all(1e-1));
	setIdentity(kf.errorCovPost,Scalar::all(1));

	controlMatrix=(Mat_<float>(1, 2) << 1, 0);
	kf.controlMatrix=controlMatrix;
	iterationNum=0;
}



void KalmanOptimization::optimize(Mat measurement)
{
	kf.predict();
	kf.correct(measurement);
	iterationNum++;
	optimizedValue=kf.statePost;
}