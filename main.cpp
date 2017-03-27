#include "clipwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClipWindow w;
    w.show();

    return a.exec();
}
