#include <QApplication>
#include <QSettings>

#include "ASLeaderboardAnalyzer.h"
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));

    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, "configs");

    ASLeaderboardAnalyzer w;
    w.show();

    return a.exec();
}
