#ifndef _OFFLINETRACK_H
#define _OFFLINETRACK_H

#include "ui_offlinetrack.h"
#include "var.h"
#include "motion.h"
#include "dip.h"
#include "dmc1000.h"

extern bool hasAligned;
extern bool hasCalibrated;
extern bool cameraIsRunning;

class OfflineTrack :public QWidget
{	
	Q_OBJECT
public:
	explicit OfflineTrack(QWidget *parent=0);
	~OfflineTrack(void);
private:
	Ui_offlineTrack  ui;

	Motion offlineMotionObject;

	double trackingDistance;
	double weldSpeed;
	bool hasSwitchedOnWeld;
	int k;
	int t;
	int trackCount;
	int dataNum;
	QString filePath;
	QString fileName;

	double rho;
	double ro;
	double lnro;

	int dataCount;
	QTimer sTimer200ms;

	bool hasCollected;
	bool hasAnalyzed;
	bool haveDataOrNot;
	bool hasTrackStarted;
	bool notAllowTrack;

	QVector<double>absOffsetList;
	QVector<double>relOffsetList;
	
	void start200msTiming();
	void stop200msTiming();
	void start100msTimming();
	void stop100msTiming();
	void start150msTiming();
	void stop150msTiming();
	void switchOnWeld();
	void switchOffWeld();

private slots:
	void updateWeldParameters(WeldParameter parameters);
	void updateHorizontalRatio(double rho);
	void on_collectDataPushButton_clicked();
	void on_analyzePushButton_clicked();
	void on_returnOriginPushButton_clicked();
	void on_startPushButton_clicked();
	void on_clearDataPushButton_clicked();
	void on_stopPushButton_clicked();
	void on_saveDataPushButton_clicked();
	void on_loadDataPushButton_clicked();
	void timeOut200ms();

signals:
	void saveImage_triggered(QString fileName);
};

#endif