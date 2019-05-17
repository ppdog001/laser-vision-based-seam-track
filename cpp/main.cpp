#include "tracksys.h"
#include "var.h"
#include "dip.h"




void dipTesting();
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SeamTrackingSys w;
	w.show();
	return a.exec();

	//dipTesting();
	return 0;
}

void dipTesting(){
	string filePath="..//Images//testing images//";
	string savePath="..//Images//testing results//";
	clock_t start,end;
	start=clock();
	for(int i=1;i<=39;i++)
	{
		
		qDebug()<<"processing "<<i<<endl;

		string newfilePath=filePath+to_string((long long)i)+".bmp";
		Mat img=imread(newfilePath);

		if(i==1)
		{
			DIP::getROIPosition(img,&DIP::roiX,&DIP::roiY);

		}
		DIP dip(img);

		//将图片显示在右侧的方框

		//imwrite(savePath+to_string((long long)i)+".jpg",dip.markImage);
		//dip.examine(filePath,i);//一些储存功能
	}
	end=clock();
	qDebug()<<"Run time="<<(end-start)/CLOCKS_PER_SEC<<endl;


	////单张图片测试
	//clock_t start,end;
	//start=clock();
	//Mat img=imread("..//Images//testing images//1.bmp");
	//DIP dipObj(img);
	//end=clock();
	//qDebug()<<"Run time="<<(end-start)/CLOCKS_PER_SEC<<endl;
}