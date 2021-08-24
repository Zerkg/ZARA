#include "ASLeaderboard.h"

//=========================================================:: Leaderboard ::=========================================================

ASLeaderboard::ASLeaderboard(as::Mode mode) noexcept
    : m_Mode(mode){ m_Leaderboard.reserve(10); }


void ASLeaderboard::reset() noexcept
{
    m_Leaderboard.clear();
}

bool ASLeaderboard::isStatsValid(int pos) const noexcept
{
    return (m_Leaderboard[pos].validationFlags[0] && m_Leaderboard[pos].validationFlags[1]);
}

void ASLeaderboard::setValid(int pos, Validation validation) noexcept
{
    m_Leaderboard[pos].validationFlags[validation] = true;
}

PlayerStats ASLeaderboard::statsAt(int pos) const noexcept
{
    const PlayerStats &source(m_Leaderboard[pos]);
    PlayerStats copy(source);

    for (int i{}; i < 3; ++i)
        for (int j{}; j < 6; ++j)
            copy.blocksStats[i][j] = source.blocksStats[i][j];

    return copy;
}

void ASLeaderboard::addStats(const PlayerStats &pStats) noexcept
{
    PlayerStats temp(pStats);

    for (int i{}; i < 3; ++i)
        for (int j{}; j < 6; ++j)
            temp.blocksStats[i][j] = pStats.blocksStats[i][j];

    m_Leaderboard.emplace_back(std::move(temp));
}

void ASLeaderboard::emplaceStat(PlayerStats &&pStats) noexcept
{
    m_Leaderboard.emplace_back(pStats);
}

int ASLeaderboard::size() const noexcept
{
    return m_Leaderboard.size();
}

QStringList ASLeaderboard::players() const noexcept
{
    QStringList playersList;

    for (const auto &stats : m_Leaderboard)
        playersList << stats.nickname;

    return playersList;
}

ASLeaderboard::operator as::Mode() const noexcept
{
    return m_Mode;
}

bool ASLeaderboard::operator==(as::Mode mode) const noexcept
{
    return m_Mode == mode;
}

PlayerStats &ASLeaderboard::operator[](int pos) noexcept
{
    return m_Leaderboard[pos];
}
