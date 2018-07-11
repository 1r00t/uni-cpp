#include "ircclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    IrcClient w;
    w.show();

    return a.exec();
}
