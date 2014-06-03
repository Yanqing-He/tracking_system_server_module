#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QFrame>

class PictureBox : public QFrame
{
    Q_OBJECT
public:
    explicit PictureBox(QWidget *parent = 0);
    void setMap(QPixmap* map);
    void refreshMap();

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *);

private:
    QPixmap map;
    
};

#endif // PICTUREBOX_H
