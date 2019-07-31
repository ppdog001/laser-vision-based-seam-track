/******************************************************************************
  File name: camera.cpp
  Author: WillLi99		Date:2019-5-18
  Description: 实现Camera类，Camera类的功能为实现调节相机成像。
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
  				15. initializeParamters				//初始化配置
  History: 
          <author>		<time>       <desc>
           WillLi99    2019-5-18     添加camra.h头部注释
******************************************************************************/

#include "JHCap.h"
#include "camera.h"

/******************************************************************************
  Function:Camera
  Description:
  Calls: 
  Called By: tracksys.cpp
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
Camera::Camera(QWidget *parent):
QWidget(parent)
{
	ui.setupUi(this);
	intCameraIndex=0;
}

/******************************************************************************
  Function:~Camera
  Description:
  Calls: 
  Called By:
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
Camera::~Camera()
{

}

/******************************************************************************
  Function:on_pushButtonWhiteBalance_clicked
  Description: 一键白平衡
  Calls: JHCap.h/CameraOnePushWB
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_pushButtonWhiteBalance_clicked()
{
	CameraOnePushWB(intCameraIndex);
}

/******************************************************************************
  Function:on_pushButtonLoadCameraConfiguration_clicked
  Description: 加载位于../camera目录下的xml相机配置文件
  Calls: QFileDialog等
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_pushButtonLoadCameraConfiguration_clicked()
{
	QString qstrFileName=QFileDialog::getOpenFileName(this,"Open XML","..\\camera",
		"XML Files (*.xml)");
	if(qstrFileName.isEmpty()==false)
	{
		QFile file(qstrFileName);
		if(!file.open(QFile::ReadOnly|QFile::Text))
		{
			std::cerr<<"Error::Cannot read file"<<qPrintable(qstrFileName)
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
					ui.horizontalSliderGamma->setValue(xmlReader.readElementText().toInt());
				}
				else if(xmlReader.name()=="Contrast")
				{
					ui.horizontalSliderContrast->setValue(xmlReader.readElementText().toInt());
				}
				else if(xmlReader.name()=="Gain")
				{
					ui.horizontalSliderGain->setValue(xmlReader.readElementText().toInt());
				}
				else if(xmlReader.name()=="Exposure")
				{
					ui.horizontalSliderExposure->setValue(xmlReader.readElementText().toInt());
				}
			}
		}

		if(xmlReader.hasError())
		{
			QMessageBox::critical(this,
				"xmlFile.xml Parse Error",xmlReader.errorString(),QMessageBox::Ok);
			return;
		}
	}
}

/******************************************************************************
  Function:on_pushButtonSaveCameraConfiguration_clicked
  Description: 保存当前的相机配置
  Calls: QFileDialog等
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_pushButtonSaveCameraConfiguration_clicked()
{
	QString qstrFileName=QFileDialog::getSaveFileName(this,"Save XML",".","XML Files (*.xml)");
	
	if(qstrFileName.isEmpty()==false)
	{
		QFile file(qstrFileName);
		file.open(QIODevice::WriteOnly);

		QXmlStreamWriter xmlWriter(&file);
		xmlWriter.setAutoFormatting(true);
		xmlWriter.writeStartDocument();

		xmlWriter.writeStartElement(QStringLiteral("相机参数"));
		xmlWriter.writeTextElement("Gamma",QString::number(ui.horizontalSliderGamma->value()));
		xmlWriter.writeTextElement("Contrast",QString::number(ui.horizontalSliderGamma->value()));

		int intGain;
		if(API_OK==CameraGetGain(intCameraIndex,&intGain));
		else intGain=0;
		xmlWriter.writeTextElement("Gain",ui.checkBoxGain->isChecked()?QString::number(intGain):QString
			::number(ui.horizontalSliderGain->value()));

		int intExposure;
		if(API_OK==CameraGetExposure(intCameraIndex,&intExposure));
		else intExposure=0;

		xmlWriter.writeTextElement("Exposure",ui.checkBoxExposure->isChecked()?QString::number(intExposure):QString
			::number(ui.horizontalSliderContrast->value()));

		xmlWriter.writeEndElement();
		xmlWriter.writeEndDocument();
		file.close();

		QMessageBox msgbox;
		msgbox.setWindowTitle(QStringLiteral("保存提示"));
		msgbox.setText(qstrFileName+QStringLiteral("已保存"));
		msgbox.exec();
	}
}


/******************************************************************************
  Function:on_pushButtonSaveCameraConfiguration_clicked
  Description: 保存单张图片的命令
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: tracksys.cpp处理
******************************************************************************/
void Camera::on_pushButtonSaveSingleImage_clicked()
{
	emit saveSingleImage_triggered();
}

/******************************************************************************
  Function:on_pushButtonSaveMultipleImages_clicked
  Description: 保存连续的图像序列
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: tracksys.cpp处理
******************************************************************************/
void Camera::on_pushButtonSaveMultipleImages_clicked()
{
	emit stopSavingMultipleImages_triggered();
}

/******************************************************************************
  Function:on_pushButtonStopSavingMultipleImages_clicked
  Description: 终止保存连续的图像序列
  Calls: 
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: tracksys.cpp处理
******************************************************************************/
void Camera::on_pushButtonStopSavingMultipleImages_clicked()
{
	emit stopSavingMultipleImages_triggered();
}

/******************************************************************************
  Function:initializeCameraConfiguration
  Description: 初始化相机配置
  Calls: JHCap.h/CameraGetGamma JHCap.h/CameraGetContrast JHCap.h/CameraGetAGC 
         JHCap.h/CameraGetAEC JHCap.h/CameraGetGain
  Called By: Camera()
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::initialize()
{
	//设置horizontalSlider的范围
	ui.horizontalSliderGamma->setMaximum(20);
	ui.horizontalSliderGamma->setMinimum(3);

	ui.horizontalSliderContrast->setMaximum(20);
	ui.horizontalSliderContrast->setMinimum(3);

	ui.horizontalSliderGain->setMaximum(32);
	ui.horizontalSliderGain->setMinimum(1);

	ui.horizontalSliderExposure->setMaximum(4000);
	ui.horizontalSliderExposure->setMinimum(1);

	double dGamma;
	CameraGetGamma(intCameraIndex,&dGamma);
	ui.labelGammaValue->setText(QString("%1").arg(dGamma));
	ui.horizontalSliderGamma->setValue(dGamma*10);

	double dContrast;
	CameraGetContrast(intCameraIndex,&dContrast);
	ui.labelContrastValue->setText(QString("%1").arg(dContrast));
	ui.horizontalSliderGamma->setValue(dContrast*100);

	int intGain;
	CameraGetGain(intCameraIndex,&intGain);
	ui.labelGainValue->setText(QString("%1").arg(intGain));
	ui.horizontalSliderGain->setValue(intGain);

	int intExposure;
	CameraGetExposure(intCameraIndex,&intExposure);
	ui.labelExposureValue->setText(QString("%1").arg(intExposure));
	ui.horizontalSliderExposure->setValue(intExposure);
}

/******************************************************************************
  Function:on_horizontalSliderGamma_valueChanged
  Description: 设置相机gamma值。此相机的gamma值范围是0.3-2.
  Calls: JHCap.h/CameraSetGamma
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_horizontalSliderGamma_valueChanged(int intGamma)
{
	double dGamma=intGamma/10.0;
	CameraSetGamma(intCameraIndex,dGamma);

	ui.labelGammaValue->setText(QString("%1").arg(intGamma));
}

/******************************************************************************
  Function:on_horizontalSliderContrast_valueChanged
  Description: 设置相机contrast值,此相机的contrast值范围是0.3-2.
  Calls: JHCap.h/CameraSetContrast
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_horizontalSliderContrast_valueChanged(int intContrastValue)
{
	double dContrast=intContrastValue/10.0;
	CameraSetContrast(intCameraIndex,dContrast);

	ui.labelContrastValue->setText(QString("%1").arg(intContrastValue));
}

/******************************************************************************
  Function:on_horizontalSliderGain_valueChanged
  Description: 设置相机gain值.增益范围为1-255，相机手册上建议是小于32
  Calls: JHCap.h/CameraSetGain
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_horizontalSliderGain_valueChanged(int intGainValue)
{
	CameraSetGain(intCameraIndex,intGainValue);
	ui.labelGainValue->setText(QString("%1").arg(intGainValue));
}

/******************************************************************************
  Function:on_exposureHS_valueChanged
  Description: 设置相机曝光值，增益范围为1-4000
  Calls: JHCap.h/CameraSetExposure
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_horizontalSliderExposure_valueChanged(int intExposureValue)
{
	CameraSetExposure(intCameraIndex,intExposureValue);
	ui.labelExposureValue->setText(QString("%1").arg(intExposureValue));
}

/******************************************************************************
  Function:on_checkBoxGain_toggled
  Description: 设置相机曝光值
  Calls: initializeCameraConfiguration
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_checkBoxGain_toggled(bool checked)
{
	CameraSetAGC(intCameraIndex,checked);

	CameraGetAGC(intCameraIndex,&bAGC);
	if(bAGC)//相机已开启自动增益
	{
		ui.horizontalSliderGain->setEnabled(false);
		ui.checkBoxGain->setChecked(true);
	}
	else //相机关闭自动增益
	{
		int gain;
		CameraGetGain(intCameraIndex,&gain);
		ui.labelGammaValue->setText(QString("%1").arg(gain));
		ui.horizontalSliderGain->setEnabled(true);
		ui.horizontalSliderGain->setValue(gain);
	}
	
}

/******************************************************************************
  Function:on_checkBoxExposure_toggled
  Description: 设置相机曝光值
  Calls: initializeCameraConfiguration
  Called By: 
  Input:          
  Output: 
  Return:       
  Others: 
******************************************************************************/
void Camera::on_checkBoxExposure_toggled(bool checked)
{
	CameraSetAEC(intCameraIndex,checked);
	
	CameraGetAEC(intCameraIndex,&bAEC);
	if(bAEC)//相机已开启自动曝光
	{
		ui.horizontalSliderExposure->setEnabled(false);
		ui.checkBoxExposure->setChecked(true);
	}
	else //相机关闭自动曝光
	{
		int exposure;
		CameraGetExposure(intCameraIndex,&exposure);
		ui.labelExposureValue->setText(QString("%1").arg(exposure));
		ui.horizontalSliderExposure->setEnabled(true);
		ui.horizontalSliderExposure->setValue(exposure);
	}
}