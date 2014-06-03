#ifndef TOPISERVER_H
#define TOPISERVER_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QList>
#include <QMap>
#include <QPoint>
#include <qmath.h>
#include <stdio.h>
#include <iostream>
#include "mainwindow.h"

#define PI 3.14159265358979323846

namespace Ui {
class TopiServer;
}

struct CamInfo;
struct ObjInfo;

class TopiServer : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit TopiServer(QWidget *parent = 0);
    ~TopiServer();
    
private slots:
    void on_startButton_clicked();

    void on_stopButton_clicked();

    void incomingConnect();

    void readData();

    void storeData(QStringList list);

    void onHAngleChanged(int camNum, int angle);

    void onVAngleChanged(int camNum, int angle);

    void onZoomChanged(int camNum, int zoomNum);

    void change_camera_angle();

private:
    Ui::TopiServer *ui;
    QString camaraFileName;

    MainWindow *mainWin;

    QTcpServer *tcpServer;
    QTcpSocket *clientConnection;

    QList<QMap<QString, ObjInfo>> topTable;
    QList<CamInfo> camList;

    bool mutex;

};

struct CamInfo
{
    int x, y, z;
    bool modified;
    double abs_alpha, abs_beta, abs_zoom;
    double alpha, beta;
    double zoom;
};


#endif // TOPISERVER_H
