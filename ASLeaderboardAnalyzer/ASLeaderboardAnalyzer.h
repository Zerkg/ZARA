#ifndef AS_LEADERBOARD_ANALYZER_H
#define AS_LEADERBOARD_ANALYZER_H

#include "ASLeaderboardAnalyzer_global.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ASLeaderboardAnalyzer; }
QT_END_NAMESPACE


class ASLEADERBOARDANALYZER_EXPORT ASLeaderboardAnalyzer : public QMainWindow
{
Q_OBJECT

public:

    ASLeaderboardAnalyzer(QWidget *parent = nullptr);

private:

    void init();
    Ui::ASLeaderboardAnalyzer *ui;   

};
#endif // AS_LEADERBOARD_ANALYZER_H
