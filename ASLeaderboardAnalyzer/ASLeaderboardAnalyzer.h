#ifndef AS_LEADERBOARD_ANALYZER_H
#define AS_LEADERBOARD_ANALYZER_H

#include "ASLeaderboardAnalyzer_global.h"

#include <QMainWindow>


class ASModuleConfigs;
class CompareModeSettings;


QT_BEGIN_NAMESPACE
namespace Ui { class ASLeaderboardAnalyzer; }
QT_END_NAMESPACE


class ASLEADERBOARDANALYZER_EXPORT ASLeaderboardAnalyzer : public QMainWindow
{
Q_OBJECT

public:

    ASLeaderboardAnalyzer(QWidget *parent = nullptr);
    virtual ~ASLeaderboardAnalyzer() override;

protected:

    virtual void timerEvent(QTimerEvent *event) override;
    virtual void moveEvent(QMoveEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:

    void init();
    Ui::ASLeaderboardAnalyzer *ui;   

    ASModuleConfigs       *m_Configs;

    CompareModeSettings *m_CompareSettings;

};
#endif // AS_LEADERBOARD_ANALYZER_H
