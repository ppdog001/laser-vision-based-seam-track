#include "offlinetrack.h"

OfflineTrack::OfflineTrack(QWidget *parent)
{
	ui.setupUi(this);
	hasSwitchedOnWeld=false;
	hasCollected=false;
	hasAnalyzed=false;
	haveDataOrNot=false;
	hasTrackStarted=false;
	notAllowTrack=false;

	dataCount=0;
	trackCount=0;
	dataNum=250;
	filePath="..\\images\\offlinetrack\\";
	fileName="";
	k=0;
	t=0;
	ro=0.0;
	lnro=0.0;
	connect(&sTimer200ms, SIGNAL(timeout()), this, SLOT(timeOut200ms()));

}

OfflineTrack::~OfflineTrack()
{

}


void OfflineTrack::updateWeldParameters(WeldParameter parameters)
{
	weldSpeed=parameters.weldSpeed;
	trackingDistance=parameters.trackingDistance;
}

void OfflineTrack::on_collectDataPushButton_clicked()
{

	t=trackingDistance/weldSpeed;
	k=t/0.2;
	dataNum=k;

	if(!cameraIsRunning)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("请确保已打开相机！"));
		msgbox.exec();
		return;
	}

	start200msTiming();
}

void OfflineTrack::timeOut200ms()
{
	stop200msTiming();
	if(!notAllowTrack)
	{
		if(!hasTrackStarted)
		{
			fileName=filePath+QString::number(dataCount)+".bmp";
			emit saveImage_triggered(fileName);
			dataCount++;
			if(dataCount<dataNum)
			{
				start200msTiming();
			}
			else
			{
				hasCollected=true;
				QMessageBox msgbox;
				msgbox.setText(QStringLiteral("数据采集完毕！"));
				msgbox.exec();
				return;
			}
		}
		else if(hasTrackStarted)
		{
			if(trackCount<dataNum)
			{
				ro=relOffsetList[trackCount];
				ro=ro+lnro;
				if(abs(ro)<0.2)
				{
					lnro=ro;
				}
				else
				{
					offlineMotionObject.xMove(abs(6*ro),ro,0.01,1);
					lnro=0.0;
				}
				
				trackCount++;
				start200msTiming();
			}
		else
			{
				switchOffWeld();
				QMessageBox msgbox;
				msgbox.setText(QStringLiteral("跟踪完成！"));
				msgbox.exec();
				return;
			}
		}
	}
	else if(notAllowTrack)	//停止时清除数据
	{
		dataCount=0;
		trackCount=0;
		absOffsetList.clear();
		relOffsetList.clear();
		
		offlineMotionObject.allStop();


		switchOffWeld();
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("已停机，并清除数据！"));
		msgbox.exec();
		return;
	}
}

void OfflineTrack::start200msTiming()
{
	//qDebug()<<"200ms timing starts...";
	sTimer200ms.start(200);
	offlineMotionObject.yMove(weldSpeed,weldSpeed*0.2,0.01,0);
}



void OfflineTrack::stop200msTiming()
{
	sTimer200ms.stop();
	//qDebug()<<"200ms timing terminates...";
}


void OfflineTrack::on_analyzePushButton_clicked()
{
	if(ui.fromCollectionRadioButton->isChecked())	//从数据收集处采集
	{
		if(!hasCollected)
		{
			QMessageBox msgbox;
			msgbox.setText(QStringLiteral("请确保数据采集完成！"));
			msgbox.exec();
			return;
		}
	}
	else if(ui.fromFolderRadioButton->isChecked())
	{
		dataNum=250;
	}
	QString dirName="..\\images\\offline\\";
	QString fileName;
	string stdFileName;
	
	for(int i=1;i<=dataNum;i++)
	{
		fileName=dirName+QString::number(i)+".bmp";
		stdFileName=fileName.toStdString();
		cv::Mat image=imread(stdFileName);

		if(i==0)
		{
			DIP::getROIPosition(image,&DIP::roiX,&DIP::roiY);

		}
		DIP dip(image);
		double offset=dip.seaminfo.weldSeamOffset;
		absOffsetList.push_back(dip.seaminfo.weldSeamOffset);
	}

	qDebug()<<"the last abs offset is"<<absOffsetList[dataNum-1];
	qDebug()<<"the first abs offset is"<<absOffsetList[0];
	qDebug()<<"the difference of the last and the first is:"<<rho*(absOffsetList[dataNum-1]-absOffsetList[0]);

	double lastAbsOffset;
	double sum=0.0;
	for(int i=0;i<absOffsetList.length();i++)
	{
		if(i==0)
		{
			lastAbsOffset=absOffsetList[i];
			relOffsetList.push_back(0.0);
			continue;
		}
		double reloffset=rho*(absOffsetList[i]-lastAbsOffset);
		relOffsetList.push_back(reloffset);
		lastAbsOffset=absOffsetList[i];
		sum=sum+reloffset;
	}

	qDebug()<<"the sum of all the rel offset is:"<<sum;

	
	hasAnalyzed=true;
	if(hasAnalyzed)
	{
		haveDataOrNot=true;
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("已完成数据分析！"));
		msgbox.exec();
		return;
	}
}

void OfflineTrack::on_returnOriginPushButton_clicked()
{
	if(!haveDataOrNot)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("请先确保数据已经采集和分析完毕！"));
		msgbox.exec();
		return;
	}
	offlineMotionObject.yMove(3*weldSpeed,-trackingDistance,0.2,1);
}

void OfflineTrack::on_startPushButton_clicked()
{
	if(!haveDataOrNot)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("请先确保数据存在！"));
		msgbox.exec();
		return;
	}
	hasTrackStarted=true;

	if(ui.onRadioButton->isChecked())
	{
		switchOnWeld();
	}

	start200msTiming();
}


void OfflineTrack::updateHorizontalRatio(double rho)
{
	this->rho=rho;
}


void OfflineTrack::on_clearDataPushButton_clicked()
{
	if(ui.memoryOnlyRadioButton->isChecked())	//仅清理内存
	{
		dataCount=0;
		trackCount=0;
		absOffsetList.clear();
		relOffsetList.clear();

		notAllowTrack=false;
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("清除数据成功！"));
		msgbox.exec();
		return;
	}
	else if(ui.allRadioButton->isChecked())		//清理内存和文件夹资料
	{
		dataCount=0;
		trackCount=0;
		absOffsetList.clear();
		relOffsetList.clear();

		QString dir="..\\Images\\offline\\";
		QDir del(dir);

		del.setFilter(QDir::Files);
		int i,j=del.count()-1;
		for (i=0;i<=j;i++)
			del.remove(del[i]);


		notAllowTrack=false;
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("清除数据成功！"));
		msgbox.exec();
		return;
	}

	
}


void OfflineTrack::on_stopPushButton_clicked()
{
	notAllowTrack=true;
	offlineMotionObject.allStop();
	
	switchOffWeld();
	ui.onRadioButton->setChecked(true);
}


void OfflineTrack::on_saveDataPushButton_clicked()
{
	dataCount=dataNum;weldSpeed=2.0;trackingDistance=100.0;

	if(relOffsetList.length()==0)
	{
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("当前无数据！"));
		msgbox.exec();
		return;
	}
	else
	{
		QString fileName=QFileDialog::getSaveFileName(this,"Save TXT",".","txt files (*.txt)");
		if(fileName.isEmpty()==false)
		{
			QFile file(fileName);
			bool isok=file.open(QIODevice::WriteOnly);
			if(isok==true)	//写入内容
			{
				QTextStream txtStream(&file);
				
				QString begin_str="$$$";
				txtStream<<begin_str<<endl;
				
				QString vitalInfo;
				vitalInfo=QString::number(dataCount)+" "+QString::number(weldSpeed)+" "+
					QString::number(trackingDistance);
				txtStream<<vitalInfo<<endl;

				QString offsetString;
				for(int i=0;i<dataCount;i++)
				{
					offsetString=QString::number(relOffsetList[i]);
					txtStream<<offsetString<<endl;
				}
			}
			file.close();
			
		}
		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("保存数据成功！"));
		msgbox.exec();
		return;

	}
}

void OfflineTrack::on_loadDataPushButton_clicked()
{
	QString fileName=QFileDialog::getOpenFileName(this,"open txt","..\\data",
		"txt file(*.txt)");
	if(fileName.isEmpty()==false)
	{
		QFile file(fileName);
		if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
		{	
			std::cerr<<"Error::Cannot read file"<<qPrintable(fileName)
				<<":"<<qPrintable(file.errorString())<<std::endl;
			return;
		}
		
		QTextStream textStream(&file);
		
		QString firstLine=textStream.readLine();
		if(firstLine!="$$$")
		{
			qDebug()<<"Wrong txt file";
			file.close();
			return;
		}
		QString secondLine=textStream.readLine();
		QStringList strList=secondLine.split(" ");

		dataNum=strList[0].toInt();
		weldSpeed=strList[1].toDouble();
		trackingDistance=strList[2].toDouble();
		
		int i=0;
		QString lineStr;
		double number;
		while(!textStream.atEnd() && i<dataNum)
		{
			lineStr=textStream.readLine();
			number=lineStr.toDouble();
			relOffsetList.push_back(number);
			//qDebug()<<number;
			i++;
		}
		file.close();

		qDebug()<<"the length of relOffsetList="<<relOffsetList.length();

		haveDataOrNot=true;

		QMessageBox msgbox;
		msgbox.setText(QStringLiteral("载入数据成功！"));
		msgbox.exec();
		return;
	}
}

void OfflineTrack::switchOnWeld()
{
	d1000_out_bit(4,0);
}
void OfflineTrack::switchOffWeld()
{
	d1000_out_bit(4,1);
}