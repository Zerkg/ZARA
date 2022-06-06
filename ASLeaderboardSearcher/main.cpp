#include "ASLeaderboardSearcher.h"

#include <QApplication>
#include <QSettings>
#include <QStyleFactory>

int main(int argc, char *argv[])
{  
    QApplication a(argc, argv);
    QSettings applicationSettings("Data/Config.ini", QSettings::IniFormat, &a);
    a.setStyle(QStyleFactory::create("Fusion"));

    ASLeaderboardSearcher w;
    w.show();

    return a.exec();
}
