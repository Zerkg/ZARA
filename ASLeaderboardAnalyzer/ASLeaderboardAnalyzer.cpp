#include "ASLeaderboardAnalyzer.h"
#include "ui_ASLeaderboardAnalyzer.h"

#include "StatsChart.h"
#include "ModuleManual.h"

using namespace zara;

ASLeaderboardAnalyzer::ASLeaderboardAnalyzer(QWidget *parent) : QMainWindow(parent) { init(); }

void ASLeaderboardAnalyzer::init()
{       
    ui = new Ui::ASLeaderboardAnalyzer;
    ui->setupUi(this);

    ui->scoreSelector->setSelectionLimit(4);
    ui->playerStatsController->setViewsCount(4);

    StatsChart *charts(new StatsChart(this));
    charts->resize(960, 480);
    charts->setSource(ui->scoreSelector->source());
    charts->setWindowFlag(Qt::Window);
    charts->hide();

    ui->pushButton_Charts->setIcon(charts->windowIcon());
    ui->pushButton_Charts->setIconSize({ 30, 30 });

    ModuleManual *manual(new ModuleManual(this));
    manual->setSourcePath(QUrl::fromLocalFile("Doc/Leaderboard Analyzer/MainPage.htm"));
    manual->setWindowFlag(Qt::Window);
    manual->hide();

    connect(ui->leaderboardSearcher, &ASLeaderboardSearcher::songSelected, ui->scoreSelector, &ASScoreSelector::loadLeaderboard);

    connect(ui->scoreSelector, &ASScoreSelector::scoreSelected  , ui->playerStatsController, &PlayerStatsController::displayStats);
    connect(ui->scoreSelector, &ASScoreSelector::scoreDeselected, ui->playerStatsController, QOverload<const PlayerStats&>::of(&PlayerStatsController::resetStats));
    connect(ui->comboBox_DisplayMode, QOverload<int>::of(&QComboBox::activated), ui->playerStatsController, &PlayerStatsController::setDisplayMode);

    connect(ui->pushButton_Charts, &QPushButton::clicked, charts, &StatsChart  ::show);
    connect(ui->action_UserManual, &QAction::triggered  , manual, &ModuleManual::show);
}



























