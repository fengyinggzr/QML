#include "portcomm.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    portComm w;
    w.show();

    return a.exec();
}
