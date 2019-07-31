/******************************************************************************
  File name: tracksys.h
  Author: WillLi99		Date:2019-5-18
  Description:
              定义了SeamTrackingSys类，此类继承于QMainWindow，负责数据交接，信息
			  显示，调用子UI等工作。               
  Others: 
  Function List:
                 SeamTrackingSys	// 类初始化
                 ~SeamTrackingSys	// 类析构时，释放资源
	             setDefaultUiConfiguration // 设置默认的菜单配置
	             initializeVariable //初始化类的私有变量
				 setReferenceOffset	//设定参考偏差
	             on_pushButtonTurnOnCamera_clicked	// 开相机
	             on_pushButtonTurnOffCamera_clicked	// 关相机
	             loadDefaultCameraConfiguration	// 加载相机的默认配置
	             updateCameraConnectionState // 更新相机连接情况
	             on_cameraResolutionComBox_currentIndexChanged	// 改变分辨率
	             updateCameraResolution	//更新相机分辨率
	             display1	// 显示相机流图像
	             display2	// 显示测试数据集的图像
	             saveCalibrationData	// 保存拟合校正图像
	             saveValidationData	// 保存拟合校正做精度验证的图像
	             saveSeamLaserStripeData	// 保存激光条纹图像
	             on_pushButtonCameraParametersSetting_clicked	// 弹出camera界面
	             updateFPSValue	// 更新相机FPS
	             saveSingleImage	// 保存单张图片
	             saveMultiImages // 连续地保存图像
	             stopSavingImages // 停止保存多张图片
	             saveImageOfflineTrack	// 保存离线模式下的图像
	             startDIP	//开启图像处理
	             updateDIPResult	// 更新识别区的结果
	             updateDIPCalculationResult // 更新图像处理的计算结果
	             updateHorizontalCalibrationRatio	// 更新水平标定比率
	             updateSeamType // 更新焊缝分类结果
	             on_pushButtonLoadTestData_clicked	// 打开测试数据集
	             on_unloadTestingDataSetPushButton_clicked	// 关闭测试数据集
	             on_pushButtonQuit_clicked	// 释放相机资源、释放控制卡
	             on_pushButtonTurnOnRecognition_clicked	// 开图像识别功能
	             on_pushButtonTurnOffRecognition_clicked	// 关图像识别功能
	             updateMotionControllerConnectionState	// 更新控制卡连接情况
	             on_actiontrapezoidal_triggered // 设置运动加速为S型加速
	             on_actionsigmoid_triggered	// 设置运动加速为梯形加速
	             on_pushButtonManualControl_clicked	// 手动控制
	             on_pushButtonAlign_clicked	// 将图像视野中间对准焊缝中心
	             on_pushButtonStartAutoTrack_clicked	// 启动自动跟踪
	             on_pushButtonStopAutoTrack_clicked	// 停止自动跟踪的过程
	             setDefaultWeldParameters	// 设置默认的焊接图像
	             updateWeldParameters	// 更新焊接参数
	             on_pushButtonSetWeldParameters_clicked	// 设置焊接参数
	             closeEvent	// 关闭窗口事件
	             on_actionabout_triggered // 弹出“关于”信息
	             on_actionCalibrationMethod1_triggered	// 进行标定方法1-简易标定
	             on_actionCalibrationMethod2_triggered	// 进行标定方法2-齿耙标定
	             on_actionCalibrationMethod3_triggered	// 进行标定方法3-拟合标定

				 saveImages_triggered();	//保存图像信号
				 stopSavingImages_triggered();		//停止保存图像信号
				 autoTrack_triggered(SeamTrackParameter seamTrackingParas);	//自动跟踪信号
				 updateHorizontalCalibrationRatio_triggered(double rho);		//更新水平标定比率信号
				 stopAutoTracking_triggered();		//停止自动跟踪信号
				 updateWeldParameters_triggered(WeldParameter parameters);		更新焊接参数信号
				 triggerTestingMode_triggered();		//触发测试模式
				 detriggerTestingMode_triggered();		//解除测试模式
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-18     添加tracksys.h头部注释
		   WillLi99    2019-5-19	将testorNot全局变量去掉
		   WillLi99    2019-6-12    添加setReferenceOffset函数
		   Will99	  2019-7-8      删除了actionOnlineTrack和actionOfflineTrack相关内容
		                            增加了trackMethodMenu内容
******************************************************************************/

#pragma once

#include "ui_tracksys.h"
#include "manual.h"
#include "camera.h"
#include "image.h"
#include "dmc1000.h"
#include "motion.h"
#include "calibration1.h"
#include "weld.h"
#include "onlinetrack.h"
#include "dip.h"
#include "common.h"
#include "offlinetrack.h"
#include "calibration3.h"

class SeamTrackingSys : public QMainWindow
{
	Q_OBJECT

public:
	SeamTrackingSys(QWidget *parent = Q_NULLPTR);
	~SeamTrackingSys();
	void closeEvent(QCloseEvent *);

// some ui
private:
	Ui::SeamTrackingSysClass ui_main;
	Manual ui_manual;
	Camera ui_camera;
	SimpleCalibration ui_simplecalibration;
	FittingCalibration ui_fittingcalibration;
	Weld ui_weld;
	OfflineTrack ui_offlinetrack;

// self-defining object
private:

	ImageAcquisition *obj_imageaquisition;		
	OnlineTrack *obj_onlinetrack;		
	Motion obj_automotion;

// Qt Object
private:
	QGraphicsScene *scene1;
	QGraphicsScene *scene2;
	QGraphicsPixmapItem *video0PixItem;
	QGraphicsPixmapItem *video1PixItem;

	QLabel *fpsLabel;	//显示帧率的标签
	QTimer *m_timer;	//定时器

	QTimer sTimer;

// normal variables
private:
	int intCameraIndex;//相机索引号
	int intCardCount; 

	QImage frame;

	int intImageWidth;
	int intImageHeight;

	double dFpsValue;
	int intTotalFrame;
	
	int intMultipleImagesNo;
	QString qsMultipleImagesFilePath;
	QString qsMultipleImagesName;

	double dHorizontalCalibrationRatio;
	double dReferenceOffset;
	double dCurrentOffset;

	WeldParameter wpWeldParameter;
	SeamTrackParameter spSeamTrackParameter;
	SeamInfo siSeamInfo;
	
	LONG64 longReceivedFrameNumber;

	SeamType stWeldSeamType;

	QImage checkico;
	QImage wrongico;
	QImage unknownico;

	int intTrackMethod;
// 开关、标志类
private:
	bool isCameraConnected;	
	bool isControlCardConnected;
	bool cmbNotResponding;
	bool isDIPStarted;
	bool isWeldParametersUpdated;
	bool isSavingMultiImagesCommandTriggered;
	bool isFirstDIPStarted;
	bool isWeldTorchTurnedOn;
	bool isSheldingGasTurnedOn;
	bool isCalibrationFinished;
	bool isAlignmentFinished;
	bool isAutoTrackTriggered;
	bool isTestModeTriggered;

// 普通私有函数
private:
	void initializeVariable();
	void setReferenceOffset();
	void setDefaultSeamTrackParameters();
// slots
private slots:
	
	// pushbutton-related
	void on_pushButtonQuit_clicked();
	void on_pushButtonStartAutoTrack_clicked();
	void on_pushButtonStopAutoTrack_clicked();
	void on_pushButtonAlign_clicked();
	void on_pushButtonTurnOnCamera_clicked();
	void on_pushButtonTurnOffCamera_clicked();
	void on_pushButtonTurnOnRecognition_clicked();
	void on_pushButtonTurnOffRecognition_clicked();
	void on_pushButtonLoadTestData_clicked();
	void on_pushButtonUnloadTestDataSets_clicked();
	void on_pushButtonCameraParametersSetting_clicked();
	void on_pushButtonManualControl_clicked();
	void on_pushButtonSetWeldParameters_clicked();

	// combox-related
	void on_comboxCameraResolution_currentIndexChanged(int index);
	
	// munu_related
	void on_actionTrackMethod1_triggered();
	void on_actionTrackMethod2_triggered();
	void on_actionTrackMethod3_triggered();
	void on_actionTrackMethod4_triggered();
	void on_actionTrapezoidal_triggered();
	void on_actionSigmoid_triggered();
	void on_actionAbout_triggered();
	void on_actionCalibration1_triggered();
	void on_actionCalibration2_triggered();
	void on_actionCalibration3_triggered();
	
	// self-defining
	void display1(QImage img, unsigned char* buffer);
	void display2(QImage image);
	void updateFPSValue();

	void saveSingleImage();
	void saveMultiImages();
	void stopSavingImages();
	
	void updateDIPResult(QImage image);
	void updateDIPCalculationResult(SeamInfo seaminfo);
	void syncHorizontalCalibrationRatio(double horizontalCalibrationRate);
	void syncWeldParameters(WeldParameter parameters);
	void syncSeamTrackParameters(SeamTrackParameter parameters);
	
	void saveImageOfflineTrack(QString fileName);

	void saveCalibrationData(QString fileName);
	void saveValidationData(QString fileName);
	void saveSeamLaserStripeData(QString fileName);

	void startDIP();

	void updateSeamType();
	void updateMotionControllerConnectionState();
	void updateCameraConnectionState();
	void updateCameraResolution();
	void setDefaultUiConfiguration();
	void setDefaultWeldParameters();
	void loadDefaultCameraConfiguration();
	void syncCalibrationState();
	void sendBlockWidthPixel();

// 信号
signals:
	void saveImages_triggered();
	void stopSavingImages_triggered();
	void autoTrack_triggered(SeamTrackParameter seamTrackingParas);
	void updateHorizontalCalibrationRatio_triggered(double rho);
	void stopAutoTracking_triggered();
	void updateWeldParameters_triggered(WeldParameter parameters);
	void triggerTestingMode_triggered();
	void detriggerTestingMode_triggered();
	void sendReferenceOffset_triggered(double dReferenceOffset);
	void updateTrackMethod_triggered(int intTrackMethod);
	void sendBlockWidthPixel_triggered(double dBlockWidthPixel);
};