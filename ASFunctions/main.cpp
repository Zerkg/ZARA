#include <QApplication>
#include "ASFunctions.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ASPhpLeaderboardQuery *pQuery(new ASPhpLeaderboardQuery(213488));
    pQuery->Execute();

    return a.exec();
}
