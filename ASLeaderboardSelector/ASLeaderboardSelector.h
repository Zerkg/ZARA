#ifndef ASLEADERBOARDSELECTOR_H
#define ASLEADERBOARDSELECTOR_H

#include "ASLeaderboardSelector_global.h"

#include <QWidget>

#include "ASTypes.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ASLeaderboardSelector; }
QT_END_NAMESPACE


struct PlayerStats;
class ASLeaderboard;
class ASPhpLeaderboardQuery;


class ASLEADERBOARDSELECTOR_EXPORT ASLeaderboardSelector : public QWidget
{
    Q_OBJECT

public:

    explicit ASLeaderboardSelector(QWidget *parent = nullptr) noexcept;
    ~ASLeaderboardSelector() noexcept;

    const ASLeaderboard* leaderboard() const noexcept;
    const ASLeaderboard* leaderboard(as::Mode mode) const noexcept;
    as::Mode currentMode() const noexcept;

public slots:

    void loadLeaderboard(int id) noexcept;
    void reset() noexcept;

private:

    Ui::ASLeaderboardSelector *ui;

    ASPhpLeaderboardQuery *m_LeaderboardQuery;
    const ASLeaderboard *m_CurrentLeaderboard;

private slots:

    void displayModeData(const QString &mode) noexcept;
    void processQueryData() noexcept;

signals:

    void leaderboardChanged();
    void modeChanged(as::Mode);

};
#endif // ASLEADERBOARDSELECTOR_H
