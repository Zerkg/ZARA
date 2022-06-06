#ifndef ASTYPES_H
#define ASTYPES_H

#include <array>

namespace as
{
enum Mode      : signed char
{
    CASUAL,
    PRO   ,
    ELITE
};
enum Character : signed char
{
    DV      ,
    ERASER  ,
    POINTMAN,
    PUSHER  ,
    VEGAS   ,
    MONO
};
enum Color     : signed char
{
    PINK  ,
    BLUE  ,
    GREEN ,
    YELLOW,
    RED   ,
    WHITE
};
enum Stat      : signed char
{
    NICKNAME             ,
    SCORE                ,
    DURATION             ,
    AFTERRIDE_BONUSES    ,

    CHARACTER            ,

    OVERFILLS            ,
    LONGEST_CHAIN        ,
    DROPPED_CHAIN        ,
    BEST_CLUSTER         ,
    AVG_CLUSTER_SIZE     ,
    AVG_CLUSTER_COLORS   ,
    CONS_BLOCKS_DODGED   ,
    CONS_BLOCKS_COLLECTED,
    SKILL_RATING         ,
    BLOCKS_STATS         ,
    POWERUPS_STATS
};
enum Powerup   : signed char
{
    STORM     ,
    PAINT     ,
    MULTIPLIER,
    SORT
};

inline constexpr std::array modes_array       { CASUAL, PRO   , ELITE                            };
inline constexpr std::array powerups_array    { STORM , PAINT , MULTIPLIER, SORT                 };
inline constexpr std::array characters_array  { DV    , ERASER, POINTMAN  , PUSHER, VEGAS, MONO  };
inline constexpr std::array colors_array      { PINK  , BLUE  , GREEN     , YELLOW, RED  , WHITE };
inline constexpr std::array modes_colors_array{ 0xd7f7b5, 0xffcfbd, 0xbdd8ea };

}

#endif // ASTYPES_H
