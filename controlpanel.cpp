#include "controlpanel.h"
#include "ui_controlpanel.h"

ControlPanel::ControlPanel(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);
    this->setFixedSize(179, 329);
}

ControlPanel::~ControlPanel()
{
    delete ui;
}

void ControlPanel::addComboItems(QString name)
{
    ui->comboBox->addItem(name);
}

void ControlPanel::refreshZoomNum(int zoomNum)
{
    ui->zoomNumber->setText(QString("%1").arg(zoomNum));
}

void ControlPanel::on_leftButton_clicked()
{
    emit hAngleChanged(ui->comboBox->currentIndex(), 10);
}

void ControlPanel::on_rightButton_clicked()
{
    emit hAngleChanged(ui->comboBox->currentIndex(), -10);
}

void ControlPanel::on_upButton_clicked()
{
    emit vAngleChanged(ui->comboBox->currentIndex(), 5);
}

void ControlPanel::on_downButton_clicked()
{
    emit vAngleChanged(ui->comboBox->currentIndex(), -5);
}

void ControlPanel::on_zoomInButton_clicked()
{
    int zoomNum = ui->zoomNumber->text().toInt();
    zoomNum += 1;
    ui->zoomNumber->setText(QString("%1").arg(zoomNum));
    emit zoomChanged(ui->comboBox->currentIndex(), zoomNum);
}

void ControlPanel::on_zoomOutButton_clicked()
{
    int zoomNum = ui->zoomNumber->text().toInt();
    zoomNum -= 1;
    ui->zoomNumber->setText(QString("%1").arg(zoomNum));
    emit zoomChanged(ui->comboBox->currentIndex(), zoomNum);
}

void ControlPanel::on_comboBox_currentIndexChanged(int index)
{
    emit getZoomNum(index);
}
