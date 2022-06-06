#ifndef PLAYERSTATSCONTROLLER_H
#define PLAYERSTATSCONTROLLER_H

#include "ASTypes.h"
#include <QWidget>

#include "PlayerStats.h"

class AbstractStatsConverter;
class PlayerStatsWidget;
namespace Ui { class PlayerStatsController; }


class PlayerStatsController : public QWidget
{
    Q_OBJECT

    using StatsData = std::vector<std::pair<PlayerStatsWidget *, PlayerStats>>;

public:

    PlayerStatsController(QWidget *parent = nullptr);
    PlayerStatsController(uchar viewsCount, QWidget *parent = nullptr);

public slots:

    void setViewsCount(uchar count);

    void displayStats(const PlayerStats &stats);

    void resetStats(const QString &rideId   );
    void resetStats(const PlayerStats &stats);

    void setDisplayMode(int mode);

    void reset();

private:

    void init();
    void createView();
    void deleteView();
    bool updateFocusWidget();

    Ui::PlayerStatsController *ui;

    StatsData m_StatsData;
    int m_DisplayMode;
    size_t m_FocusWidget;

signals:

    void statsLoaded(const QString &rideId);
    void statsCountChanged(int);
    void outOfFreeViews();

};

#endif // PLAYERSTATSCONTROLLER_H
