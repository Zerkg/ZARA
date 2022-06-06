#ifndef ASAFTERRIDEBONUS_H
#define ASAFTERRIDEBONUS_H

#include <array>
#include <list>

namespace as
{
enum Bonus : unsigned char
{
    IRONMODE     = 1  ,

    CLEAN_FINISH = 2  ,

    STEALTH      = 4  ,

    MATCH_7      = 8  ,
    MATCH_11     = 16 ,
    MATCH_21     = 32 ,

    SEEING_RED   = 64 ,
    BUTTER_NINJA = 128
};
constexpr std::array bonuses_array{ IRONMODE, CLEAN_FINISH, STEALTH, MATCH_7, MATCH_11, MATCH_21, SEEING_RED, BUTTER_NINJA };
}

class __declspec(dllexport) ASAfterrideBonus
{
public:

    struct BonusInfo
    {      
        float multiplier;
        const char* name;
    };

    constexpr ASAfterrideBonus() noexcept = default;
    template<typename ..._bT>
    constexpr ASAfterrideBonus(_bT ...bonuses) noexcept { (m_Bonuses |= ... |= bonuses); }

    constexpr void setBonus(as::Bonus bonus, bool setEnable = true) noexcept { setEnable ? m_Bonuses |= bonus : m_Bonuses &= ~bonus; }

    constexpr bool isAchieved(as::Bonus bonus) const noexcept { return (m_Bonuses & bonus); }
    constexpr as::Bonus matchBonus() const noexcept;

    constexpr double bonusesBoost() const noexcept;

    constexpr void reset() noexcept { m_Bonuses = {}; };

    std::list<std::string> achievedList() const;
    std::list<as::Bonus  > vectorize   () const;

    static constexpr BonusInfo bonusInfo(as::Bonus bonus) noexcept;

private:

    char m_Bonuses{};

};



inline constexpr ASAfterrideBonus::BonusInfo ASAfterrideBonus::bonusInfo(as::Bonus bonus) noexcept
{
    switch (bonus)
    {
    case as::IRONMODE    : return { 1.00, "Ironmode"     };
    case as::CLEAN_FINISH: return { 1.25, "Clean Finish" };
    case as::STEALTH     : return { 1.30, "Stealth"      };
    case as::MATCH_7     : return { 1.07, "Match 7"      };
    case as::MATCH_11    : return { 1.11, "Match 11"     };
    case as::MATCH_21    : return { 1.21, "Match 21"     };
    case as::SEEING_RED  : return { 1.05, "Seeing Red"   };
    case as::BUTTER_NINJA: return { 1.05, "Butter Ninja" };
    default: return BonusInfo();
    }
}
inline constexpr double ASAfterrideBonus::bonusesBoost() const noexcept
{
    double boost{1};

    for (auto bonus : as::bonuses_array)
        if (m_Bonuses & bonus)
            boost += bonusInfo(bonus).multiplier - 1;

    return boost;
}
inline constexpr as::Bonus ASAfterrideBonus::matchBonus() const noexcept
{
    if      (m_Bonuses & as::MATCH_21) return as::MATCH_21;
    else if (m_Bonuses & as::MATCH_11) return as::MATCH_11;
    else if (m_Bonuses & as::MATCH_7 ) return as::MATCH_7 ;

    return as::Bonus(0);
}


#endif // ASAFTERRIDEBONUS_H

