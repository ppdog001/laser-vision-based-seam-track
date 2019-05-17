#ifndef Camera_H
#define Camera_H
#include "ui_camera.h"
#include "var.h"

//相机控制类
class Camera:public QWidget
{
	Q_OBJECT
public:
	explicit Camera(QWidget *parent=0);
	~Camera();

private slots:
	void on_wbPushButton_clicked();
	void on_loadCameraParasPushButton_clicked();
	void on_saveCameraParasPushButton_clicked();
	void on_saveSingleImagePushButton_clicked();
	void on_saveMultiImagesPushButton_clicked();
	void on_stopSavingImagesPushButton_clicked();

	void on_gammaHS_valueChanged(int value);
	void on_contrastHS_valueChanged(int value);
	void on_hsGain_valueChanged(int value);
	void on_exposureHS_valueChanged(int value);
	
	void on_gainCheckBox_toggled(bool checked);
	void on_exprosureCheckBox_toggled(bool checked);

private:
	Ui_Camera ui;	//Camera模块的ui
	int cameraIndex;
	void initializeParamters();

signals:

	void saveSingleImage_triggered();
	void saveMultiImages_triggered();
	void stopSavingImages_triggered();
};

#endif