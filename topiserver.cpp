#include "topiserver.h"
#include "ui_topiserver.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

TopiServer::TopiServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TopiServer)
{
    ui->setupUi(this);

    this->setFixedSize(400, 300);
    tcpServer = NULL;
    clientConnection = NULL;
    mainWin = NULL;
    ui->stopButton->setEnabled(false);
    QMessageBox::information(this, "Info", "Please specify camara file");
    camaraFileName = QFileDialog::getOpenFileName(this, "Open Camara File", "D:/Qt/project/tracking_system/resources");
}

TopiServer::~TopiServer()
{
    delete ui;
}

void TopiServer::on_startButton_clicked()
{
    tcpServer = new QTcpServer(this);
    if(!tcpServer->listen(QHostAddress::LocalHost, 1207))
    {
        QMessageBox::warning(this, "Error", "Starting server error!");
        return;
    }
    ui->textBrowser->append("Server started!\n");
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(incomingConnect()));
}

void TopiServer::on_stopButton_clicked()
{

}

void TopiServer::incomingConnect()
{
    if(mainWin == NULL)
    {
        mainWin = new MainWindow(this);
        connect(mainWin, SIGNAL(hAngleChanged(int,int)), this, SLOT(onHAngleChanged(int,int)));
        connect(mainWin, SIGNAL(vAngleChanged(int,int)), this, SLOT(onVAngleChanged(int,int)));
        connect(mainWin, SIGNAL(zoomChanged(int,int)), this, SLOT(onZoomChanged(int,int)));
        mainWin->show();
    }
    clientConnection = tcpServer->nextPendingConnection();
    ui->textBrowser->append("New incoming connection\n");
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));

    // refresh data table
    topTable.clear();
    QMap<QString, ObjInfo> objMap;
    topTable.append(objMap);
    topTable.append(objMap);
    topTable.append(objMap);
    topTable.append(objMap);
    topTable.append(objMap);

    // read camara parameters
    QFile sensorFile(camaraFileName);
    if(sensorFile.open(QIODevice::ReadOnly))
    {
        QTextStream sensorIn(&sensorFile);
        while(!sensorIn.atEnd())
        {
            QString line = sensorIn.readLine();
            QStringList list = line.split(",");
            CamInfo tempSensor;
            tempSensor.x = list[0].toInt();
            tempSensor.y = list[1].toInt();
            tempSensor.z = list[2].toInt();
            tempSensor.abs_alpha = list[3].toInt();
            tempSensor.abs_beta = list[4].toInt();
            tempSensor.abs_zoom = 0;
            tempSensor.alpha = 0;
            tempSensor.beta = 0;
            tempSensor.zoom = 0;
            tempSensor.modified = false;
            camList.append(tempSensor);
        }
        ui->textBrowser->append(QString("%1 Camara positions have been set\n").arg(camList.size()));
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("unable to open sensor info file"));
        return;
    }
    sensorFile.close();
}

void TopiServer::readData()
{
    QString newData(clientConnection->readAll());
    ui->textBrowser->append(newData + QString(" recved!\n"));
    QStringList tempList = newData.split(" ");
    for(int i = 0; i < tempList.size(); i++)
    {
        QStringList subList = tempList[i].split(",");
        storeData(subList);
    }

    // go through the whole table to erase objs out of range
    for(int i = 0; i < topTable.size() - 1; i++)
    {
        QMap<QString, ObjInfo>::iterator it, itBegin, itEnd;
        itBegin = topTable[i].begin();
        itEnd = topTable[i].end();
        for(it = topTable[i].begin(); it != topTable[i].end();)
        {
            if(it.value().modified == false)
            {
                it = topTable[i].erase(it);
                continue;
            }
            else
            {
                it.value().modified = false;
            }
            it++;
        }
    }

    // refresh output image
    mainWin->refreshLocalView(topTable);

    change_camera_angle();

    QString outData;
    for (int i = 0; i < 4; i++)
    {
        outData += QString("%1,%2,%3,%4 ").arg(i).arg(camList[i].alpha)
                .arg(camList[i].beta).arg(camList[i].zoom);
        camList[i].abs_alpha += camList[i].alpha;
        camList[i].abs_beta += camList[i].beta;
        camList[i].abs_zoom += camList[i].zoom;

        camList[i].alpha = 0;
        camList[i].beta = 0;
        camList[i].zoom = 0;
        camList[i].modified = false;
    }
    outData.chop(1);
    if(outData != "")
    {
        clientConnection->write(outData.toStdString().c_str(), strlen(outData.toStdString().c_str()));
        clientConnection->waitForBytesWritten();
    }
}

void TopiServer::storeData(QStringList list)
{
    int camNum = list[0].toInt();
    QPoint point(list[2].toInt(), list[3].toInt());
    QMap<QString, ObjInfo>::iterator it = topTable[camNum].find(list[1]);
    if(it != topTable[camNum].end())
    {
        it.value().modified = true;
        it.value().position.append(point);
        if(it.value().position.size() > 2)
        {
           it.value().position.removeFirst();
        }
        it.value().vector = it.value().position[1] - it.value().position[0];
        it.value().z = list[4].toInt();
    }
    else
    {
        ObjInfo newObj;
        newObj.key = list[1];
        newObj.modified = true;
        newObj.position.append(point);
        newObj.vector.setX(0);
        newObj.vector.setY(0);
        newObj.z = list[4].toInt();
        topTable[camNum].insert(list[1], newObj);
    }
}

void TopiServer::onHAngleChanged(int camNum, int angle)
{
    /*if(mutex == true)
    {
        return;
    }*/
    camList[camNum].alpha += angle;
    camList[camNum].modified = true;
}

void TopiServer::onVAngleChanged(int camNum, int angle)
{
    /*if(mutex == true)
    {
        return;
    }*/
    camList[camNum].beta += angle;
    camList[camNum].modified = true;
}

void TopiServer::onZoomChanged(int camNum, int zoomNum)
{
    /*if(mutex == true)
    {
        return;
    }*/
    camList[camNum].zoom += zoomNum;
    camList[camNum].modified = true;
}

void TopiServer::change_camera_angle()
{
    /*QMap<QString, ObjInfo>::iterator it;
    QPoint focusPoint[4];
    double ver[4];
    QPoint dir[4];
    double speed[4];
    for (int i = 0; i < 4; i++)
    {
        dir[i].setX(0);
        dir[i].setY(0);
        focusPoint[i].setX(0);
        focusPoint[i].setY(0);
        speed[i] = 0;
        ver[i] = 0;
    }

    for (it = topTable[4].begin(); it !=topTable[4].end(); it++)
    {
        double temp = sqrt(qPow(it.value().vector.x(), 2) + qPow(it.value().vector.y(), 2));
        if (temp > speed[0])
        {
            for (int j = 3; j > 0; j--)
            {
                speed[j] = speed[j - 1];
                focusPoint[j] = focusPoint[j - 1];
                ver[j] = ver[j - 1];
            }
            speed[0] = temp;
            focusPoint[0] = it.value().position[1];
            ver[0] = it.value().z;
            dir[0] = it.value().vector;
        }
        else if (temp <= speed[0] && temp > speed[1])
        {
            for (int j = 3; j > 1; j--)
            {
                speed[j] = speed[j - 1];
                focusPoint[j] = focusPoint[j - 1];
                ver[j] = ver[j - 1];
            }
            speed[1] = temp;
            focusPoint[1] = it.value().position[1];
            ver[1] = it.value().z;
            dir[1] = it.value().vector;
        }
        else if (temp <= speed[1] && temp > speed[2])
        {
            for (int j = 3; j > 2; j--)
            {
                speed[j] = speed[j - 1];
                focusPoint[j] = focusPoint[j - 1];
                ver[j] = ver[j - 1];
            }
            speed[2] = temp;
            focusPoint[2] = it.value().position[1];
            ver[2] = it.value().z;
            dir[2] = it.value().vector;
        }
        else if (temp <= speed[2] && temp > speed[3])
        {
            speed[3] = temp;
            focusPoint[3] = it.value().position[1];
            dir[3] = it.value().vector;
            ver[3] = it.value().z;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        double x, y, z, a, b, c;
        double alpha_s, beta_s;
        x = camList[i].x;
        y = camList[i].y;
        z = camList[i].z;
        a = focusPoint[i].x();
        b = focusPoint[i].y();
        c = ver[i];
        if ((b - y) < 0){
            alpha_s = atan2(b - y, a - x) * 180 / PI + 360;
        }
        else {
            alpha_s = atan2(b - y, a - x) * 180 / PI;
        }

        beta_s = atan2(c - z,
                       sqrt((a - x) * (a - x) + (b - y)* (b - y))) * 180 / PI;
        camList[i].alpha = alpha_s - camList[i].abs_alpha;
        camList[i].beta = beta_s - camList[i].abs_beta;
        camList[i].modified = true;
    }*/

    QMap<QString, ObjInfo>::iterator it, curIt;
    int frame, curFrame;
    for(int i = 0; i < 4; i++)
    {
        frame = 65535;
        for(curIt = topTable[i].begin(); curIt != topTable[i].end(); curIt++)
        {
            int curFrameX, curFrameY;
            curFrameX = curFrameY = 65535;
            if(curIt.value().vector.x() != 0)
            {
                int a = (100 - curIt.value().position[1].x()) / curIt.value().vector.x();
                int b = (curIt.value().position[1].x() - 0) / curIt.value().vector.x();
                //int a = (100 - curIt.value().vector.x()) / curIt.value().vector.x();
                //int b = (curIt.value().vector.x() - 0) / curIt.value().vector.x();
                curFrameX = a > b ? a : b;
            }
            if(curIt.value().vector.y() != 0)
            {
                int a = (100 - curIt.value().position[1].y()) / curIt.value().vector.y();
                int b = (curIt.value().position[1].y() - 0) / curIt.value().vector.y();
                //int a = (100 - curIt.value().vector.y()) / curIt.value().vector.y();
                //int b = (curIt.value().vector.y() - 0) / curIt.value().vector.y();
                curFrameY = a > b ? a : b;
            }
            curFrame = curFrameX < curFrameY ? curFrameX : curFrameY;
            //if(curFrame <= 2 && curFrame < frame)
            if(curFrame < frame)
            {
                frame = curFrame;
                it = curIt;
            }
        }
        if(frame < 65535)
        {
            double x, y, z, a, b, c;
            double alpha_s, beta_s;
            x = camList[i].x;
            y = camList[i].y;
            z = camList[i].z;
            a = topTable[4].find(it.value().key).value().position[1].x();
            b = topTable[4].find(it.value().key).value().position[1].y();
            c = topTable[4].find(it.value().key).value().z;
            if ((a - x) < 0){
                alpha_s = 180 - atan((b - y) /(a - x)) * 180 / PI;
            }
            else {
                alpha_s = - atan((b - y) /(a - x)) * 180 / PI;
            }
            if (alpha_s < 0)
            {
                alpha_s += 360;
            }

            beta_s = atan((c - z) /
                           sqrt((a - x) * (a - x) + (b - y)* (b - y))) * 180 / PI;
            camList[i].alpha = alpha_s - camList[i].abs_alpha;
            camList[i].beta = beta_s - camList[i].abs_beta;
            camList[i].modified = true;
            //topTable[i].clear();
        }
    }

}
