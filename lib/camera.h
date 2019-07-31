/******************************************************************************
  File name: camera.h
  Author: WillLi99		Date:2019-5-18
  Description: 定义Camera类，实现调节相机成像。
  Others: 
  Function List:
                1. Camera
                2. ~Camera
				3. on_pushButtonWhiteBalance_clicked				//一键白平衡
				4. on_pushButtonLoadCameraConfiguration_clicked		//载入相机配置
				5. on_pushButtonSaveCameraConfiguration_clicked		//保存相机配置
				6. on_pushButtonSaveSingleImage_clicked				//保存单张图片
				7. on_pushButtonSaveMultipleImages_clicked			//保存多张图像
				8. on_pushButtonStopSavingMultipleImages_clicked	//停止保存图像
				9. on_horizontalSliderGamma_valueChanged			//改变gamma值
				10. on_horizontalSliderContrast_valueChanged		//改变对比度
				11. on_horizontalSliderGain_valueChanged			//改变相机增益
				12. on_horizontalSliderExposure_valueChanged		//改变曝光值
				13  on_checkBoxGain_toggled			//自动增益
				14. on_checkBoxExposure_toggled		//自动曝光
				15. initialize						//初始化配置
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-18     添加camra.h头部注释
******************************************************************************/

#ifndef Camera_H
#define Camera_H
#include "ui_camera.h"
#include "common.h"

//相机控制类
class Camera:public QWidget
{
	Q_OBJECT
public:
	explicit Camera(QWidget *parent=0);
	~Camera();

private slots:
	void on_pushButtonWhiteBalance_clicked();
	void on_pushButtonLoadCameraConfiguration_clicked();
	void on_pushButtonSaveCameraConfiguration_clicked();
	void on_pushButtonSaveSingleImage_clicked();
	void on_pushButtonSaveMultipleImages_clicked();
	void on_pushButtonStopSavingMultipleImages_clicked();

	void on_horizontalSliderGamma_valueChanged(int value);
	void on_horizontalSliderContrast_valueChanged(int value);
	void on_horizontalSliderGain_valueChanged(int value);
	void on_horizontalSliderExposure_valueChanged(int value);
	
	void on_checkBoxGain_toggled(bool checked);
	void on_checkBoxExposure_toggled(bool checked);

private:
	Ui_Camera ui;
	int intCameraIndex;
	bool bAGC;
	bool bAEC;
	void initialize();

signals:
	void saveSingleImage_triggered();
	void saveMultipleImages_triggered();
	void stopSavingMultipleImages_triggered();
};

#endif