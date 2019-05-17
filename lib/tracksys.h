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
#include "var.h"
#include "offlinetrack.h"
#include "calibration3.h"

extern bool testOrNot;


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

	ImagesAcqusition *obj_imageaquisition;		
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
	int cameraIndex;//相机索引号
	int cardCount; 

	QImage frame;

	int imageWidth;
	int imageHeight;

	double fpsValue;
	int m_totalFrame;
	
	int multiImagesNo;
	QString multiImagesFilePath;
	QString multiImagesName;

	double horizontalCalibrationRatio;
	double currentSeamOffset;
	double lastSeamOffset;

	WeldParameter weldparameter;		//焊接参数
	SeamTrackParameter seamtrackparameter;	//跟踪参数
	
	LONG64 receivedFrameNumber;

	SeamType weldSeamType;

	QImage checkico;
	QImage wrongico;
	QImage unknownico;

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

// 普通私有函数
private:
	void initializeVariable();

// slots
private slots:
	
	// pushbutton-related
	void on_quitPushButton_clicked();
	void on_startAutoTrackPushButton_clicked();
	void on_stopAutoTrackPushButton_clicked();
	void on_alignPushButton_clicked();
	void on_turnOnCameraPushButton_clicked();
	void on_turnOffCameraPushButton_clicked();
	void on_turnOnRecognitionPushButton_clicked();
	void on_turnOffRecognitonPushButton_clicked();
	void on_loadTestingDataSetsPushButton_clicked();
	void on_unloadTestingDataSetsPushButton_clicked();
	void on_cameraParametersSettingPushButton_clicked();
	void on_manualControlPushButton_clicked();
	void on_setWeldParametersPushButton_clicked();

	// combox-related
	void on_cameraResolutionComBox_currentIndexChanged(int index);
	
	// munu_related
	void on_actionOnlineTrack_triggered();
	void on_actionOfflineTrack_triggered();
	void on_actionTrapezoidal_triggered();
	void on_actionSigmoid_triggered();
	void on_actionAbout_triggered();
	void on_actionCalibrationMethod1_triggered();
	void on_actionCalibrationMethod2_triggered();
	void on_actionCalibrationMethod3_triggered();
	
	// self-defining
	void display1(QImage img, unsigned char* buffer);
	void display2(QImage image);
	void updateFPSValue();

	void saveSingleImage();
	void saveMultiImages();
	void stopSavingImages();
	
	void updateDIPResult(QImage image);
	void updateDIPCalculationResult(SeamInfo seaminfo);
	void updateHorizontalCalibrationRatio(double ratio);

	void updateWeldParameters(WeldParameter paramters);
	
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


// 信号
signals:
	void saveImages_triggered();
	void stopSavingImages_triggered();
	void autoTrack_triggered(SeamTrackParameter seamTrackingParas);
	void updateHorizontalCalibrationRatio_triggered(double rho);
	void stopAutoTracking_triggered();
	void updateWeldParameters_triggered(WeldParameter parameters);
};