/******************************************************************************
  FileName: tracksys.cpp
  Author: WillLi		Date:2019-5-18
  Description:
	          SeamTrackingSys的实现。负责信息的交互，ui交互等。
  Function List:
                1. SeamTrackingSys				// 类初始化
                2. ~SeamTrackingSys				// 类析构时，释放资源
                3. setDefaultUiConfiguration	// 设置默认的菜单配置
                4. initializeVariable			//初始化类的私有变量
                5. on_turnOnCameraPushButton_clicked	// 开相机
                6. on_turnOffCameraPushButton_clicked	// 关相机
                7. loadDefaultCameraConfiguration		// 加载相机的默认配置
                8. updateCameraConnectionState			// 更新相机连接情况
                9. on_cameraResolutionComBox_currentIndexChanged	// 改变分辨率
                10. updateCameraResolution				//更新相机分辨率
                11. display1					// 显示相机流图像
                12. display2					// 显示测试数据集的图像
                13. saveCalibrationData		// 保存拟合校正图像
                14. saveValidationData		// 保存拟合校正做精度验证的图像
                15. saveSeamLaserStripeData	// 保存激光条纹图像
                16. on_cameraParametersSettingPushButton_clicked	// 弹出camera界面
                17. updateFPSValue			// 更新相机FPS
                18. saveSingleImage			// 保存单张图片
                19. saveMultiImages			// 连续地保存图像
                20. stopSavingImages		// 停止保存多张图片
                21. saveImageOfflineTrack	// 保存离线模式下的图像
                22. startDIP				//开启图像处理
                23. updateDIPResult			// 更新识别区的结果
                24. updateDIPCalculationResult			// 更新图像处理的计算结果
                25. updateHorizontalCalibrationRatio	// 更新水平标定比率
                26. updateSeamType			// 更新焊缝分类结果
                27. on_loadTestingDataSetsPushButton_clicked	// 打开测试数据集
                28. on_unloadTestingDataSetPushButton_clicked	// 关闭测试数据集
                29. on_quitPushButton_clicked				// 释放相机资源、释放控制卡
                30. on_turnOnRecognitionPushButton_clicked	// 开图像识别功能
                31. on_turnOffRecognitonPushButton_clicked	// 关图像识别功能
                32. updateMotionControllerConnectionState	// 更新控制卡连接情况
                33. on_actiontrapezoidal_triggered			// 设置运动加速为S型加速
                34. on_actionsigmoid_triggered				// 设置运动加速为梯形加速
                35. on_manualControlPushButton_clicked		// 手动控制
                36. on_alignPushButton_clicked				// 将图像视野中间对准焊缝中心
                37. on_startAutoTrackPushButton_clicked		// 启动自动跟踪
                38. on_stopAutoTrackPushButton_clicked		// 停止自动跟踪的过程
                39. setDefaultWeldParameters				// 设置默认的焊接图像
                40. updateWeldParameters					// 更新焊接参数
                41. on_setWeldParametersPushButton_clicked	// 设置焊接参数
                42. closeEvent								// 关闭窗口事件
                43. on_actionOfflineTrack_triggered			// 离线焊接
                44. on_actionOnlineTrack_triggered			// 在线焊接
                45. on_actionabout_triggered				// 弹出“关于”信息
                46. on_actionCalibrationMethod1_triggered	// 进行标定方法1-简易标定
                47. on_actionCalibrationMethod2_triggered	// 进行标定方法2-齿耙标定
                48. on_actionCalibrationMethod3_triggered	// 进行标定方法3-拟合标定
  History:
         <author>		<time>     <desc>
          WillLi99    2019-5-18     添加tracksys.cpp头部注释和各函数的头部注释  
******************************************************************************/

#include "JHCap.h"
#include "tracksys.h"

/******************************************************************************
 Name:g_isCameraWorking
 Scope:true/false
 Used by:
 Others:
******************************************************************************/
bool g_isCameraWorking=false;


/******************************************************************************
  Function:SeamTrackingSys
  Description:
             1. 注册自定义的数据结构
			 2. 创建一些Qt组件
			 3. 设置Qt信息槽
			 4. 进行一些相机和控制卡的配置
  Calls: 
		d1000_board_init setDefaultUiConfiguration initializeVariable
		setDefaultWeldParameters CameraInit CameraSetResolution
  Called By: SeamTrackingSys类
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
SeamTrackingSys::SeamTrackingSys(QWidget *parent)
	: QMainWindow(parent),scene1(0),scene2(0),video0PixItem(0),video1PixItem(0)
{
	qRegisterMetaType<SeamInfo> ("SeamInfo");
	qRegisterMetaType<WeldParameter> ("WeldParameter");

	qDebug()<<"***AutoTracking System***";
	ui_main.setupUi(this);
	cardCount=d1000_board_init();
	setDefaultUiConfiguration();
	obj_onlinetrack=new OnlineTrack(this);obj_onlinetrack->start();
	obj_imageaquisition=new ImageAcquisition(this);

	//创建场景
	scene1=new QGraphicsScene(this);
	ui_main.video0GraphicsView->setScene(scene1);
	scene2=new QGraphicsScene(this);
	ui_main.video1graphicsView->setScene(scene2);

	//分辨率显示标签
	fpsLabel=new QLabel(this);
	fpsLabel->setAlignment(Qt::AlignHCenter);
	ui_main.statusBar->addWidget(fpsLabel);

	initializeVariable();
	setDefaultWeldParameters();
	
	m_timer=new QTimer(this);

	connect(this,SIGNAL(saveImages_triggered()),obj_imageaquisition,SLOT(setSavingPicturesSwitch()));
	connect(this,SIGNAL(stopSavingImages_triggered()),obj_imageaquisition,SLOT(resetSavingPicturesSwitch()));
	connect(this,SIGNAL(triggerTestingMode_triggered()),obj_imageaquisition,SLOT(tirggerTestingMode()));
	connect(this,SIGNAL(detriggerTestingMode_triggered()),obj_imageaquisition,SLOT(detriggerTestingMode_triggered()));

	connect(this,SIGNAL(autoTrack_triggered(SeamTrackParameter)),obj_onlinetrack,SLOT(setAutoTrackingSwitch(SeamTrackParameter)));
	connect(this,SIGNAL(stopAutoTracking_triggered()),obj_onlinetrack,SLOT(stopAutoTracking()));
	connect(this,SIGNAL(updateHorizontalCalibrationRatio_triggered(double)),obj_onlinetrack,SLOT(updateHorizontalCalibrationRatio(double)));

	connect(this,SIGNAL(updateWeldParameters_triggered(WeldParameter)),&ui_offlinetrack,SLOT(updateWeldParameters(WeldParameter)));
	connect(this,SIGNAL(updateHorizontalCalibrationRatio_triggered(double)),&ui_offlinetrack,SLOT(updateHorizontalRatio(double)));
	connect(&ui_offlinetrack,SIGNAL(saveImage_triggered(QString)),this,SLOT(saveImageOfflineTrack(QString)));

	connect(obj_onlinetrack,SIGNAL(sendDIPResult_triggered(QImage)),this,SLOT(updateDIPResult(QImage)));
	connect(obj_onlinetrack,SIGNAL(sendDIPCalculationResult_triggered(SeamInfo)),this,SLOT(updateDIPCalculationResult(SeamInfo)));
	connect(obj_onlinetrack,SIGNAL(allowNextDIP_triggered()),this,SLOT(startDIP()));

	connect(obj_imageaquisition,SIGNAL(captured(QImage,unsigned char *)),this,SLOT(display1(QImage,unsigned char *)));
	connect(obj_imageaquisition,SIGNAL(captured2(QImage)),this,SLOT(display2(QImage)));

	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateFPSValue()));

	connect(&ui_camera,SIGNAL(saveSingleImage_triggered()),this,SLOT(saveSingleImage()));
	connect(&ui_camera,SIGNAL(saveMultiImages_triggered()),this,SLOT(saveMultiImages()));
	connect(&ui_camera,SIGNAL(stopSavingImages_triggered()),this,SLOT(stopSavingImages()));
		
	connect(&ui_simplecalibration,SIGNAL(updateHorizontalCalibrationRatio_triggered(double)),this,SLOT(updateHorizontalCalibrationRatio(double)));
	connect(&ui_simplecalibration,SIGNAL(updateCalibrationState_triggered()),this,SLOT(updateCalibrationState()));
	connect(&ui_weld,SIGNAL(updateWeldParameters_triggled(WeldParameter)),this,SLOT(updateWeldParameters(WeldParameter)));
	
	connect(&ui_fittingcalibration,SIGNAL(saveCalibrationData_triggered(QString)),this,SLOT(saveCalibrationData(QString)));
	connect(&ui_fittingcalibration,SIGNAL(saveValidationData_triggered(QString)),this,SLOT(saveValidationData(QString)));
	connect(&ui_fittingcalibration,SIGNAL(saveSeamLaserStripeData_triggered(QString)),this,SLOT(saveSeamLaserStripeData(QString)));



	m_timer->start(1000);

	/***检查相机连接状态***/
	int cameraCount=0;
	CameraGetCount(&cameraCount);
	if(cameraCount>0)
	{
		isCameraConnected=true;
		ui_main.cameraStateDisplayLabel->setPixmap(QPixmap::fromImage(checkico));
		CameraInit(cameraIndex);
		CameraSetResolution(cameraIndex,5,&imageWidth,&imageHeight);
		cmbNotResponding=true;
		cmbNotResponding=false;
		
		qDebug()<<"Resolution= "<<imageWidth<<"*"<<imageHeight;

	}
	else if(cameraCount==0)
	{
		isCameraConnected=false;
		ui_main.cameraStateDisplayLabel->setPixmap(QPixmap::fromImage(wrongico));
	}
	
	d1000_out_bit(4,1);
}


/******************************************************************************
  Function:~SeamTrackingSys
  Description: 卸载控制卡和delete一些内容
  Calls: 
		d1000_board_close
  Called By: SeamTrackingSys类
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
SeamTrackingSys::~SeamTrackingSys()
{
	d1000_board_close();
	delete scene1;
	delete scene2;
	delete obj_imageaquisition;
}


/******************************************************************************
  Function:setDefaultUiConfiguration
  Description: 设置菜单栏和主界面上的一些内容
  Calls: 
		1. updateSeamType
		2. updateMotionControllerConnectionState
		3. updateCameraConnectionState
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::setDefaultUiConfiguration()
{
	//设置菜单栏选项卡的默认选项
	ui_main.actionOnlineTrack->setCheckable(true);
	ui_main.actionOfflineTrack->setCheckable(true);
	ui_main.actionTrapezoidal->setCheckable(true);
	ui_main.actionSigmoid->setCheckable(true);

	ui_main.actionCalibrationMethod1->setCheckable(true);
	ui_main.actionCalibrationMethod2->setCheckable(true);

	ui_main.actionOnlineTrack->setChecked(true);
	ui_main.actionOfflineTrack->setChecked(false);

	ui_main.actionTrapezoidal->setChecked(true);
	ui_main.actionSigmoid->setChecked(false);

	ui_main.actionNormal->setChecked(true);
	ui_main.actionPID->setChecked(false);

	ui_main.actionCalibrationMethod1->setCheckable(false);
	ui_main.actionCalibrationMethod2->setCheckable(false);

	//设置焊接控制栏的默认参数

	isWeldTorchTurnedOn=false;

	ui_main.switchOffRadioButton->setChecked(true);
	ui_main.switchOnRadioButton->setChecked(false);

	updateSeamType();
	updateMotionControllerConnectionState();
	updateCameraConnectionState();

}

/******************************************************************************
  Function:initializeVariable
  Description: 初始化SeamTrackingSys类的局部变量
  Calls: 
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::initializeVariable()
{
	cameraIndex=0;
	fpsValue=0;
	m_totalFrame=0;
	receivedFrameNumber=0;
	multiImagesNo=0;
	multiImagesFilePath="..\\Images\\Multi";

	isDIPStarted=false;
	isAlignmentFinished=false;
	isWeldParametersUpdated=false;
	isFirstDIPStarted=false;
	isSavingMultiImagesCommandTriggered=false;
	isCalibrationFinished=false;
	isTestModeTriggered=false;

	weldSeamType=BUTT_WELD_SEAM;

	checkico=QImage("check_16px.ico");
	wrongico=QImage("wrong_16px.ico");
	unknownico=QImage("unknown_16px.ico");
}


/******************************************************************************
  Function:on_turnOnCameraPushButton_clicked
  Description: 开启相机线程
  Calls: 
        1. CameraGetResolutionCount
		2. loadDefaultCameraConfiguration
		3. CameraGetResolution
		4. CameraSetResolution
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 凡是类似于on_****_clicked或者on_****_triggered等都是一种动作相应函数
******************************************************************************/
void SeamTrackingSys::on_turnOnCameraPushButton_clicked()
{
	if(isCameraConnected==true)
	{

		isTestModeTriggered=false;
		obj_imageaquisition->start();
		obj_imageaquisition->stream();			//obj_imageaquisition开始运作

		//更新分辨率列表
		int count;
		CameraGetResolutionCount(cameraIndex,&count);
		loadDefaultCameraConfiguration();
		for(int i=0; i<count; i++)
		{
			int imageWidth, imageHeight;
			CameraGetResolution(cameraIndex,i, &imageWidth, &imageHeight);
			ui_main.cameraResolutionComBox->addItem(QString("%1x%2").arg(imageWidth).arg(imageHeight));
		}

		CameraSetResolution(cameraIndex,5,0,0);//设置默认分辨率是800*600
		g_isCameraWorking=true;
	}
	else
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("相机未连接，请连接相机！"));
		msgbox.exec();
	}
}

/******************************************************************************
  Function:on_turnOffCameraPushButton_clicked
  Description: 关闭相机
  Calls: 
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_turnOffCameraPushButton_clicked()
{
	isTestModeTriggered=true;
	isDIPStarted=false;
	g_isCameraWorking=false;
	obj_imageaquisition->pause();

	if(!frame.isNull())
	{
		QImage img1=frame.scaled(ui_main.video0GraphicsView->width(),ui_main.video0GraphicsView->height());
		video0PixItem=scene1->addPixmap(QPixmap::fromImage(img1));
		scene1->setSceneRect(0,0,img1.width(),img1.height());
	}
}

/******************************************************************************
  Function:loadDefaultCameraConfiguration
  Description: 加载默认的相机配置
  Calls:
        1. CameraSetGamma
        2. CameraSetContrast
		3. CameraSetGain
		4. CameraSetExposure
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::loadDefaultCameraConfiguration()
{
	QString fileName="..\\camera\\default.xml";
	QFile file(fileName);

	if(!file.open(QFile::ReadOnly|QFile::Text))
	{
		std::cerr<<"Error::Cannot read file"<<qPrintable(fileName)
			<<":"<<qPrintable(file.errorString())<<std::endl;
	}

	QXmlStreamReader xmlReader;
	xmlReader.setDevice(&file);
	xmlReader.readNext();

	while(!xmlReader.atEnd() && !xmlReader.hasError())
	{
		QXmlStreamReader::TokenType token=xmlReader.readNext();
		if(token==QXmlStreamReader::StartDocument) continue;
		if(token==QXmlStreamReader::StartElement)
		{
			if(xmlReader.name()==QStringLiteral("相机参数"))
			{
				continue;
			}
			else if(xmlReader.name()=="Gamma")
			{
				double gamma=xmlReader.readElementText().toInt()/100.0;
				CameraSetGamma(cameraIndex,gamma);
			}
			else if(xmlReader.name()=="Contrast")
			{
				double contrast=xmlReader.readElementText().toInt()/100.0;
				CameraSetContrast(cameraIndex,contrast);
			}
			else if(xmlReader.name()=="Gain")
			{
				CameraSetGain(cameraIndex,xmlReader.readElementText().toInt());
			}
			else if(xmlReader.name()=="Exposure")
			{
				CameraSetExposure(cameraIndex,xmlReader.readElementText().toInt());
			}
		}
	}

	if(xmlReader.hasError())
	{
		QMessageBox::critical(this,
			"xmlFile.xml Parse Error",xmlReader.errorString(),
			QMessageBox::Ok);
		return;
	}
}

/******************************************************************************
  Function:updateCameraConnectionState
  Description: 更新相机的连接状态
  Calls:
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::updateCameraConnectionState()
{
	if(isCameraConnected)
	{
		ui_main.cameraStateDisplayLabel->setPixmap(QPixmap::fromImage(checkico));
	}
	else
	{
		ui_main.cameraStateDisplayLabel->setPixmap(QPixmap::fromImage(wrongico));
	}
}


/******************************************************************************
  Function:on_cameraResolutionComBox_currentIndexChanged
  Description: 改变相机的分辨率
  Calls:
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_cameraResolutionComBox_currentIndexChanged(int index)
{
	CameraGetResolution(cameraIndex,index,&imageWidth,&imageHeight);
	if(cmbNotResponding==false)
	{
		obj_imageaquisition->pause();
		CameraSetResolution(cameraIndex,index, 0, 0);
		obj_imageaquisition->stream();
	}
}

/******************************************************************************
  Function: updateCameraResolution
  Description: 设置相机分辨率为480*640？
  Calls:
  Called By: SeamTrackingSys
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::updateCameraResolution()
{
	QString cameraResolutionString="480*640";
	ui_main.cameraResolutionDisplayLabel->setText(cameraResolutionString);
}


/******************************************************************************
  Function: display1
  Description: 显示相机采集的图片
  Calls:
  Called By: 
  Input: img-图像 buffer-图像的内存位置
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::display1(QImage img, unsigned char* buffer)
{
	if(video0PixItem)
	{
		scene1->removeItem(video0PixItem);
		delete video0PixItem;
		video0PixItem=0;
	}

	frame=img.copy(0,0,img.width(),img.height());	//拷贝图像,深拷贝

	if(isSavingMultiImagesCommandTriggered)
	{
		multiImagesNo++;
		multiImagesName=multiImagesFilePath+QString::number(multiImagesNo)+".bmp";
		frame.save(multiImagesName,0,-1);
	}

	/***固定大小显示***/
	if(!img.isNull())
	{
		QImage img1=img.scaled(ui_main.video0GraphicsView->width(),ui_main.video0GraphicsView->height());
		video0PixItem=scene1->addPixmap(QPixmap::fromImage(img1));
		scene1->setSceneRect(0,0,img1.width(),img1.height());
	}
	if(isDIPStarted && !isTestModeTriggered && !isFirstDIPStarted)
	{
		obj_onlinetrack->dip(frame);
		isFirstDIPStarted=true;//以后由startDIP负责发送图像处理
	}

	m_totalFrame++;
	delete [] buffer;
}

/******************************************************************************
  Function: display2
  Description: 显示测试图像
  Calls:
  Called By: 
  Input: image-测试图像
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::display2(QImage image)
{
	if(video0PixItem)
	{
		scene1->removeItem(video0PixItem);
		delete video0PixItem;
		video0PixItem=0;
	}

	frame=image.copy(0,0,image.width(),image.height());
	QImage img1=image.scaled(ui_main.video0GraphicsView->width(),ui_main.video0GraphicsView->height());
	video0PixItem=scene1->addPixmap(QPixmap::fromImage(img1));
	scene1->setSceneRect(0,0,img1.width(),img1.height());

	if(isDIPStarted)
	{
		obj_onlinetrack->dip(frame);
	}
	m_totalFrame++;
}

/******************************************************************************
  Function: saveCalibrationData
  Description: 保存用于校正的图片
  Calls:
  Called By:  
  Input: fileName-图像的目录和文件名
  Output: 
  Return:       
  Others: 响应ui_fittingcalibration的saveCalibrationData_triggered
******************************************************************************/
void SeamTrackingSys::saveCalibrationData(QString fileName)
{
	frame.save(fileName,0,-1);
}

/******************************************************************************
  Function: saveValidationData
  Description: 保存用于验证校正精度的图片
  Calls:
  Called By:  
  Input: fileName-图像的目录和文件名
  Output: 
  Return:       
  Others: 响应ui_fittingcalibration的saveValidationData_triggered
******************************************************************************/
void SeamTrackingSys::saveValidationData(QString fileName)
{
	frame.save(fileName,0,-1);
}

/******************************************************************************
  Function: saveSeamLaserStripeData
  Description: 保存焊缝轮廓的结构光条纹图像
  Calls:
  Called By:  
  Input: fileName-图像的目录和文件名
  Output: 
  Return:       
  Others: 响应ui_fittingcalibration的saveSeamLaserStripeData_triggered。这个功能
		采集的图像可用于进行焊缝的三维重建
******************************************************************************/
void SeamTrackingSys::saveSeamLaserStripeData(QString fileName)
{
	frame.save(fileName,0,-1);
}


/******************************************************************************
  Function: on_cameraParametersSettingPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others:
******************************************************************************/
void SeamTrackingSys::on_cameraParametersSettingPushButton_clicked()
{
	ui_camera.show();	//弹出cameraControl界面
}

/******************************************************************************
  Function: updateFPSValue
  Description: 更新帧率显示
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others:
******************************************************************************/
void SeamTrackingSys::updateFPSValue()
{
	/*if(fpsValue==0) fpsValue=m_totalFrame;
	else fpsValue=0.4*fpsValue+0.6*m_totalFrame;*/
	fpsValue=m_totalFrame;
	m_totalFrame=0;
	fpsLabel->setText(QString("FPS:%1").arg(QString::number(fpsValue, 'f', 2)));
}

/******************************************************************************
  Function: saveImage
  Description: 保存图片
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others:
******************************************************************************/
void SeamTrackingSys::saveSingleImage()
{

	obj_imageaquisition->pause();

	/***保存单张图片***/
	QString fileName;
	fileName=QFileDialog::getSaveFileName(this,QStringLiteral("保存"),"..","Images(*.png *.bmp *.jpg)");

	//qDebug()<<fileName;

	if(!fileName.isNull())
	{
		if(!frame.isNull())
		{

			bool retV=false;
			retV=frame.save(fileName,0,-1);
			if(retV)
			{
				QMessageBox msgbox;
				msgbox.setText(QStringLiteral("已保存")+fileName);
				msgbox.exec();
				obj_imageaquisition->stream();
			}
		}
		else
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("当前无图片！"));
			msgbox.exec();
			return;
		}

	}
	else
	{
		return;
	}
}

/******************************************************************************
  Function: saveMultiImages
  Description: 保存多张图片，保存目录"../Images/"
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others:
******************************************************************************/
void SeamTrackingSys::saveMultiImages()
{
	//emit saveImages_triggered();	//此signal交给obj_imageaquisition处理
	isSavingMultiImagesCommandTriggered=true;
}


/******************************************************************************
  Function: stopSavingImages
  Description: 停止保存多张图片
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others:
******************************************************************************/
void SeamTrackingSys::stopSavingImages()
{
	//emit stopSavingImages_triggered();//此signal交给obj_imageaquisition处理
	isSavingMultiImagesCommandTriggered=false;
	multiImagesNo=0;
}

/******************************************************************************
  Function: saveImageOfflineTrack
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others:
******************************************************************************/
void SeamTrackingSys::saveImageOfflineTrack(QString fileName)
{
	frame.save(fileName,0,-1);
}

/******************************************************************************
  Function: startDIP
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::startDIP()
{
	if(isDIPStarted && isFirstDIPStarted)
		obj_onlinetrack->dip(frame);
}

/******************************************************************************
  Function: updateDIPResult
  Description: 更新识别区显示
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::updateDIPResult(QImage img)
{
	//显示在视频2区上

	if(video1PixItem)
	{
		scene2->removeItem(video1PixItem);
		delete video1PixItem;
		video1PixItem=0;
	}

	img=img.scaled(ui_main.video1graphicsView->width(),ui_main.video1graphicsView->height());
	video1PixItem=scene2->addPixmap(QPixmap::fromImage(img));
	scene1->setSceneRect(0,0,img.width(),img.height());

}

/******************************************************************************
  Function: updateDIPCalculationResult
  Description: 更新偏差值
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::updateDIPCalculationResult(SeamInfo seaminfo)
{
	receivedFrameNumber++;
	if(receivedFrameNumber!=0)
	{
		lastSeamOffset=currentSeamOffset;
	}
	currentSeamOffset=seaminfo.dWeldSeamOffset*horizontalCalibrationRatio;
	double sw=int(seaminfo.dWeldSeamWidth*horizontalCalibrationRatio*1000)/1000.0;
	QString seamWidth=QString::number(sw);

	QString weldTorchHeight=QString::number(seaminfo.dRemainingHeight);

	double co=int(seaminfo.dWeldSeamOffset*horizontalCalibrationRatio*1000)/1000.0;
	QString weldingcurrentOffset=QString::number(co);
	ui_main.seamWidthDisplayLabel->setText(seamWidth);
	ui_main.seamRemainingHeightDisplayLabel->setText(weldTorchHeight);
	ui_main.seamOffsetDisplayLabel->setText(weldingcurrentOffset);

}

/******************************************************************************
  Function: updateHorizontalCalibrationRatio
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::updateHorizontalCalibrationRatio(double value)
{
	horizontalCalibrationRatio=value;
	//发送horizontalCalibrationRatio给obj_onlinetrack
	emit updateHorizontalCalibrationRatio_triggered(horizontalCalibrationRatio);
}

/******************************************************************************
  Function: updateSeamType
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::updateSeamType()
{
	switch(weldSeamType)
	{
	case BUTT_WELD_SEAM:ui_main.seamTypeDisplayLabel->setText(QStringLiteral("对接焊缝"));
		break;
	case LAP_WELD_SEAM:ui_main.seamTypeDisplayLabel->setText(QStringLiteral("搭接焊缝"));
		break;
	case GROOVE_WELD_SEAM:ui_main.seamTypeDisplayLabel->setText(QStringLiteral("V型焊缝"));
		break;
	}
}

/******************************************************************************
  Function: on_loadTestingDataSetsPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_loadTestingDataSetsPushButton_clicked()
{
	emit triggerTestingMode_triggered();

	obj_imageaquisition->start();
	obj_imageaquisition->stream();			//obj_imageaquisition开始运作
}

/******************************************************************************
  Function: on_unloadTestingDataSetsPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_unloadTestingDataSetsPushButton_clicked()
{
	emit detriggerTestingMode_triggered();

	obj_imageaquisition->pause();

}

/******************************************************************************
  Function: on_quitPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_quitPushButton_clicked()
{
	QMessageBox msgBox;
	msgBox.setText(QStringLiteral("确定退出？"));
	msgBox.exec();
	CameraFree(cameraIndex);
	d1000_board_close();
	exit(0);
}

/******************************************************************************
  Function: on_turnOnRecognitionPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_turnOnRecognitionPushButton_clicked()
{
	if(g_isCameraWorking)
	{
		isTestModeTriggered=false;
		isDIPStarted=true;
	}
	else
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("相机未开，请先开启相机！"));
		msgbox.exec();
	}
}

/******************************************************************************
  Function: on_turnOffRecognitonPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_turnOffRecognitonPushButton_clicked()
{
	isDIPStarted=false;
	isFirstDIPStarted=false;
}

/******************************************************************************
  Function: updateMotionControllerConnectionState
  Description: 更新控制卡连接情况
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::updateMotionControllerConnectionState()
{
	if(0==cardCount) isControlCardConnected=false;
	else if (1==cardCount) isControlCardConnected=true;

	if(isControlCardConnected)
	{
		ui_main.cardStateDisplayLabel->setPixmap(QPixmap::fromImage(wrongico));
	}
	else
	{
		ui_main.cardStateDisplayLabel->setPixmap(QPixmap::fromImage(checkico));
	}
}

/******************************************************************************
  Function: on_actionSigmoid_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void  SeamTrackingSys::on_actionSigmoid_triggered()
{
	ui_main.actionTrapezoidal->setChecked(false);
	ui_main.actionSigmoid->setChecked(true);
}

/******************************************************************************
  Function: on_actionTrapezoidal_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionTrapezoidal_triggered()
{
	ui_main.actionTrapezoidal->setChecked(true);
	ui_main.actionSigmoid->setChecked(false);
}

/******************************************************************************
  Function: on_manualControlPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_manualControlPushButton_clicked()
{
	ui_manual.show();
}

/******************************************************************************
  Function: on_alignPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_alignPushButton_clicked()
{
	if(!isDIPStarted)
	{
		QMessageBox msgBox;
		msgBox.setText(QStringLiteral("识别未开！"));
		msgBox.exec();
		return;
	}
	else
	{
		if(!isCalibrationFinished)
		{
			QMessageBox msgBox;
			msgBox.setText(QStringLiteral("未校正，请先进行校正"));
			msgBox.exec();
			return;

		}
		double tolerance=0.2;
		qDebug()<<"currentSeamOffset:"<<currentSeamOffset;
		if(abs(currentSeamOffset)>tolerance)
		{
			obj_automotion.xMove(1,currentSeamOffset,0.5,1);
		}
		else
		{
			isAlignmentFinished=true;
		}
	}
}

/******************************************************************************
  Function: on_startAutoTrackPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_startAutoTrackPushButton_clicked()
{
	//check
	if(!g_isCameraWorking)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("相机未开！"));
		msgbox.exec();
		return;
	}
	if(!isDIPStarted)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("识别未开！"));
		msgbox.exec();
		return;
	}

	if(!isAlignmentFinished)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("未对齐！请先执行对齐命令"));
		msgbox.exec();
		return;
	}

	isAutoTrackTriggered=true;
	qDebug()<<"isAutoTrackTriggered="<<isAutoTrackTriggered;
}


/******************************************************************************
  Function: on_stopAutoTrackPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_stopAutoTrackPushButton_clicked()
{
	isAutoTrackTriggered=false;
	qDebug()<<"isAutoTrackTriggered="<<isAutoTrackTriggered;
	emit stopAutoTracking_triggered();

	d1000_decel_stop(0);
	d1000_decel_stop(1);
	d1000_decel_stop(2);
	d1000_decel_stop(3);
}

/******************************************************************************
  Function: setDefaultWeldParameters
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::setDefaultWeldParameters()
{
	weldparameter.dWeldSpeed=2.0;
	weldparameter.dGasFlow=0.5;
	weldparameter.dWeldVoltage=25.0;
	weldparameter.dWeldCurrent=100.0;
	weldparameter.dWireRate=2.0;
	weldparameter.dWeldDistance=100.0;
	weldparameter.dGasPressure=5;
	emit updateWeldParameters_triggered(weldparameter);
}

/******************************************************************************
  Function: updateWeldParameters
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::updateWeldParameters(WeldParameter parameters)
{
	weldparameter=parameters;
	isWeldParametersUpdated=true;
	emit updateWeldParameters_triggered(weldparameter);
	//更新显示焊接速度
}


/******************************************************************************
  Function: on_setWeldParametersPushButton_clicked
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_setWeldParametersPushButton_clicked()
{
	ui_weld.show();
}

/******************************************************************************
  Function: closeEvent
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::closeEvent(QCloseEvent *e)
{
	obj_imageaquisition->stop();
	obj_imageaquisition->wait();

	CameraFree(cameraIndex);
	d1000_board_close();
	exit(0);
	QMainWindow::closeEvent(e);

}

/******************************************************************************
  Function: on_actionOnlineTrack_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void  SeamTrackingSys::on_actionOnlineTrack_triggered()
{

}

/******************************************************************************
  Function: on_actionOfflineTrack_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionOfflineTrack_triggered()
{
	ui_main.actionOnlineTrack->setChecked(false);
	ui_offlinetrack.show();
}

/******************************************************************************
  Function: on_actionAbout_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionAbout_triggered()
{
	QMessageBox msgbox;
	msgbox.setWindowTitle("About");
	msgbox.setText(QStringLiteral("焊缝跟踪系统 Version 2.0.0 "));
	msgbox.exec();
}

/******************************************************************************
  Function: on_actionCalibrationMethod1_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionCalibrationMethod1_triggered()
{
	ui_simplecalibration.show();
	ui_main.actionCalibrationMethod1->setChecked(true);
	ui_main.actionCalibrationMethod2->setChecked(false);

}

/******************************************************************************
  Function: on_actionCalibrationMethod2_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionCalibrationMethod2_triggered()
{

}

/******************************************************************************
  Function: on_actionCalibrationMethod3_triggered
  Description: 
  Calls:
  Called By:  
  Input:
  Output: 
  Return:       
  Others: 
******************************************************************************/
void SeamTrackingSys::on_actionCalibrationMethod3_triggered()
{
	ui_fittingcalibration.show();
}

void SeamTrackingSys::updateCalibrationState()
{
	isCalibrationFinished=true;
}