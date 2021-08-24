#include "ZARAMainWindow.h"
#include "ui_ZARAMainWindow.h"

#include "ASLeaderboardAnalyzer.h"
#include "ASScoreAnalyzer.h"

#include <QStyle>

ZARAMainWindow::ZARAMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ZARAMainWindow)
{
    ui->setupUi(this);
    init();
}

ZARAMainWindow::~ZARAMainWindow()
{
    delete ui;
}

void ZARAMainWindow::init()
{
    QAction *pActionHelp { new QAction("Help")};
    pActionHelp->setShortcut(QKeySequence("F1"));

    ui->menubar->addAction(pActionHelp);

    ASLeaderboardAnalyzer *pLeaderboardAnalyzer(new ASLeaderboardAnalyzer(this));
    ASScoreAnalyzer       *pScoreAnalyzer      (new ASScoreAnalyzer      (this));

    pLeaderboardAnalyzer->setWindowIcon(QPixmap(":/Icon/LeaderboardAnalyzer.png"));
    pScoreAnalyzer      ->setWindowIcon(QPixmap(":/Icon/ASScoreAnalyzer.png"    ));

    ui->pushButton_LeaderboardAnalyzer->setIcon(QPixmap(":/Icon/LeaderboardAnalyzer.png"));
    ui->pushButton_LeaderboardAnalyzer->setIconSize(QSize(60, 60));

    ui->pushButton_ScoreAnalyzer->setIcon(QPixmap(":/Icon/ASScoreAnalyzer.png"));
    ui->pushButton_ScoreAnalyzer->setIconSize(QSize(60, 60));

    QObject::connect(ui->pushButton_ScoreAnalyzer      , &QPushButton::clicked, pScoreAnalyzer      , &ASScoreAnalyzer      ::show);
    QObject::connect(ui->pushButton_LeaderboardAnalyzer, &QPushButton::clicked, pLeaderboardAnalyzer, &ASLeaderboardAnalyzer::show);
}
