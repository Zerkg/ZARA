#include "ASPlayerStatsModel.h"

#include "TableModel.h"
#include "ASFunctions.h"
#include "ASCharacter.h"
#include "ASPhpQuery.h"

#include <QLocale>


ASAbstractPlayerStatsModel::ASAbstractPlayerStatsModel(QObject *parent) noexcept
    : QObject(parent)
    , m_ScoreStats(nullptr)
    , m_BlocksStats(nullptr)
{
    m_AdditionalData.blocksCount = 0;
}
ASAbstractPlayerStatsModel::~ASAbstractPlayerStatsModel()
{
}


QAbstractItemModel *ASAbstractPlayerStatsModel::scoreStatsModel()
{
    return m_ScoreStats;
}
QAbstractItemModel *ASAbstractPlayerStatsModel::blocksStatsModel()
{
    return m_BlocksStats;
}


ASAbstractPlayerStatsModel::AdditionalData ASAbstractPlayerStatsModel::additionalData() const
{
    return m_AdditionalData;
}





ASPlayerStatsModel::ASPlayerStatsModel(QObject *parent)
    : ASAbstractPlayerStatsModel(parent)
{
    init();
}
ASPlayerStatsModel::ASPlayerStatsModel(const PlayerStats &pStats, QObject *parent)
    : ASAbstractPlayerStatsModel(parent)
{
    init();
    ASPlayerStatsModel::setModelSource(pStats);
}


void ASPlayerStatsModel::setModelSource(const PlayerStats &pStats)
{
    m_AdditionalData.duration  = pStats.trackDuration;
    m_AdditionalData.character = pStats.character;

    m_ScoreStats->setData(m_ScoreStats->index(SCORE         , 1), pStats.score                                                  , Qt::EditRole   );
    m_ScoreStats->setData(m_ScoreStats->index(SCORE         , 1), as::convertNumber(pStats.score)                               , Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(BONUSES_LIST  , 1), pStats.bonuses.bonuses()                                      , Qt::EditRole   );
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_IRONMODE, 1), pStats.bonuses.isBonusAchieved(ASAfterrideBonus::IRONMODE    ), Qt::EditRole   );
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_CF      , 1), pStats.bonuses.isBonusAchieved(ASAfterrideBonus::CLEAN_FINISH), Qt::EditRole   );
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_MATCH   , 1), pStats.bonuses.matchBonus()                                   , Qt::EditRole   );
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_MATCH   , 0), ASAfterrideBonus::bonusToText(pStats.bonuses.matchBonus())    , Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_STEALTH , 1), pStats.bonuses.isBonusAchieved(ASAfterrideBonus::STEALTH     ), Qt::EditRole   );
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_BN      , 1), pStats.bonuses.isBonusAchieved(ASAfterrideBonus::BUTTER_NINJA), Qt::EditRole   );
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_SR      , 1), pStats.bonuses.isBonusAchieved(ASAfterrideBonus::SEEING_RED  ), Qt::EditRole   );
    m_ScoreStats->setData(m_ScoreStats->index(CHARACTER_TYPE, 1), pStats.character->getType()                                   , Qt::EditRole   );

    if (pStats.skillRaiting != std::nullopt)
    {
        auto statsName  (pStats.character->getAbilityStatsName());
        auto statsValues(pStats.character->getAbilityStats    ());

        m_ScoreStats->setData(m_ScoreStats->index(CHARACTER_DATA_1  , 0), statsName.first  + ':'                   , Qt::DisplayRole);
        m_ScoreStats->setData(m_ScoreStats->index(CHARACTER_DATA_1  , 1), statsValues[0]                           , Qt::DisplayRole);
        m_ScoreStats->setData(m_ScoreStats->index(CHARACTER_DATA_2  , 0), statsName.second + ':'                   , Qt::DisplayRole);
        m_ScoreStats->setData(m_ScoreStats->index(CHARACTER_DATA_2  , 1), statsValues[1]                           , Qt::DisplayRole);

        m_ScoreStats->setData(m_ScoreStats->index(OVERFILLS         , 1), pStats.overfillsCount                    , Qt::DisplayRole);
        m_ScoreStats->setData(m_ScoreStats->index(LONGEST_CHAIN     , 1), pStats.longestChainTime                  , Qt::DisplayRole);
        m_ScoreStats->setData(m_ScoreStats->index(DROPPED_CHAIN     , 1), pStats.droppedChains                     , Qt::DisplayRole);
        m_ScoreStats->setData(m_ScoreStats->index(BEST_CLUSTER      , 1), pStats.bestCluster                       , Qt::DisplayRole);

        QLocale l(QLocale::English);
        m_ScoreStats->setData(m_ScoreStats->index(AVG_CLUSTER_SIZE  , 1), l.toString(pStats.avgClusterSize, 'f', 1), Qt::DisplayRole);
        m_ScoreStats->setData(m_ScoreStats->index(AVG_CLUSTER_COLORS, 1), l.toString(pStats.avgColorsCount, 'f', 1), Qt::DisplayRole);

        m_ScoreStats->setData(m_ScoreStats->index(MAX_CONS_DODGED   , 1), pStats.maxConsecutiveBlocksDodged        , Qt::DisplayRole);
        m_ScoreStats->setData(m_ScoreStats->index(MAX_CONS_COLLECTED, 1), pStats.maxConsecutiveBlocksCollected     , Qt::DisplayRole);
        m_ScoreStats->setData(m_ScoreStats->index(SKILL_RAITING     , 1), pStats.skillRaiting.value()              , Qt::DisplayRole);

        if (pStats.character->getType() != as::MONO)
        {
            m_ScoreStats->setData(m_ScoreStats->index(SHTORMS_USED    , 1), pStats.shtormsUsed    , Qt::DisplayRole);
            m_ScoreStats->setData(m_ScoreStats->index(PAINTS_USED     , 1), pStats.paintsUsed     , Qt::DisplayRole);
            m_ScoreStats->setData(m_ScoreStats->index(MULTIPLIERS_USED, 1), pStats.multipliersUsed, Qt::DisplayRole);
            m_ScoreStats->setData(m_ScoreStats->index(SORTS_USED      , 1), pStats.sortsUsed      , Qt::DisplayRole);

            for (size_t row{}; row < 3; ++row)
                for (size_t column{}; column < as::colors_array.size(); ++column)
                    for (auto role : { Qt::EditRole, Qt::DisplayRole })
                        m_BlocksStats->setData(m_BlocksStats->index(row, column), pStats.blocksStats[row][column], role);


            short blocksCount[as::colors_array.size()]{};

            for (size_t column{}; column < as::colors_array.size(); ++column)
            {
                if (pStats.blocksStats[0][column] != 0)
                    blocksCount[column] = qRound(pStats.blocksStats[0][column] * 100.0 / pStats.blocksStats[2][column]);

                m_BlocksStats->setData(m_BlocksStats->index(3, column), blocksCount[column], Qt::DisplayRole);
                m_BlocksStats->setData(m_BlocksStats->index(3, column), blocksCount[column], Qt::EditRole   );
            }

            m_AdditionalData.blocksCount = std::accumulate(&blocksCount[0], &blocksCount[6], 0);
            m_AdditionalData.traffic = m_AdditionalData.blocksCount / (pStats.trackDuration.minute() + pStats.trackDuration.second() / 60.0);
        }
        else
        {
            m_ScoreStats->setData(m_ScoreStats->index(SHTORMS_USED    , 1), 0, Qt::DisplayRole);
            m_ScoreStats->setData(m_ScoreStats->index(PAINTS_USED     , 1), 0, Qt::DisplayRole);
            m_ScoreStats->setData(m_ScoreStats->index(MULTIPLIERS_USED, 1), 0, Qt::DisplayRole);
            m_ScoreStats->setData(m_ScoreStats->index(SORTS_USED      , 1), 0, Qt::DisplayRole);

            resetBlocksStats();
        }
    }
    else
    {
        resetExtStats();
        resetBlocksStats();

        m_ScoreStats->setData(m_ScoreStats->index(CHARACTER_DATA_1, 0), "The score is too old," , Qt::DisplayRole);
        m_ScoreStats->setData(m_ScoreStats->index(CHARACTER_DATA_2, 0), "stats are unavailable.", Qt::DisplayRole);
    }

    emit dataChanged();
}

void ASPlayerStatsModel::init()
{
    m_ScoreStats = new TableModel(2, TOTAL_COUNT, this);

    m_ScoreStats->setData(m_ScoreStats->index(SCORE         , 0), QStringLiteral("Score: ")       , Qt::DisplayRole      );

    m_ScoreStats->setData(m_ScoreStats->index(BONUS_IRONMODE, 0), ASAfterrideBonus::bonusToText(ASAfterrideBonus::IRONMODE    ), Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_CF      , 0), ASAfterrideBonus::bonusToText(ASAfterrideBonus::CLEAN_FINISH), Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_MATCH   , 0), QStringLiteral("Match bonus")                                , Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_STEALTH , 0), ASAfterrideBonus::bonusToText(ASAfterrideBonus::STEALTH     ), Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_BN      , 0), ASAfterrideBonus::bonusToText(ASAfterrideBonus::BUTTER_NINJA), Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_SR      , 0), ASAfterrideBonus::bonusToText(ASAfterrideBonus::SEEING_RED  ), Qt::DisplayRole);

    m_ScoreStats->setData(m_ScoreStats->index(OVERFILLS         , 0), QStringLiteral("Overfills:"                 ), Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(LONGEST_CHAIN     , 0), QStringLiteral("Longest chain, seconds:"    ), Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(DROPPED_CHAIN     , 0), QStringLiteral("Dropped chains, count:"     ), Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(BEST_CLUSTER      , 0), QStringLiteral("Best cluster, points:"      ), Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(AVG_CLUSTER_SIZE  , 0), QStringLiteral("Avg. cluster size:"         ), Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(AVG_CLUSTER_COLORS, 0), QStringLiteral("Avg. cluster color count:"  ), Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(MAX_CONS_DODGED   , 0), QStringLiteral("Max cons. blocks dodged:"   ), Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(MAX_CONS_COLLECTED, 0), QStringLiteral("Max cons. blocks collected:"), Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(SKILL_RAITING     , 0), QStringLiteral("Skill rating:"              ), Qt::DisplayRole);

    m_ScoreStats->setData(m_ScoreStats->index(SHTORMS_USED    , 0), QStringLiteral("Shtorms used:"              ), Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(PAINTS_USED     , 0), QStringLiteral("Paints used:"               ), Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(MULTIPLIERS_USED, 0), QStringLiteral("Multipliers used:"          ), Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(SORTS_USED      , 0), QStringLiteral("Sorts used:"                ), Qt::DisplayRole);


    m_BlocksStats = new TableModel(as::colors_array.size(), 4, this);

    for (size_t i{}; i < as::colors_array.size(); ++i)
    {
        m_BlocksStats->setHeaderData(i, Qt::Horizontal, as::colorToString(as::colors_array[i]), Qt::DisplayRole);
        for (int j{}; j < 4; ++j)
            m_BlocksStats->setData(m_BlocksStats->index(j, i), Qt::AlignCenter, Qt::TextAlignmentRole);
    }

    m_BlocksStats->setHeaderData(0, Qt::Vertical, QStringLiteral("Collected, n"), Qt::DisplayRole);
    m_BlocksStats->setHeaderData(1, Qt::Vertical, QStringLiteral("Derived, %"  ), Qt::DisplayRole);
    m_BlocksStats->setHeaderData(2, Qt::Vertical, QStringLiteral("Collected, %"), Qt::DisplayRole);
    m_BlocksStats->setHeaderData(3, Qt::Vertical, QStringLiteral("Total count" ), Qt::DisplayRole);

    resetBaseStats  ();
    resetExtStats   ();
    resetBlocksStats();
}

void ASPlayerStatsModel::resetExtStats()
{
    m_ScoreStats->setData(m_ScoreStats->index(CHARACTER_DATA_1, 0), "Unknown character", Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(CHARACTER_DATA_2, 0), "Unknown character", Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(CHARACTER_DATA_1, 1), "N/A", Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(CHARACTER_DATA_2, 1), "N/A", Qt::DisplayRole);

    m_ScoreStats->setData(m_ScoreStats->index(OVERFILLS         , 1), "N/A", Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(LONGEST_CHAIN     , 1), "N/A", Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(DROPPED_CHAIN     , 1), "N/A", Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(BEST_CLUSTER      , 1), "N/A", Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(AVG_CLUSTER_SIZE  , 1), "N/A", Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(AVG_CLUSTER_COLORS, 1), "N/A", Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(MAX_CONS_DODGED   , 1), "N/A", Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(MAX_CONS_COLLECTED, 1), "N/A", Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(SKILL_RAITING     , 1), "N/A", Qt::DisplayRole);

    m_ScoreStats->setData(m_ScoreStats->index(SHTORMS_USED    , 1), 0, Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(PAINTS_USED     , 1), 0, Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(MULTIPLIERS_USED, 1), 0, Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(SORTS_USED      , 1), 0, Qt::DisplayRole);
}
void ASPlayerStatsModel::resetBlocksStats()
{
    for (size_t row{}; row < 4; ++row)
        for (size_t column{}; column < as::colors_array.size(); ++column)
        {
            m_BlocksStats->setData(m_BlocksStats->index(row, column), "N/A", Qt::DisplayRole);
            m_BlocksStats->setData(m_BlocksStats->index(row, column), -1   , Qt::EditRole   );
        }

    m_AdditionalData.blocksCount = 0;
    m_AdditionalData.traffic = 0;
}
void ASPlayerStatsModel::resetBaseStats()
{
    m_ScoreStats->setData(m_ScoreStats->index(SCORE         , 1), "N/A", Qt::DisplayRole);
    m_ScoreStats->setData(m_ScoreStats->index(BONUSES_LIST  , 1),     0, Qt::EditRole   );
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_IRONMODE, 1), false, Qt::EditRole   );
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_CF      , 1), false, Qt::EditRole   );
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_MATCH   , 1), false, Qt::EditRole   );
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_STEALTH , 1), false, Qt::EditRole   );
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_BN      , 1), false, Qt::EditRole   );
    m_ScoreStats->setData(m_ScoreStats->index(BONUS_SR      , 1), false, Qt::EditRole   );
    m_ScoreStats->setData(m_ScoreStats->index(CHARACTER_TYPE, 1),    -1, Qt::EditRole   );
}











