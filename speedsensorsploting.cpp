#include "speedsensorsploting.h"
#include "ui_speedsensorsploting.h"
#include "QVector"
#include <time.h>
#include <QGlobal.h>
#include <QTime>

SpeedSensorsPloting::SpeedSensorsPloting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpeedSensorsPloting)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    ui->setupUi(this);
    ui->PlotRPM->addGraph();
    ui->PlotRPM->xAxis->setLabel("Time, c");
    ui->PlotRPM->yAxis->setLabel("Speed, rpm");
    ui->PlotRPM->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->PlotRPM->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->PlotRPM->yAxis->setRange(-12000, 12000);
    ui->PlotRPM->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->PlotRPM->xAxis->setTicker(timeTicker);
    ui->PlotRPM->axisRect()->setupFullAxesBox();
    this->key = 0;
}

SpeedSensorsPloting::~SpeedSensorsPloting()
{
    delete ui;
}

void SpeedSensorsPloting::on_btnPlot_clicked()
{
    double rpm_value = qrand() % ((1200 + 1) - (-1200)) + (-1200);
    ui->PlotRPM->graph(0)->addData(key, rpm_value);
    this->key++;
    ui->PlotRPM->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->PlotRPM->rescaleAxes();
    ui->PlotRPM->replot();

//    QVector<double> x = {1, 2 ,3 ,4 ,5 ,6}, y = {5,7,8,9,10,5};
//    ui->PlotRPM->graph(0)->setData(x,y);
//    ui->PlotRPM->rescaleAxes();
//    ui->PlotRPM->replot();
//    ui->PlotRPM->update();
}

void SpeedSensorsPloting::on_btnClear_clicked()
{
    ui->PlotRPM->graph(0)->data()->clear();
    ui->PlotRPM->replot();
    ui->PlotRPM->update();
    this->key = 0;
}
