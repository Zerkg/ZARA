#ifndef STATSALIAS_H
#define STATSALIAS_H

#include "ASTypes.h"

#include <QString>
#include <vector>
#include <map>

struct PlayerStats;

struct ConvertedStat
{
    QString text;
    QString styleSheet{};
};

using ConvertedBonuses  = QVector<std::pair<QString, ConvertedStat>>;
using ConvertedPowerups = std::map<as::Powerup, ConvertedStat               >;
using ConvertedBlocks   = std::map<as::Color  , std::array<ConvertedStat, 4>>;

namespace sc
{
struct AdvancedStats
{
    uint rawScore;
    ushort colorBlocksCount[as::colors_array.size()]{};
    ushort traffic;
    ushort totalBlocksCount;
};

AdvancedStats calcAdvanced(const PlayerStats &s);

enum DisplayMode : unsigned char
{
    DEFAULT,
    DIFF,
    MINMAX,
    ANALYZE
};
enum Stat
{
    SCORE                ,
    RAW_SCORE            ,
    DURATION             ,
    CHARACTER_STAT_1     ,
    CHARACTER_STAT_2     ,
    OVERFILLS            ,
    LONGEST_CHAIN        ,
    DROPPED_CHAIN        ,
    BEST_CLUSTER         ,
    AVG_CLUSTER_SIZE     ,
    AVG_CLUSTER_COLORS   ,
    CONS_BLOCKS_DODGED   ,
    CONS_BLOCKS_COLLECTED,
    SKILL_RATING         ,
    BLOCKS_COUNT         ,
    TRAFFIC              ,
};
}

#endif // STATSALIAS_H
