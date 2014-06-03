#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QString>

namespace Ui {
class ControlPanel;
}

class ControlPanel : public QWidget
{
    Q_OBJECT
    
public:
    explicit ControlPanel(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~ControlPanel();
    void addComboItems(QString name);

signals:
    void hAngleChanged(int camNum, int angle);
    void vAngleChanged(int camNum, int angle);
    void zoomChanged(int camNum, int zoomNum);
    void getZoomNum(int camNum);

public slots:
    void refreshZoomNum(int zoomNum);
    
private slots:
    void on_leftButton_clicked();

    void on_rightButton_clicked();

    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_zoomInButton_clicked();

    void on_zoomOutButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::ControlPanel *ui;
};

#endif // CONTROLPANEL_H
