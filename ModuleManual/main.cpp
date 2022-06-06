#include "ModuleManual.h"

#include <QApplication>

#include <QUrl>
#include <QFileDialog>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    zara::ModuleManual w;
    w.setSourcePath(QUrl::fromLocalFile("../build-Test-Desktop_Qt_5_15_2_MinGW_32_bit/Data/MainPage.htm"));
    w.show();
    return a.exec();
}
