#include "ASCharacter.h"
#include "ASFunctions.h"

using namespace as;

ASCharacter::ASCharacter(as::Mode mode, as::Character character) noexcept
    : m_Mode(mode), m_Type(character) {}

void ASCharacter::setAbilityStat(int index, int value) noexcept
{
    m_AbilityStats[index] = value;
}
void ASCharacter::setAbilityStats(int first, int second) noexcept
{
    m_AbilityStats[0] = first ;
    m_AbilityStats[1] = second;
}

int ASCharacter::getAbilityStat(int index) const noexcept
{
    return m_AbilityStats.at(index);
}
std::array<int, 2> ASCharacter::getAbilityStats() const noexcept
{
    return m_AbilityStats;
}

Character ASCharacter::getType() const noexcept
{
    return m_Type;
}
Mode ASCharacter::getMode() const noexcept
{
    return m_Mode;
}

ASCharacter *ASCharacter::clone() const noexcept
{
    ASCharacter *clone(createCharacter(m_Type, m_Mode));
    clone->m_AbilityStats = m_AbilityStats;

    return clone;
}


DoubleVision::DoubleVision(as::Mode mode) noexcept : ASCharacter(mode, DV      ) {}
Eraser      ::Eraser      (as::Mode mode) noexcept : ASCharacter(mode, ERASER  ) {}
Pointman    ::Pointman    (as::Mode mode) noexcept : ASCharacter(mode, POINTMAN) {}
Pusher      ::Pusher      (as::Mode mode) noexcept : ASCharacter(mode, PUSHER  ) {}
Vegas       ::Vegas       (             ) noexcept : ASCharacter(PRO , VEGAS   ) {}
Mono        ::Mono        (as::Mode mode) noexcept : ASCharacter(mode, MONO    ) {}

QPair<QString, QString> DoubleVision::getAbilityStatsName() const noexcept
{
    return { "% of matches shared", "Score Δ" };
}
QPair<QString, QString> Eraser/*::*/::getAbilityStatsName() const noexcept
{
    return { "Erases", "Average # erased" };
}
QPair<QString, QString> Pointman/**/::getAbilityStatsName() const noexcept
{
    return { "Scooped blocks", "Scooped powerups" };
}
QPair<QString, QString> Pusher/*::*/::getAbilityStatsName() const noexcept
{
    return { "Left pushes", "Right pushes" };
}
QPair<QString, QString> Vegas/*:::*/::getAbilityStatsName() const noexcept
{
    return { "Shuffles", "Average shuffle size" };
}
QPair<QString, QString> Mono/*::::*/::getAbilityStatsName() const noexcept
{
    return { "Grey blocks hit", "Colored blocks collected, %" };
}
