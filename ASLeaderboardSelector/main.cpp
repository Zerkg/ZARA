#include "ASLeaderboardSelector.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ASLeaderboardSelector w;
    w.show();
    return a.exec();
}
