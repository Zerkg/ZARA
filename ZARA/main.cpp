#include "ZARAMainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ZARAMainWindow w;
    w.show();

    return a.exec();
}
