#include "ASLeaderboardSearcher.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{  
    QApplication a(argc, argv);

    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, "configs/cfg.ini");

    ASLeaderboardSearcher w;
    w.show();

    return a.exec();
}
