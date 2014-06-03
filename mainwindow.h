#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTextBrowser>
#include "controlpanel.h"
#include "picturebox.h"
#include <vector>
#include <QList>

using std::vector;

namespace Ui {
class MainWindow;
}

class ControlPanel;
class PictureBox;
struct ObjInfo;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *);

public slots:
    void getZoomNumber(int camNum);
    void onHAngleChanged(int camNum, int angle);
    void onVAngleChanged(int camNum, int angle);
    void onZoomChanged(int camNum, int zoomNum);

signals:
    void mainWindowClose();
    void currentZoomNum(int zoomNum);

signals:
    void hAngleChanged(int camNum, int angle);
    void vAngleChanged(int camNum, int angle);
    void zoomChanged(int camNum, int zoomNum);
    void getZoomNum(int camNum);
    
public slots:
    void on_action_Quit_triggered();

    void on_action_Control_Panel_triggered();

    void refreshLocalView(QList<QMap<QString, ObjInfo>> topTable);

private:
    Ui::MainWindow *ui;
    ControlPanel *controlPanel;


    PictureBox* globalView;
    QList<PictureBox*> localView;
    QTextBrowser* statusWindow;

};

struct ObjInfo
{
    QString key;
    bool modified, danger;
    QList<QPoint> position;
    int z;
    QPoint vector;
};

#endif // MAINWINDOW_H
