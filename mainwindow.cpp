#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QPainter>
#include <QPixmap>
#include <QPen>
//#include "transform.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controlPanel = new ControlPanel(this, Qt::Window);
    for(int i = 0; i < 4; i++)
    {
        controlPanel->addComboItems(QString("Camara No. %1").arg(i + 1));
    }

    controlPanel->show();

    connect(this, SIGNAL(mainWindowClose()), controlPanel, SLOT(onMainWindowClosed()));
    connect(controlPanel, SIGNAL(hAngleChanged(int,int)), this, SIGNAL(hAngleChanged(int,int)));
    connect(controlPanel, SIGNAL(vAngleChanged(int,int)), this, SIGNAL(vAngleChanged(int,int)));
    connect(controlPanel, SIGNAL(zoomChanged(int,int)), this, SIGNAL(zoomChanged(int,int)));
    connect(controlPanel, SIGNAL(getZoomNum(int)), this, SLOT(getZoomNumber(int)));
    connect(this, SIGNAL(currentZoomNum(int)), controlPanel, SLOT(refreshZoomNum(int)));

    globalView = new PictureBox();
    globalView->setFrameShape(QFrame::Box);
    localView.append(new PictureBox());
    localView[0]->setFrameShape(QFrame::Box);
    localView.append(new PictureBox());
    localView[1]->setFrameShape(QFrame::Box);
    localView.append(new PictureBox());
    localView[2]->setFrameShape(QFrame::Box);
    localView.append(new PictureBox());
    localView[3]->setFrameShape(QFrame::Box);
    statusWindow = new QTextBrowser();

    QGridLayout* topLayout = new QGridLayout();
    topLayout->addWidget(globalView, 0, 0, 3, 3);
    topLayout->addWidget(statusWindow, 3, 0, 1, 3);
    topLayout->addWidget(localView[0], 0, 3, 1, 1);
    topLayout->addWidget(localView[1], 1, 3, 1, 1);
    topLayout->addWidget(localView[2], 2, 3, 1, 1);
    topLayout->addWidget(localView[3], 3, 3, 1, 1);
    topLayout->setSpacing(10);
    for(int i = 0; i < 4; i++)
    {
        topLayout->setRowStretch(i, 1);
        topLayout->setColumnStretch(i, 1);
    }

    QWidget* topWidget = new QWidget;
    topWidget->setLayout(topLayout);
    this->setCentralWidget(topWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    emit mainWindowClose();
}

void MainWindow::getZoomNumber(int camNum)
{
    //emit currentZoomNum(sensorList[camNum].zoom);
}

void MainWindow::onHAngleChanged(int camNum, int angle)
{

}

void MainWindow::onVAngleChanged(int camNum, int angle)
{

}

void MainWindow::onZoomChanged(int camNum, int zoomNum)
{

}

void MainWindow::on_action_Quit_triggered()
{
    this->close();
}

void MainWindow::on_action_Control_Panel_triggered()
{
    if(controlPanel == NULL)
    {
        controlPanel = new ControlPanel(this, Qt::Window);
    }
    else
    {
        controlPanel->show();
        controlPanel->setFocus();
    }
}

void MainWindow::refreshLocalView(QList<QMap<QString, ObjInfo>> topTable)
{
    for(int i = 0; i < topTable.size() - 1; i++)
    {
        QPixmap *newMap = new QPixmap(100, 100);
        QPainter* painter = new QPainter();
        painter->begin(newMap);
        QMap<QString, ObjInfo>::iterator it;
        for(it = topTable[i].begin(); it != topTable[i].end(); it++)
        {
            painter->drawEllipse(it.value().position.last(), 3, 3);
            painter->drawLine(it.value().position.last(), it.value().position.last() + it.value().vector);
            painter->drawText(it.value().position.last(), it.value().key);
        }
        painter->end();
        localView[i]->setMap(newMap);
        delete newMap;
        delete painter;
    }
    QPixmap *newMap = new QPixmap(1000, 1000);
    QPainter* painter = new QPainter();
    painter->begin(newMap);
    QMap<QString, ObjInfo>::iterator it;
    for(it = topTable[4].begin(); it != topTable[4].end(); it++)
    {
        painter->drawEllipse(it.value().position.last(), 3, 3);
        painter->drawLine(it.value().position.last(), it.value().position.last() + it.value().vector);
        painter->drawText(it.value().position.last(), it.value().key);
    }
    painter->end();
    globalView->setMap(newMap);
    delete newMap;
    delete painter;
}
