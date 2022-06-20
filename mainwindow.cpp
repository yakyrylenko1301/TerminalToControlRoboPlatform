#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qabstractsocket.h"
#include <QThread>
#include <QKeyEvent>
#include "convertmatqimage.h"

namespace imageType {
    const int leftFrame = 1;
    const int rightFrame = 2;
}
typedef struct TAG_MSG_PACK_CTRL
{
    int type;
    int len;
    char data[];
}MSG_PACK_CTRL;

typedef struct TAG_MSG_PACK_IMG
{

}MSG_PACK_IMG;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->disableMoveBtn();
    socket = new QTcpSocket(this);
    dataSize = 0;
    imageType = -1;
    formPlotRPM = new SpeedSensorsPloting;
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    connect(this, &MainWindow::GetImage, this, &MainWindow::Disp);
    connect(this, &MainWindow::GetRPM, this, &MainWindow::DispRPM);
    ui->lineEditPort->setText("5001");
    ui->lineEditIpAddr->setText("192.168.1.107");
    this->server_addr = ui->lineEditIpAddr->text();
    this->server_port = ui->lineEditIpAddr->text().toInt();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    socket->connectToHost(this->server_addr, static_cast<quint16>(this->server_port));
    this->timerCheckTCP_Conection = new QTimer;
    connect(this->timerCheckTCP_Conection, SIGNAL(timeout()), this, SLOT(slotTimerCheckTCP_Conection()));
    this->timerCheckTCP_Conection->start(5001);
    ui->textBrowser->append("Status: Try to connect...");
    ui->textBrowser->append("IP Address:" + this->server_addr);
    ui->textBrowser->append("Port:" + QString::number(this->server_port));
}

void MainWindow::SendToServer(QString str, int type)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    int nRes = 0;
    int nLen = str.size();
    TAG_MSG_PACK_CTRL *msgPack;
    char *pSZMsgPack = (char*)malloc(sizeof(TAG_MSG_PACK_CTRL)+nLen);
    msgPack = (TAG_MSG_PACK_CTRL *)pSZMsgPack;
    msgPack->type = type;
    msgPack->len = nLen;
    memcpy(pSZMsgPack+sizeof(TAG_MSG_PACK_CTRL), str.data(), nLen);
    nRes = socket->write(pSZMsgPack, sizeof(TAG_MSG_PACK_CTRL) + nLen);
    printf("Write DataBuf: %d\n",nRes);
    free(pSZMsgPack);
    if(nRes == (nLen + sizeof(TAG_MSG_PACK_CTRL)))
    {
        ui->textBrowser->append("Status: Send - ok");
    }
    else
    {
        ui->textBrowser->append("Status: Send - ok");
    }
}

void MainWindow::slotReadyRead()
{
    QDataStream stream(socket);
    stream.setVersion(QDataStream::Qt_5_9);

    if( this->socket->bytesAvailable() < sizeof(quint32) )
        return;

    while(true)
    {
        if(dataSize > this->socket->bytesAvailable() )
            return;

        if( this->socket->bytesAvailable() < sizeof(quint32) )
            return;

        if(imageType==-1)
        {
            stream>>imageType;
            qDebug()<<"imageType: "<<imageType;
        }

        if( this->socket->bytesAvailable() < sizeof(quint32) )
            return;

        if(dataSize == 0)
        {
            stream >> dataSize; //size of received data
            qDebug()<<"dataSize:"<<dataSize;
        }

        if( dataSize > this->socket->bytesAvailable() || dataSize == 0)
            {
            return;
            }

        if(imageType == imageType::leftFrame)//frame,rgb
        {
            memset(image1BufLeft, 0, 76800*3);
            this->socket->read(image1BufLeft, dataSize);

            imageLeft = QImage((unsigned char*)image1BufLeft, 320, 240, 960, QImage::Format_RGB888);
        }
        else if (imageType == imageType::rightFrame)
        {
            memset(image1BufRight, 0, 76800*3);
            this->socket->read(image1BufRight, dataSize);

            imageRight = QImage((unsigned char*)image1BufRight, 320, 240, 960, QImage::Format_RGB888);
        }
        if (imageType == 5)
        {
            memset(rpmBuff, 0, 255);
            this->socket->read(rpmBuff, dataSize);
            ed_ss.rpmFL = rpmBuff[0];
            ed_ss.rpmFR = rpmBuff[1];
            ed_ss.rpmBL = rpmBuff[2];
            ed_ss.rpmBR = rpmBuff[3];

            emit GetRPM(ed_ss);
        }

        if( !imageLeft.isNull() )
        {
            qDebug()<<"--------------------GetImage-------------------";
            emit GetImage(imageLeft, imageType);
        }
        else if( !imageRight.isNull() )
        {
            qDebug()<<"--------------------GetImage-------------------";
            emit GetImage(imageRight, imageType);
        }
        else
        {
            qDebug()<<" Image Is Null: " << imageType;
        }
        imageType = -1;
        dataSize = 0;
    }
}

void MainWindow::slotTimerCheckTCP_Conection()
{
    bool connected = (socket->state() == QTcpSocket::ConnectedState);
    if (connected)
    {
        ui->textBrowser->append("Status: Connected");
        this->enableMoveBtn();
    }
    else
    {
        ui->textBrowser->append("Status:Error - can not Connect to:");
        ui->textBrowser->append("IP Address:" + this->server_addr);
        ui->textBrowser->append("Port:" + QString::number(this->server_port));
    }

    this->timerCheckTCP_Conection->stop();
}

void MainWindow::on_pushButton_2_clicked()
{
    SendToServer(ui->lineEdit->text(), 5);
}

void MainWindow::moveForward(void)
{
    SendToServer("F", cmdToControlMotor::moveFrorward);
}
void MainWindow::moveBack(void)
{
    SendToServer("B", cmdToControlMotor::moveBack);
}
void MainWindow::moveLeft(void)
{
    SendToServer("L", cmdToControlMotor::moveLeft);
}
void MainWindow::moveRight(void)
{
    SendToServer("R", cmdToControlMotor::moveRight);
}

void MainWindow::moveStop(void)
{
    SendToServer("S", cmdToControlMotor::moveStop);
}

void MainWindow::enableMoveBtn()
{
    ui->btnMoveBack->setEnabled(true);
    ui->btnMoveForward->setEnabled(true);
    ui->btnMoveLeft->setEnabled(true);
    ui->btnMoveRight->setEnabled(true);
}

void MainWindow::disableMoveBtn()
{
    ui->btnMoveBack->setEnabled(false);
    ui->btnMoveForward->setEnabled(false);
    ui->btnMoveLeft->setEnabled(false);
    ui->btnMoveRight->setEnabled(false);
}

void MainWindow::Disp(QImage image, int imageTpye)
{
    int tempWidth = ui->labVideoLeft->geometry().width();
    int tempHeight = ui->labVideoLeft->geometry().height();
    if (image.height() > 0)
    {
        QPixmap pix = QPixmap::fromImage(image.scaled(tempWidth, tempHeight));

        if(imageTpye == imageType::leftFrame)
        {
            QGraphicsScene *graphic = new QGraphicsScene(this);
            graphic->addPixmap(pix);
            ui->labVideoLeft->setScene(graphic);
        }
        if(imageTpye == imageType::rightFrame)
        {
            QGraphicsScene *graphic = new QGraphicsScene(this);
            graphic->addPixmap(pix);
            ui->labVideoRight->setScene(graphic);
        }
    }

}

void MainWindow::DispRPM(struct EDspeedSensors ed_ss)
{
    ui->lcdRpmFrontLeft->display(ed_ss.rpmFL);
    ui->lcdRpmFrontRight->display(ed_ss.rpmFR);
    ui->lcdRpmBackLeft->display(ed_ss.rpmBL);
    ui->lcdRpmBackRight->display(ed_ss.rpmBR);
}

void MainWindow::on_btnMoveForward_clicked()
{
    this->moveForward();
}

void MainWindow::on_btnMoveBack_clicked()
{
    this->moveBack();
}

void MainWindow::on_btnMoveLeft_clicked()
{
    this->moveLeft();
}

void MainWindow::on_btnMoveRight_clicked()
{
    this->moveRight();
}

void MainWindow::on_btnStop_clicked()
{
    this->moveStop();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
    {
        this->moveForward();
    }

    if(event->key() == Qt::Key_S)
    {
        this->moveBack();
    }

    if(event->key() == Qt::Key_A)
    {
        this->moveLeft();
    }

    if(event->key() == Qt::Key_D)
    {
        this->moveRight();
    }

    if(event->key() == Qt::Key_X)
    {
        this->moveStop();
    }

}

void MainWindow::on_btnSaveServerAddr_clicked()
{
    this->server_addr = ui->lineEditIpAddr->text();
    this->server_port = ui->lineEditPort->text().toInt();
}

void MainWindow::on_btnShowSpeedSensorsGraf_clicked()
{
    formPlotRPM->show();
}
