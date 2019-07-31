#include "tracksys.h"
#include "common.h"
#include "dip.h"

void dipTesting1Frame();
void dipTestingnFrames();
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SeamTrackingSys w;
	w.show();
	return a.exec();

	//dipTestingnFrames();
	//dipTesting1Frame();
	return 0;
}

void dipTestingnFrames(){
	string filePath="..//Images//testing images//";
	//string savePath="..//Images//testing results//";
	//clock_t start,end;
	//start=clock();
	for(int i=1;i<=250;i++)
	{
		string newfilePath=filePath+to_string((long long)i)+".bmp";
		Mat img=imread(newfilePath);

		if(i==1)
		{
			DIP::setROISize(400,200);
			DIP::locateROI(img);
		}
		DIP dip(img);
		imshow("out",dip.mOutputImage);waitKey();
		//imwrite(savePath+to_string((long long)i)+".tif",dip.thinnedImage);
	}
	//end=clock();
	//qDebug()<<"Run time="<<(end-start)/CLOCKS_PER_SEC<<endl;
}

void dipTesting1Frame()
{
	string filePath="..//images//test//0.bmp";
	Mat img=imread(filePath);
	
	DIP::setROISize(400,200);
	DIP::locateROI(img);
	DIP dip(img);

	//imshow("result",dip.mOutputImage);
	waitKey();
}