#include <QApplication>
#include <QSettings>

#include "ASLeaderboardAnalyzer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, "configs");

    ASLeaderboardAnalyzer w;
    w.show();

    return a.exec();
}
