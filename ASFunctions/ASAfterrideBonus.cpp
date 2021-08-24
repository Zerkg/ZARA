#include "ASAfterrideBonus.h"

#include <QHash>

ASAfterrideBonus::ASAfterrideBonus() noexcept : m_Bonuses(0){}
ASAfterrideBonus::ASAfterrideBonus(char bonuses) noexcept: m_Bonuses(bonuses){}

bool ASAfterrideBonus::isBonusAchieved(Bonus bonus) const noexcept
{
    return (m_Bonuses & bonus);
}

double ASAfterrideBonus::bonusMultiplier(Bonus bonus) const noexcept
{
    return M_BONUSES_INFO.value(bonus).multiplier;
}

double ASAfterrideBonus::applyBonuses(double score, bool isRawScore) const noexcept
{
    double multiplier(1);

    if      (m_Bonuses & CLEAN_FINISH) multiplier += M_BONUSES_INFO.value(CLEAN_FINISH).multiplier - 1;

    if      (m_Bonuses & MATCH_21    ) multiplier += M_BONUSES_INFO.value(MATCH_21    ).multiplier - 1;
    else if (m_Bonuses & MATCH_11    ) multiplier += M_BONUSES_INFO.value(MATCH_11    ).multiplier - 1;
    else if (m_Bonuses & MATCH_7     ) multiplier += M_BONUSES_INFO.value(MATCH_7     ).multiplier - 1;

    if      (m_Bonuses & STEALTH     ) multiplier += M_BONUSES_INFO.value(STEALTH     ).multiplier - 1;
    else
    {
        if  (m_Bonuses & SEEING_RED  ) multiplier += M_BONUSES_INFO.value(SEEING_RED  ).multiplier - 1;
        if  (m_Bonuses & BUTTER_NINJA) multiplier += M_BONUSES_INFO.value(BUTTER_NINJA).multiplier - 1;
    }

    if (isRawScore) return score * multiplier;
    else            return score / multiplier;
}

ASAfterrideBonus::Bonus ASAfterrideBonus::matchBonus() const noexcept
{
    if      (m_Bonuses & MATCH_21    ) return MATCH_21;
    else if (m_Bonuses & MATCH_11    ) return MATCH_11;
    else if (m_Bonuses & MATCH_7     ) return MATCH_7 ;

    return Bonus(0);
}

void ASAfterrideBonus::reset() noexcept
{
    m_Bonuses = 0;
}

void ASAfterrideBonus::setBonus(Bonus bonus, bool setEnable) noexcept
{
    setEnable ? m_Bonuses |= bonus : m_Bonuses &= ~bonus;
}

char ASAfterrideBonus::bonuses() const noexcept
{
    return m_Bonuses;
}

QStringList ASAfterrideBonus::achievedBonusesList() const noexcept
{
    QStringList bonuses;

    if      (m_Bonuses & IRONMODE    ) bonuses << M_BONUSES_INFO.value(IRONMODE    ).name;

    if      (m_Bonuses & CLEAN_FINISH) bonuses << M_BONUSES_INFO.value(CLEAN_FINISH).name;

    if      (m_Bonuses & MATCH_21    ) bonuses << M_BONUSES_INFO.value(MATCH_21    ).name;
    else if (m_Bonuses & MATCH_11    ) bonuses << M_BONUSES_INFO.value(MATCH_11    ).name;
    else if (m_Bonuses & MATCH_7     ) bonuses << M_BONUSES_INFO.value(MATCH_7     ).name;

    if      (m_Bonuses & STEALTH     ) bonuses << M_BONUSES_INFO.value(STEALTH     ).name;
    else
    {
        if  (m_Bonuses & SEEING_RED  ) bonuses << M_BONUSES_INFO.value(SEEING_RED  ).name;
        if  (m_Bonuses & BUTTER_NINJA) bonuses << M_BONUSES_INFO.value(BUTTER_NINJA).name;
    }

    return bonuses;
}

QString ASAfterrideBonus::bonusToText(Bonus b) noexcept
{
    return M_BONUSES_INFO.value(b).name;
}


const QHash<ASAfterrideBonus::Bonus, ASAfterrideBonus::BonusInfo> ASAfterrideBonus::M_BONUSES_INFO
       ({std::pair(IRONMODE    , BonusInfo({ "Ironmode"    , 1.00 }) ),
         std::pair(CLEAN_FINISH, BonusInfo({ "Clean Finish", 1.25 }) ),
         std::pair(MATCH_21    , BonusInfo({ "Match 21"    , 1.21 }) ),
         std::pair(MATCH_11    , BonusInfo({ "Match 11"    , 1.11 }) ),
         std::pair(MATCH_7     , BonusInfo({ "Match 7"     , 1.70 }) ),
         std::pair(STEALTH     , BonusInfo({ "Stealth"     , 1.30 }) ),
         std::pair(SEEING_RED  , BonusInfo({ "Seeing Red"  , 1.05 }) ),
         std::pair(BUTTER_NINJA, BonusInfo({ "Butter Ninja", 1.05 }) )});
