#include "StatsConverter.h"

#include "StatsAlias.h"
#include "PlayerStats.h"
#include "ASFunctions.h"

using namespace QColorConstants;

void AbstractStatsConverter::convertToText(const PlayerStats &stats)
{
    clear();

    m_AdvStats = sc::calcAdvanced(stats);

    convertGeneralStats    (stats);
    convertAfterrideBonuses(stats);

    if (stats.validationFlags[PlayerStats::EXT] == PlayerStats::VALID)
    {
        convertRideStats(stats);

        if (stats.characterData.type != as::MONO)
        {
            convertBlocksStats  (stats);
            convertPowerupsStats(stats);
        }
    }
}

ConvertedStat AbstractStatsConverter::stat(sc::Stat stat) const
{
    auto iter(m_Stats.find(stat));

    if (iter != m_Stats.cend())
        return iter->second;
    else
        return { "N/A" };
}
ConvertedBlocks   AbstractStatsConverter::blocks  () const { return m_Blocks ;  }
ConvertedBonuses  AbstractStatsConverter::bonuses () const { return m_Bonuses;  }
ConvertedPowerups AbstractStatsConverter::powerups() const { return m_Powerups; }

void AbstractStatsConverter::clear() noexcept
{
    m_Stats   .clear();
    m_Blocks  .clear();
    m_Bonuses .clear();
    m_Powerups.clear();
}

void StatsDefaultConverter::convertGeneralStats    (const PlayerStats &stats)
{
    m_Stats[sc::SCORE    ] = { as::commaPrint(stats.score)             };
    m_Stats[sc::RAW_SCORE] = { as::commaPrint(m_AdvStats.rawScore    ) };
    m_Stats[sc::DURATION ] = { stats.trackDuration.toString("mm':'ss") };
}
void StatsDefaultConverter::convertAfterrideBonuses(const PlayerStats &stats)
{
    for (const auto bonus : stats.bonuses.vectorize())
    {
        auto[boost, name](ASAfterrideBonus::bonusInfo(bonus));
        m_Bonuses.push_back({ name, { bonus == as::IRONMODE ? "" : '+' + as::commaPrint(qRound(m_AdvStats.rawScore * (boost - 1))) } });
    }
}
void StatsDefaultConverter::convertRideStats       (const PlayerStats &stats)
{            
    m_Stats[sc::CHARACTER_STAT_1     ] = { QString::number(stats.characterData.stats[0]) };
    m_Stats[sc::CHARACTER_STAT_2     ] = { QString::number(stats.characterData.stats[1]) };
    m_Stats[sc::OVERFILLS            ] = { QString::number(stats.overfillsCount        ) };
    m_Stats[sc::LONGEST_CHAIN        ] = { QString::number(stats.longestChainTime      ) };
    m_Stats[sc::DROPPED_CHAIN        ] = { QString::number(stats.droppedChains         ) };
    m_Stats[sc::BEST_CLUSTER         ] = { QString::number(stats.bestCluster           ) };
    m_Stats[sc::AVG_CLUSTER_SIZE     ] = { QString::number(stats.avgClusterSize, 'f', 1) };
    m_Stats[sc::AVG_CLUSTER_COLORS   ] = { QString::number(stats.avgColorsCount, 'f', 1) };
    m_Stats[sc::CONS_BLOCKS_DODGED   ] = { QString::number(stats.maxConsBlocksDodged   ) };
    m_Stats[sc::CONS_BLOCKS_COLLECTED] = { QString::number(stats.maxConsBlocksCollected) };
    m_Stats[sc::SKILL_RATING         ] = { QString::number(stats.skillRating           ) };
}
void StatsDefaultConverter::convertBlocksStats     (const PlayerStats &stats)
{
    for (const auto color : as::colors_array)
    {
        if (stats.blocksStats[color][0] == 0)
            continue;

        std::array<ConvertedStat, 4> blockStats;

        for (size_t stat{}; stat < 3; ++stat)
            blockStats[stat].text = QString::number(stats.blocksStats[color][stat]);

        blockStats[3].text = QString::number(m_AdvStats.colorBlocksCount[color]);

        m_Blocks.insert(m_Blocks.cend(), { color, std::move(blockStats) });
    }
    m_Stats[sc::BLOCKS_COUNT] = { QString::number(m_AdvStats.totalBlocksCount) };
    m_Stats[sc::TRAFFIC     ] = { QString::number(m_AdvStats.traffic, 'f', 1 ) };
}
void StatsDefaultConverter::convertPowerupsStats   (const PlayerStats &stats)
{
    for (const auto p : as::powerups_array)
        if (stats.powerups[p] > 0)
            m_Powerups.insert(m_Powerups.cend(), { p, { QString::number(stats.powerups[p]) } });
}


StatsDiffConverter::StatsDiffConverter(const PlayerStats &focusStats)
{
    setFocusStats(focusStats);
}
void StatsDiffConverter::setFocusStats(const PlayerStats &stats)
{
    m_FocusStats = stats;
    m_AdvFocusStats = sc::calcAdvanced(stats);
}

void StatsDiffConverter::convertGeneralStats    (const PlayerStats &stats)
{
    auto comma([](uint v1, uint v2)->ConvertedStat
    {
        ConvertedStat stat{ as::commaPrint(v1) };

        if      (v1 > v2)
        {
            stat.text.append(" (+" + as::commaPrint(v1 - v2) + ")");
            stat.styleSheet = "color: " + QColorConstants::DarkGreen.name() + ";";
        }
        else if (v1 < v2)
        {
            stat.text.append(" (-" + as::commaPrint((v1 - v2) * -1) + ")");
            stat.styleSheet = "color: " + QColorConstants::Red      .name() + ";";
        }

        return stat;
    });

    m_Stats[sc::SCORE    ] = comma(stats.score, m_FocusStats->score);
    m_Stats[sc::RAW_SCORE] = comma(m_AdvStats.rawScore, m_AdvFocusStats.rawScore);

    m_Stats[sc::DURATION].text = stats.trackDuration.toString("mm':'ss");
    if (stats.trackDuration != m_FocusStats->trackDuration)
        m_Stats[sc::DURATION ].styleSheet = "color: " + Svg::darkorange.name() + ";";
}
void StatsDiffConverter::convertAfterrideBonuses(const PlayerStats &stats)
{
    auto compare([this, &stats](as::Bonus bonus, const QColor &achievedColor)
    {
        if (stats.bonuses.isAchieved(bonus))
        {
            auto[boost, name](ASAfterrideBonus::bonusInfo(bonus));

            ConvertedStat s{ bonus == as::IRONMODE ? "" : '+' + as::commaPrint(qRound(m_AdvStats.rawScore * (boost - 1))) };

            if (!m_FocusStats->bonuses.isAchieved(bonus))
                s.styleSheet = "color: " + achievedColor.name() + ";";

            m_Bonuses.push_back({ name, s });
        }
    });

    compare(as::IRONMODE    , Svg::darkorange);
    compare(as::CLEAN_FINISH, DarkGreen      );

    if (auto matchBonus(stats.bonuses.matchBonus()); matchBonus)
    {
        if (auto focusMatch(m_FocusStats->bonuses.matchBonus()); matchBonus > focusMatch)
            compare(matchBonus, DarkGreen);
        else
            compare(matchBonus, Red      );
    }

    if (stats.characterData.type == as::MONO)
    {
        if (m_FocusStats->characterData.type == as::MONO)
            compare(as::STEALTH, DarkGreen);
        else
            compare(as::STEALTH, Black    );
    }
    else
        for (const auto bonus : { as::BUTTER_NINJA, as::SEEING_RED })
            compare(bonus, DarkGreen);
}
void StatsDiffConverter::convertRideStats       (const PlayerStats &stats)
{
    m_Stats[sc::OVERFILLS            ] = findDiff(stats.overfillsCount        , m_FocusStats->overfillsCount     , Red, DarkGreen);
    m_Stats[sc::LONGEST_CHAIN        ] = findDiff(stats.longestChainTime      , m_FocusStats->longestChainTime           );
    m_Stats[sc::DROPPED_CHAIN        ] = findDiff(stats.droppedChains         , m_FocusStats->droppedChains      , Red, DarkGreen);
    m_Stats[sc::BEST_CLUSTER         ] = findDiff(stats.bestCluster           , m_FocusStats->bestCluster                );
    m_Stats[sc::AVG_CLUSTER_SIZE     ] = findDiff(stats.avgClusterSize        , m_FocusStats->avgClusterSize             );
    m_Stats[sc::AVG_CLUSTER_COLORS   ] = findDiff(stats.avgColorsCount        , m_FocusStats->avgColorsCount             );
    m_Stats[sc::CONS_BLOCKS_DODGED   ] = findDiff(stats.maxConsBlocksDodged   , m_FocusStats->maxConsBlocksDodged, Red, DarkGreen);
    m_Stats[sc::CONS_BLOCKS_COLLECTED] = findDiff(stats.maxConsBlocksCollected, m_FocusStats->maxConsBlocksCollected     );
    m_Stats[sc::SKILL_RATING         ] = findDiff(stats.skillRating           , m_FocusStats->skillRating                );

    if (m_FocusStats->characterData.type == stats.characterData.type)
    {
        m_Stats[sc::CHARACTER_STAT_1] = findDiff(stats.characterData.stats[0], m_FocusStats->characterData.stats[0], Svg::darkorange, Svg::darkorange);
        m_Stats[sc::CHARACTER_STAT_2] = findDiff(stats.characterData.stats[1], m_FocusStats->characterData.stats[1], Svg::darkorange, Svg::darkorange);
    }
    else
    {
        m_Stats[sc::CHARACTER_STAT_1] = { QString::number(stats.characterData.stats[0]) };
        m_Stats[sc::CHARACTER_STAT_2] = { QString::number(stats.characterData.stats[1]) };
    }
}
void StatsDiffConverter::convertBlocksStats     (const PlayerStats &stats)
{
    if (m_FocusStats->characterData.type == as::MONO)
    {
        StatsDefaultConverter::convertBlocksStats(stats);
        return;
    }

    for (const auto color : as::colors_array)
    {
        if (stats.blocksStats[color][0] == 0)
            continue;

        std::array<ConvertedStat, 4> blockStats;

        for (size_t stat{}; stat < 3; ++stat)
            blockStats[stat] = findDiff(stats.blocksStats[color][stat], m_FocusStats->blocksStats[color][stat], DarkGreen, Red, "%d<br>(%+d)");

        blockStats[3] = findDiff(m_AdvStats.colorBlocksCount[color], m_AdvFocusStats.colorBlocksCount[color], DarkGreen, Red, "%d<br>(%+d)");

        m_Blocks.insert(m_Blocks.cend(), { color, std::move(blockStats) });
    }
    m_Stats[sc::BLOCKS_COUNT] = findDiff(m_AdvStats.totalBlocksCount, m_AdvFocusStats.totalBlocksCount);
    m_Stats[sc::TRAFFIC     ] = findDiff(m_AdvStats.traffic         , m_AdvFocusStats.traffic         );
}
void StatsDiffConverter::convertPowerupsStats   (const PlayerStats &stats)
{
    if (m_FocusStats->characterData.type == as::MONO)
    {
        StatsDefaultConverter::convertPowerupsStats(stats);
        return;
    }

    for (const auto p : as::powerups_array)
        if (stats.powerups[p] > 0)
            m_Powerups.insert(m_Powerups.cend(), { p, findDiff(stats.powerups[p], m_FocusStats->powerups[p], Svg::darkorange, Svg::darkorange, "%d<br>(%+d)") });
}


template<typename T>
ConvertedStat StatsDiffConverter::findDiff(T first, T second, const QColor &more, const QColor &less, const char *printFormat) const
{
    QString color;

    if      (first > second)
        color = more.name();
    else if (first < second)
        color = less.name();
    else
        return { QString::number(first) };

    return { QString::asprintf(printFormat, first, first - second), "color: " + color + ";"};
}













