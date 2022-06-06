#include "StatsAlias.h"

#include <PlayerStats.h>

sc::AdvancedStats sc::calcAdvanced(const PlayerStats &stats)
{
    sc::AdvancedStats advStats;

    if (stats.validationFlags[PlayerStats::EXT] == PlayerStats::VALID && stats.characterData.type != as::MONO)
    {
        for (const auto color : as::colors_array)
            if (stats.blocksStats[color][0] != 0)
                advStats.colorBlocksCount[color] = qRound(stats.blocksStats[color][0] * 100.0 / stats.blocksStats[color][2]);

        advStats.totalBlocksCount = std::accumulate(std::cbegin(advStats.colorBlocksCount), std::cend(advStats.colorBlocksCount), 0);
        advStats.traffic = advStats.totalBlocksCount / (stats.trackDuration.minute() + stats.trackDuration.second() / 60.0);

    }
    advStats.rawScore = qRound(stats.score / stats.bonuses.bonusesBoost());

    return advStats;
}
