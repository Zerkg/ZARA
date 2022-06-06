#include "StatsChart.h"

#include "ASPhpQuery.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ASPhpLeaderboardQuery *lQuery(new ASPhpLeaderboardQuery(1734776));

    StatsChart w;
    w.show();

    w.setSource(lQuery);

    lQuery->execute();

    return a.exec();
}
