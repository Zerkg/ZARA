#ifndef ASAFTERRIDEBONUS_H
#define ASAFTERRIDEBONUS_H

#include "ASFunctions_global.h"

#include <QString>

class ASFUNCTIONS_EXPORT ASAfterrideBonus
{
public:

    enum Bonus
    {
        IRONMODE     = 1 ,

        CLEAN_FINISH = 2 ,

        STEALTH      = 4 ,

        MATCH_7      = 8 ,
        MATCH_11     = 16,
        MATCH_21     = 32,

        SEEING_RED   = 64,
        BUTTER_NINJA = 128
    };
    struct BonusInfo
    {
        QString name;
        double multiplier;
    };

    ASAfterrideBonus() noexcept;
    ASAfterrideBonus(char bonuses) noexcept;

    void setBonus(Bonus bonus, bool setEnable = true) noexcept;

    char bonuses() const noexcept;

    bool isBonusAchieved(Bonus bonus) const noexcept;
    Bonus matchBonus() const noexcept;

    double bonusMultiplier(Bonus bonus) const noexcept;
    double applyBonuses(double score, bool isRawScore = true) const noexcept;

    void reset() noexcept;

    QStringList achievedBonusesList() const noexcept;

    static QString bonusToText(Bonus b) noexcept;

private:

    char m_Bonuses;
    static const QHash<Bonus, BonusInfo> M_BONUSES_INFO;

};

#endif // ASAFTERRIDEBONUS_H

