/*****************************************************************************
Copyright: Guangdong Provincial Welding Technology Research Center
Author: Li Yangjin
Date: 2018-8-27
Description:Camera模块封装了关于相机参数的设置，保存图片等功能。
*****************************************************************************/

#include "JHCap.h"
#include "camera.h"

Camera::Camera(QWidget *parent):
QWidget(parent)
{
	ui.setupUi(this);
	cameraIndex=0;
}

Camera::~Camera()
{

}

/***********************************************************************
@函数名：onWBClicked
@参数：无
@返回值：无
@功能：一键白平衡
@修改信息：
***********************************************************************/
void Camera::on_wbPushButton_clicked()
{
	CameraOnePushWB(cameraIndex);
}

/***********************************************************************
@函数名：on_loadCameraParasPushButton_clicked
@参数：无
@返回值：无
@功能：加载相机参数
@修改信息：
***********************************************************************/
void Camera::on_loadCameraParasPushButton_clicked()
{
	QString fileName=QFileDialog::getOpenFileName(this,"Open XML","..\\camera",
		"XML Files (*.xml)");
	if(fileName.isEmpty()==false)
	{
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
					ui.gammaHS->setValue(xmlReader.readElementText().toInt());
				}
				else if(xmlReader.name()=="Contrast")
				{
					ui.contrastHS->setValue(xmlReader.readElementText().toInt());
				}
				else if(xmlReader.name()=="Gain")
				{
					ui.hsGain->setValue(xmlReader.readElementText().toInt());
				}
				else if(xmlReader.name()=="Exposure")
				{
					ui.exposureHS->setValue(xmlReader.readElementText().toInt());
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

/***********************************************************************
@函数名：on_saveCameraParasPushButton_clicked
@参数：无
@返回值：无
@功能：保存相机参数
@修改信息：
***********************************************************************/
void Camera::on_saveCameraParasPushButton_clicked()
{
	QString fileName=QFileDialog::getSaveFileName(this,"Save XML",".","XML Files (*.xml)");
	
	if(fileName.isEmpty()==false)
	{
		QFile file(fileName);
		file.open(QIODevice::WriteOnly);

		QXmlStreamWriter xmlWriter(&file);
		xmlWriter.setAutoFormatting(true);
		xmlWriter.writeStartDocument();

		xmlWriter.writeStartElement(QStringLiteral("相机参数"));
		xmlWriter.writeTextElement("Gamma",QString::number(ui.gammaHS->value()));
		xmlWriter.writeTextElement("Contrast",QString::number(ui.gammaHS->value()));

		int gain;
		if(API_OK==CameraGetGain(cameraIndex,&gain));
		else gain=0;
		xmlWriter.writeTextElement("Gain",ui.gainCheckBox->isChecked()?QString::number(gain):QString
			::number(ui.hsGain->value()));

		int exposure;
		if(API_OK==CameraGetExposure(cameraIndex,&exposure));
		else exposure=0;

		xmlWriter.writeTextElement("Exposure",ui.exprosureCheckBox->isChecked()?QString::number(exposure):QString
			::number(ui.exposureHS->value()));

		xmlWriter.writeEndElement();
		xmlWriter.writeEndDocument();
		file.close();

		QMessageBox msgbox;
		msgbox.setWindowTitle(QStringLiteral("保存提示"));
		msgbox.setText(fileName+QStringLiteral("已保存"));
		msgbox.exec();
	}
}


/***********************************************************************
@函数名：on_saveSingleImagePushButton_clicked
@参数：无
@返回值：无
@功能：保存单张图片
@修改信息：
***********************************************************************/
void Camera::on_saveSingleImagePushButton_clicked()
{
	emit saveSingleImage_triggered();
}

/***********************************************************************
@函数名：on_saveMultiImagesPushButton_clicked
@参数：无
@返回值：无
@功能：保存多张图片
@修改信息：
***********************************************************************/
void Camera::on_saveMultiImagesPushButton_clicked()
{
	emit saveMultiImages_triggered();
}

/***********************************************************************
@函数名：on_stopSavingPicturesPushButton_clicked
@参数：无
@返回值：无
@功能：停止保存多张图片
@修改信息：
***********************************************************************/
void Camera::on_stopSavingImagesPushButton_clicked()
{
	emit stopSavingImages_triggered();
}

/***********************************************************************
@函数名：_initializeParamters
@参数：无
@返回值：无
@功能：设置各项默认参数
@修改信息：
***********************************************************************/
void Camera::initializeParamters()
{
	/***设置各种Sliders的范围***/
	ui.contrastHS->setMaximum(200);
	ui.contrastHS->setMinimum(30);
	ui.exposureHS->setMaximum(2000);
	ui.exposureHS->setMinimum(1);
	ui.hsGain->setMaximum(64);
	ui.hsGain->setMinimum(8);
	ui.gammaHS->setMaximum(200);
	ui.gammaHS->setMinimum(30);

	double gamma;
	CameraGetGamma(cameraIndex,&gamma);//获取相机gamma值
	ui.gammaLabel->setText(QString("%1").arg(gamma));
	ui.gammaHS->setValue(gamma*100);

	double contrast;
	CameraGetContrast(cameraIndex,&contrast);
	ui.contrastLabel->setText(QString("%1").arg(contrast));
	ui.contrastHS->setValue(contrast*100);

	bool aec,agc;
	CameraGetAGC(cameraIndex,&agc);//获取自动增益状态
	CameraGetAEC(cameraIndex,&aec);//获取自动曝光状态

	if(agc)
	{
		ui.hsGain->setEnabled(false);
		ui.gainCheckBox->setChecked(true);
	}
	else
	{
		int gain;
		CameraGetGain(cameraIndex,&gain);
		ui.gainLabel->setText(QString("%1").arg(gain));
		ui.hsGain->setValue(gain);
		ui.hsGain->setEnabled(true);
	}

	if(aec)
	{
		ui.exposureHS->setEnabled(false);
		ui.exprosureCheckBox->setChecked(true);
	}
	else
	{
		int exposure;
		CameraGetExposure(cameraIndex,&exposure);
		ui.exposureLabel->setText(QString("%1").arg(exposure));
		ui.exposureHS->setValue(exposure);
		ui.exposureHS->setEnabled(true);
		ui.exprosureCheckBox->setChecked(false);
	}
}

void Camera::on_gammaHS_valueChanged(int value)
{
	double gamma=value/100.0;
	CameraSetGamma(cameraIndex,gamma);

	ui.space1->setText(QString("%1").arg(value));
}

void Camera::on_contrastHS_valueChanged(int value)
{
	double contrast=value/100.0;
	CameraSetContrast(cameraIndex,contrast);

	ui.space2->setText(QString("%1").arg(value));
}

void Camera::on_hsGain_valueChanged(int value)
{
	CameraSetGain(cameraIndex,value);
	ui.space3->setText(QString("%1").arg(value));
}

void Camera::on_exposureHS_valueChanged(int value)
{
	CameraSetExposure(cameraIndex,value);
	ui.space4->setText(QString("%1").arg(value));
}

void Camera::on_gainCheckBox_toggled(bool checked)
{
	CameraSetAGC(cameraIndex,checked);
	initializeParamters();
}

void Camera::on_exprosureCheckBox_toggled(bool checked)
{
	CameraSetAEC(cameraIndex,checked);
	initializeParamters();
}