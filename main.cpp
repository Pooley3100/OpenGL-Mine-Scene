#include "minemain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MineMain *w = new MineMain(NULL);
    w->resize(1600,1200);
    w->show();
    return a.exec();
}
