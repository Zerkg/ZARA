#ifndef ASPLAYERSTATSMODEL_H
#define ASPLAYERSTATSMODEL_H

#include "ASTypes.h"

#include <QObject>
#include <QTime>
#include <memory>

class QAbstractItemModel;
class ASCharacter;

struct PlayerStats;

class ASAbstractPlayerStatsModel : public QObject
{
Q_OBJECT
public:

    enum StatsDataIndex
    {
        SCORE           ,
        BONUSES_LIST    ,
        BONUS_IRONMODE  ,
        BONUS_CF        ,
        BONUS_MATCH     ,
        BONUS_STEALTH   ,
        BONUS_BN        ,
        BONUS_SR        ,
        CHARACTER_TYPE  ,
        CHARACTER_DATA_1,
        CHARACTER_DATA_2,

        OVERFILLS         = 12,
        LONGEST_CHAIN     ,
        DROPPED_CHAIN     ,
        BEST_CLUSTER      ,
        AVG_CLUSTER_SIZE  ,
        AVG_CLUSTER_COLORS,
        MAX_CONS_DODGED   ,
        MAX_CONS_COLLECTED,
        SKILL_RAITING     ,

        SHTORMS_USED    = 22,
        PAINTS_USED     ,
        MULTIPLIERS_USED,
        SORTS_USED      ,

        TOTAL_COUNT
    };
    struct AdditionalData
    {
        float traffic;
        int blocksCount;
        QTime duration;
        std::shared_ptr<ASCharacter> character;
    };

    explicit ASAbstractPlayerStatsModel(QObject *parent = nullptr) noexcept;
    virtual ~ASAbstractPlayerStatsModel();

    QAbstractItemModel* scoreStatsModel ();
    QAbstractItemModel* blocksStatsModel();

    virtual AdditionalData additionalData() const;

    virtual void setModelSource(const PlayerStats &pStats) = 0;

protected:

    QAbstractItemModel *m_ScoreStats, *m_BlocksStats;
    AdditionalData m_AdditionalData;

signals:

    void dataChanged();

};

class ASPlayerStatsModel : public ASAbstractPlayerStatsModel
{
    Q_OBJECT
public:

    explicit ASPlayerStatsModel(QObject *parent = nullptr);
    explicit ASPlayerStatsModel(const PlayerStats &pStats, QObject *parent = nullptr);

    void setModelSource(const PlayerStats &pStats) override;

private:

    void init();

    void resetBaseStats  ();
    void resetExtStats   ();
    void resetBlocksStats();

};

#endif // ASPLAYERSTATSMODEL_H
