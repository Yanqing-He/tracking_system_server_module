#include "picturebox.h"

PictureBox::PictureBox(QWidget *parent) :
    QFrame(parent)
{
    this->setAutoFillBackground(true);
}

void PictureBox::setMap(QPixmap *newMap)
{
    this->map = *newMap;
    this->refreshMap();
}

void PictureBox::refreshMap()
{
    QPalette palette;
    palette.setBrush(QPalette::Window,
                    QBrush(this->map.scaled(this->size(),
                    Qt::IgnoreAspectRatio,
                    Qt::FastTransformation)));
    this->setPalette(palette);
}

void PictureBox::resizeEvent(QResizeEvent *)
{
    QPalette palette;
    palette.setBrush(QPalette::Window,
                    QBrush(this->map.scaled(this->size(),
                    Qt::IgnoreAspectRatio,
                    Qt::SmoothTransformation)));
    this->setPalette(palette);
}
