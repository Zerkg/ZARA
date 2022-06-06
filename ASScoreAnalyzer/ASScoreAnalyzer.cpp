#include "ASScoreAnalyzer.h"
#include "ui_ASScoreAnalyzer.h"

#include "ASCharacter.h"
#include "ASFunctions.h"
#include "ASPhpQuery.h"

ASScoreAnalyzer::ASScoreAnalyzer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ASScoreAnalyzer)
{
    ui->setupUi(this);
    init();
}

ASScoreAnalyzer::~ASScoreAnalyzer()
{
    delete ui;
}

void ASScoreAnalyzer::init()
{
    QObject::connect(ui->widget_Searcher  , &ASLeaderboardSearcher::songSelected, ui->widget_Selector, &ASLeaderboardSelector::loadLeaderboard);
    QObject::connect(ui->widget_Selector  , &ASLeaderboardSelector::modeChanged , this, &ASScoreAnalyzer::displayLeaderboard);
    QObject::connect(ui->listWidget_Scores, &QListWidget::itemDoubleClicked     , this, &ASScoreAnalyzer::displayAnalyze);
}

void ASScoreAnalyzer::displayLeaderboard()
{
    ui->listWidget_Scores->clear();

    auto size(ui->widget_Selector->leaderboard()->size());

    for(int i{}; i < size; ++i)
    {
        PlayerStats s(ui->widget_Selector->leaderboard()->statsAt(i));
        QListWidgetItem *pScore(new QListWidgetItem);

        pScore->setIcon(as::characterPixmap(s.character->getType(), s.character->getMode()));
        pScore->setText(s.nickname + " - " + as::convertNumber(s.score));

        ui->listWidget_Scores->addItem(pScore);
    }

}
void ASScoreAnalyzer::displayAnalyze()
{
    int currentPlayer(ui->listWidget_Scores->currentIndex().row());
    ui->textEdit_ScoreAnalyze->clear();

    while (!ui->widget_Selector->leaderboard()->isStatsValid(currentPlayer))
        QApplication::processEvents();

    auto playerStats(ui->widget_Selector->leaderboard()->statsAt(currentPlayer));

    double rawScore(playerStats.bonuses.applyBonuses(playerStats.score, false));

    ui->textEdit_ScoreAnalyze->append("Total score: "    + as::convertNumber(playerStats.score));
    ui->textEdit_ScoreAnalyze->append("Raw score: "      + as::convertNumber(rawScore) + '\n');
    ui->textEdit_ScoreAnalyze->append("Character used: " + as::characterToString(playerStats.character->getType(), playerStats.character->getMode()) + '\n');

    QString strTemplate("<font color=\"%2\">%1</font>");
    QStringList lostBonuses;

    double lostScore{};

    if (playerStats.bonuses.isBonusAchieved(ASAfterrideBonus::CLEAN_FINISH))
        ui->textEdit_ScoreAnalyze->append(strTemplate.arg("Clean Finish (+" + as::convertNumber(rawScore * 0.25) + " pts)", "green"));
    else
    {
        lostBonuses << strTemplate.arg("Missed Clean Finish (-" + as::convertNumber(rawScore * 0.25) + " pts)", "red");
        lostScore += 0.25;
    }



    switch (playerStats.character->getType())
    {
    case as::MONO:
    {
        if (playerStats.bonuses.isBonusAchieved(ASAfterrideBonus::STEALTH))
            ui->textEdit_ScoreAnalyze->append(strTemplate.arg("Stealth (+" + as::convertNumber(rawScore * 0.3) + " pts)", "green"));
        else
        {
            lostBonuses << strTemplate.arg("Missed Stealth (-" + as::convertNumber(rawScore * 0.3) + " pts)", "red");
            lostScore += 0.3;
        }
        break;
    }
    default: break;
    }



    if (ASAfterrideBonus::Bonus matchBonus(playerStats.bonuses.matchBonus()); matchBonus == ASAfterrideBonus::MATCH_21)
        ui->textEdit_ScoreAnalyze->append(strTemplate.arg("Match 21 (+" + as::convertNumber(rawScore * 0.21) + " pts)", "green"));

    else if  (matchBonus == ASAfterrideBonus::MATCH_11)
    {
        ui->textEdit_ScoreAnalyze->append(strTemplate.arg("Match 11 (+" + as::convertNumber(rawScore * 0.11) + " pts)", "orange"));

        lostBonuses << strTemplate.arg("Missed Match 21 (-" + as::convertNumber(rawScore * 0.1) + " pts)", "red");
        lostScore += 0.1;
    }

    else if (matchBonus == ASAfterrideBonus::MATCH_7)
    {
        ui->textEdit_ScoreAnalyze->append(strTemplate.arg("Match 7 (+" + as::convertNumber(rawScore * 0.07) + " pts)", "red"));

        lostBonuses << strTemplate.arg("Missed Match 21 (-" + as::convertNumber(rawScore * 0.14) + " pts)", "red");
        lostScore += 0.14;
    }

    else
    {
        lostBonuses << strTemplate.arg("Missed Match 21 (-" + as::convertNumber(rawScore * 0.21) + " pts)", "red");
        lostScore += 0.21;
    }



    switch (playerStats.character->getType())
    {
    case as::POINTMAN:
    case as::ERASER:
    {
        if (!playerStats.bonuses.isBonusAchieved(ASAfterrideBonus::SEEING_RED))
        {
            lostBonuses << strTemplate.arg("Missed Seeing Red (-" + as::convertNumber(rawScore * 0.05) + " pts)", "red");
            lostScore += 0.05;
        }

        if (!playerStats.bonuses.isBonusAchieved(ASAfterrideBonus::BUTTER_NINJA))
        {
            lostBonuses << strTemplate.arg("Missed Butter Ninja (-" + as::convertNumber(rawScore * 0.05) + " pts)", "red");
            lostScore += 0.05;
        }
    }
    [[fallthrough]]; default:
    {
        if (playerStats.bonuses.isBonusAchieved(ASAfterrideBonus::SEEING_RED))
            ui->textEdit_ScoreAnalyze->append(strTemplate.arg("Seeing Red (+" + as::convertNumber(rawScore * 0.05) + " pts)", "green"));


        if (playerStats.bonuses.isBonusAchieved(ASAfterrideBonus::BUTTER_NINJA))
            ui->textEdit_ScoreAnalyze->append(strTemplate.arg("Butter Ninja (+" + as::convertNumber(rawScore * 0.05) + " pts)", "green"));
    }
    }
    ui->textEdit_ScoreAnalyze->append("\nTotal bonuses score: " + as::convertNumber(playerStats.score - rawScore) + " pts\n");

    if (!lostBonuses.isEmpty())
    {
        foreach (const auto string, lostBonuses) ui->textEdit_ScoreAnalyze->append(string);
        ui->textEdit_ScoreAnalyze->append("\nTotal potential score missed: " + as::convertNumber(lostScore * rawScore) + " pts");
    }
}

