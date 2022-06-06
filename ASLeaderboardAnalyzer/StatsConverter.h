#ifndef STATSMODELMODIFIER_H
#define STATSMODELMODIFIER_H

#include "PlayerStats.h"
#include "StatsAlias.h"

#include <QColor>

#include <map>

class AbstractStatsConverter
{
public:

    virtual ~AbstractStatsConverter() = default;

    void convertToText(const PlayerStats& stats);
    ConvertedStat stat(sc::Stat stat) const;

    ConvertedBlocks blocks() const;
    ConvertedBonuses bonuses() const;
    ConvertedPowerups powerups() const;

    void clear() noexcept;

protected:

    virtual void convertGeneralStats    (const PlayerStats& stats) = 0;
    virtual void convertAfterrideBonuses(const PlayerStats& stats) = 0;
    virtual void convertRideStats       (const PlayerStats& stats) = 0;
    virtual void convertBlocksStats     (const PlayerStats& stats) = 0;
    virtual void convertPowerupsStats   (const PlayerStats& stats) = 0;

    std::map<sc::Stat, ConvertedStat> m_Stats;
    sc::AdvancedStats m_AdvStats;

    ConvertedBonuses m_Bonuses;
    ConvertedPowerups m_Powerups;
    ConvertedBlocks m_Blocks;
};

class StatsDefaultConverter : public AbstractStatsConverter
{

protected:

    virtual void convertGeneralStats    (const PlayerStats& stats) override;
    virtual void convertAfterrideBonuses(const PlayerStats& stats) override;
    virtual void convertRideStats       (const PlayerStats& stats) override;
    virtual void convertBlocksStats     (const PlayerStats& stats) override;
    virtual void convertPowerupsStats   (const PlayerStats& stats) override;

};

class StatsDiffConverter : public StatsDefaultConverter
{
public:

    StatsDiffConverter() = default;
    StatsDiffConverter(const PlayerStats& focusStats);

    void setFocusStats(const PlayerStats &stats);

protected:

    virtual void convertGeneralStats    (const PlayerStats& stats) override;
    virtual void convertAfterrideBonuses(const PlayerStats& stats) override;
    virtual void convertRideStats       (const PlayerStats& stats) override;
    virtual void convertBlocksStats     (const PlayerStats& stats) override;
    virtual void convertPowerupsStats   (const PlayerStats& stats) override;

private:



    template<typename T>
    ConvertedStat findDiff(T first, T second,
                           const QColor &more = QColorConstants::DarkGreen, const QColor &less = QColorConstants::Red,
                           const char* printFormat = std::is_integral_v<T> ? "%d (%+d)" : "%.1f (%+.1f)") const;

    std::optional<PlayerStats> m_FocusStats;
    sc::AdvancedStats m_AdvFocusStats;

};
//class StatsMinMaxConverter : public AbstractStatsConverter
//{
//public:

//    void addStats(const PlayerStats &stats) noexcept;
//    void removeStats(const PlayerStats &stats) noexcept;
//    void clearStatsList() noexcept;

//private:

//    std::list<PlayerStats> m_StatsList;

//};
//class StatsAnalyzeConverter : public AbstractStatsConverter
//{
//public:

//};

#endif // STATSMODELMODIFIER_H























