#include "ZARAMainWindow.h"
#include "ui_ZARAMainWindow.h"

#include "ASLeaderboardAnalyzer.h"

#include <QStyle>

ZARAMainWindow::ZARAMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ZARAMainWindow)
{
    ui->setupUi(this);
    init();
}

void ZARAMainWindow::init()
{
    ASLeaderboardAnalyzer *pLeaderboardAnalyzer(new ASLeaderboardAnalyzer(this));
    pLeaderboardAnalyzer->setWindowIcon(QPixmap(":/Icon/LeaderboardAnalyzer.png"));

    ui->pushButton_LeaderboardAnalyzer->setIcon(QPixmap(":/Icon/LeaderboardAnalyzer.png"));
    ui->pushButton_LeaderboardAnalyzer->setIconSize(QSize(60, 60));

    QObject::connect(ui->pushButton_LeaderboardAnalyzer, &QPushButton::clicked, pLeaderboardAnalyzer, &ASLeaderboardAnalyzer::show);
}
