#include "ASAfterrideBonus.h"

using namespace as;

using std::list  ;
using std::string;

list<string> ASAfterrideBonus::achievedList() const
{
    list<string> achieved;
    for (auto bonus : bonuses_array)
        if (m_Bonuses & bonus)
            achieved.push_back(bonusInfo(bonus).name);

    return achieved;
}

list<Bonus> ASAfterrideBonus::vectorize() const
{
    list<Bonus> vectorized;
    for (auto bonus : bonuses_array)
        if (m_Bonuses & bonus)
            vectorized.push_back(bonus);

    return vectorized;
}



