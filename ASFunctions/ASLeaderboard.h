#ifndef ASLEADERBOARD_H
#define ASLEADERBOARD_H

#include "ASFunctions_global.h"

#include "ASTypes.h"
#include "ASAfterrideBonus.h"
#include <QTime>
#include <QStringList>
#include <vector>
#include <memory>

class ASCharacter;

struct ASFUNCTIONS_EXPORT PlayerStats
{
    uint score;
    std::shared_ptr<ASCharacter> character;

    int    bestCluster   ;
    float  avgClusterSize;
    float  avgColorsCount;

    QString rID;

    QString nickname;
    QTime trackDuration;

    short  longestChainTime;
    short  droppedChains   ;

    short maxConsecutiveBlocksCollected;
    short maxConsecutiveBlocksDodged   ;
    std::optional<int>   skillRaiting  ;

    short blocksStats[3][6];

    uchar overfillsCount ;

    uchar paintsUsed     ;
    uchar shtormsUsed    ;
    uchar multipliersUsed;
    uchar sortsUsed      ;

    ASAfterrideBonus bonuses;

    char validationFlags[2]{};
};

class ASFUNCTIONS_EXPORT ASLeaderboard
{
public:

    enum Validation
    {
        BASE_STATS,
        EXT_STATS
    };

    ASLeaderboard(as::Mode mode = as::CASUAL) noexcept;

    void reset() noexcept;

    bool isStatsValid(int pos) const noexcept;
    void setValid(int pos, Validation validation) noexcept;

    PlayerStats statsAt(int pos) const noexcept;

    void addStats(const PlayerStats& pStats) noexcept;
    void emplaceStat(PlayerStats &&pStats) noexcept;

    int size() const noexcept;
    void resize(size_t newSize) noexcept;

    QStringList players() const noexcept;

    operator as::Mode() const noexcept;

    bool operator==(as::Mode mode) const noexcept;

    PlayerStats& operator[](int pos) noexcept;

private:

    as::Mode m_Mode;
    std::vector<PlayerStats> m_Leaderboard;

};


#endif // ASLEADERBOARD_H
