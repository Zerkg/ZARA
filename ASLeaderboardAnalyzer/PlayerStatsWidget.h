#ifndef PLAYERSTATSWIDGET_H
#define PLAYERSTATSWIDGET_H

#include <QWidget>
#include "StatsAlias.h"

#include <list>

class QLabel;

class AbstractStatsConverter;

struct PlayerStats;

namespace Ui { class PlayerStatsWidget; class PowerupsWidget; }
namespace sc
{
inline constexpr std::array base_stats{ sc::SCORE, sc::RAW_SCORE, sc::DURATION };
inline constexpr std::array ext_stats { sc::CHARACTER_STAT_1, sc::CHARACTER_STAT_2, sc::OVERFILLS, sc::LONGEST_CHAIN, sc::DROPPED_CHAIN, sc::BEST_CLUSTER,
            sc::AVG_CLUSTER_SIZE, sc::AVG_CLUSTER_COLORS, sc::CONS_BLOCKS_COLLECTED, sc::CONS_BLOCKS_DODGED, sc::SKILL_RATING };
}

class StatWidget     : public QWidget
{
public:

    explicit StatWidget(sc::Stat stat, QWidget *parent = nullptr);
    explicit StatWidget(QWidget *parent = nullptr);

    void setName (sc::Stat stat);
    void setName (const QString &name);
    void setValue(const ConvertedStat &stat);
    void setDefaultValue();

    void reset();
    void resetValue();
    void resetName();

private:

    void init();

    QLabel *m_Name ;
    QLabel *m_Value;

};
class PowerupsWidget : public QWidget
{
Q_OBJECT
public:

    explicit PowerupsWidget(QWidget *parent = nullptr);
    void displayStats(const ConvertedPowerups &stats);

public slots:

    void reset();

private:

    void init();

    Ui::PowerupsWidget *ui;
    std::array<QWidget*, 4> m_Output;
};
class BonusesWidget  : public QWidget
{
Q_OBJECT
public:

    explicit BonusesWidget(QWidget *parent = nullptr);
    void displayStats(const ConvertedBonuses &stats);

public slots:

    void reset();

private:

    void init();

    Ui::PowerupsWidget *ui;
    std::list<StatWidget*> m_Output;
};


class PlayerStatsWidget : public QWidget
{
Q_OBJECT
public:

    explicit PlayerStatsWidget(QWidget *parent = nullptr);
    virtual ~PlayerStatsWidget();

    void displayStats(const PlayerStats& stats);
    void setStatsConverter(AbstractStatsConverter *converter) noexcept;
    AbstractStatsConverter* statsConverter() const noexcept;

public slots:

    void reset();

private:

    void init();

    Ui::PlayerStatsWidget *ui;
    AbstractStatsConverter *m_StatsConverter;
    std::map<sc::Stat, StatWidget*> m_StatsOutput;

};

#endif // PLAYERSTATSWIDGET_H
