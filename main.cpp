#include "topiserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TopiServer w;
    w.show();
    
    return a.exec();
}
