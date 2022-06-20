#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include <QTime>
#include <QImage>
#include "speedsensorsploting.h"

namespace cmdToControlMotor {
   const int moveFrorward   = 34;
   const int moveBack       = 35;
   const int moveLeft       = 36;
   const int moveRight      = 37;
   const int moveStop       = 38;
};

struct EDspeedSensors
{
    int rpmFL;
    int rpmFR;
    int rpmBL;
    int rpmBR;
};
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_btnMoveForward_clicked();

    void on_btnMoveBack_clicked();

    void on_btnMoveLeft_clicked();

    void on_btnMoveRight_clicked();

    void on_btnStop_clicked();

    void on_btnSaveServerAddr_clicked();

    void on_btnShowSpeedSensorsGraf_clicked();

private:
    QTimer *timerCheckTCP_Conection;
    QTimer *timerCheckTCP_PeriodConection;
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
    int dataSize;
    void SendToServer(QString str, int type);
    void moveForward(void);
    void moveBack(void);
    void moveLeft(void);
    void moveRight(void);
    void moveStop(void);
    void enableMoveBtn(void);
    void disableMoveBtn(void);
    int imageType;
    QImage imageLeft;
    QImage imageRight;
    char image2Buf[76800];
    char image1BufLeft[76800*3];
    char image1BufRight[76800*3];
    char rpmBuff[255];
    char ParaBuf[1024];
    void Disp(QImage image, int imageTpye);
    void DispRPM(struct EDspeedSensors ed_ss);
    void keyPressEvent(QKeyEvent *event);
    QString server_addr;
    int server_port;
    struct EDspeedSensors ed_ss;
    SpeedSensorsPloting* formPlotRPM;
public slots:
    void slotReadyRead();
    void slotTimerCheckTCP_Conection(void);
//    void slotTimerCheckTCP_PeriodConection(void);

signals:
    void GetImage(QImage image, int imageType);
    void GetRPM(struct EDspeedSensors ed_ss);

};
#endif // MAINWINDOW_H
