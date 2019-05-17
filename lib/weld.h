#pragma once

#include "ui_weld.h"
#include "var.h"

class Weld :
	public QWidget
{	
	Q_OBJECT
public:
	explicit Weld(QWidget *parent=0);
	~Weld(void);
private:
	Ui_Weld  ui;
	double weldSpeed;
	double wireFeedRate;
	double sheildingGasFlow;
	double weldVoltage;
	double weldCurrent;
	double trackingDistance;	//1是正向，-1是负向。
	WeldParameter parameters;

	void setDefaultParameters();
	int checkParameters();

private slots:
	void on_confirmPushButton_clicked();
	void on_cancelPushButton_clicked();
signals:
	void updateWeldParameters_triggled(WeldParameter paramters);
};
