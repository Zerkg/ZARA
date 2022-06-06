#ifndef PLAYERSTATS_H
#define PLAYERSTATS_H

#include "ASTypes.h"
#include "ASAfterrideBonus.h"
#include <QTime>

struct PlayerStats
{
    enum StatsStatus : unsigned char
    {
        INVALID   ,
        PROCESSING,
        VALID     ,
    };
    enum StatsType   : unsigned char
    {
        BASE,
        EXT
    };

    struct CharacterData
    {
        as::Mode      mode    ;
        as::Character type    ;
        int           stats[2];
    };

    CharacterData characterData;

    uint skillRating;
    uint score;

    uint  bestCluster   ;
    float avgClusterSize;
    float avgColorsCount;

    QString rID;

    QString nickname     ;
    QTime   trackDuration;

    ushort longestChainTime;
    ushort droppedChains   ;

    ushort maxConsBlocksCollected;
    ushort maxConsBlocksDodged   ;

    ushort blocksStats[as::colors_array.size()][3]{};

    uchar powerups[as::powerups_array.size()]{};
    uchar overfillsCount;

    ASAfterrideBonus bonuses;

    StatsStatus validationFlags[2]{};
};
#endif // PLAYERSTATS_H
