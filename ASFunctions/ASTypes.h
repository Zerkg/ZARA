#ifndef ASTYPES_H
#define ASTYPES_H

#include "ASFunctions_global.h"
#include <array>

namespace as
{
enum Mode
{
    CASUAL,
    PRO   ,
    ELITE
};
enum Character
{
    DV      ,
    ERASER  ,
    POINTMAN,
    PUSHER  ,
    VEGAS   ,
    MONO    ,

    TOTAL_COUNT
};
enum Color
{
    PINK  ,
    BLUE  ,
    GREEN ,
    YELLOW,
    RED   ,
    WHITE
};

constexpr std::array<Color    , 6> colors_array    ({ PINK  , BLUE  , GREEN   , YELLOW, RED  , WHITE });
constexpr std::array<Mode     , 3> modes_array     ({ CASUAL, PRO   , ELITE                          });
constexpr std::array<Character, 6> characters_array({ DV    , ERASER, POINTMAN, PUSHER, VEGAS, MONO  });

}

#endif // ASTYPES_H
