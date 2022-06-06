#include "ASScoreAnalyzer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ASScoreAnalyzer w;
    w.show();
    return a.exec();
}
