#ifndef ASFUNCTIONS_H
#define ASFUNCTIONS_H

#include "ASFunctions_global.h"
#include "ASAfterrideBonus.h"
#include "ASTypes.h"

#include <QString>

class QPixmap;

namespace as
{
ASFUNCTIONS_EXPORT QString fixName    (const QString &name         );
ASFUNCTIONS_EXPORT int     parseInt   (const QString &input, const QString &ignoreList = QString());
ASFUNCTIONS_EXPORT double  parseDouble(const QString &input        );
ASFUNCTIONS_EXPORT QString commaPrint (long value, char comma = ',');

constexpr const char *modeToString(Mode mode) noexcept
{
    switch (mode)
    {
    case CASUAL: return "Casual";
    case PRO   : return "Pro";
    case ELITE : return "Elite";

    default: return "Unknown";
    }
}
ASFUNCTIONS_EXPORT Mode stringToMode(const char *str ) noexcept;

constexpr std::pair<const char*, const char*> characterStatsNames(Character type) noexcept
{
    switch (type)
    {
    case as::DV      : return { "Crossover matches" , "Score Δ"              };
    case as::ERASER  : return { "Erases"            , "Average # erased"     };
    case as::POINTMAN: return { "Scooped blocks"    , "Scooped powerups"     };
    case as::PUSHER  : return { "Left pushes"       , "Right pushes"         };
    case as::VEGAS   : return { "Shuffles"          , "Avg. shuffle size"    };
    case as::MONO    : return { "Grey hit, n"       , "Colored collected, %" };

    default: return std::pair<const char*, const char*>();
    }
}
constexpr const char *characterToString(Character type, Mode mode) noexcept
{
    switch (type)
    {
    case as::MONO    : return (mode == ELITE ? "Mono Ninja"     : mode == PRO ? "Mono Pro"     : "Mono"    );
    case as::DV      : return (mode == ELITE ? "DVE"            : mode == PRO ? "DVP"          : "DV"      );
    case as::POINTMAN: return (mode == ELITE ? "Pointman Elite" : mode == PRO ? "Pointman Pro" : "Pointman");
    case as::ERASER  : return (mode == ELITE ? "Eraser Elite"   : "Eraser");
    case as::PUSHER  : return (mode == ELITE ? "Pusher Elite"   : "Pusher");
    case as::VEGAS   : return "Vegas";
    default: return "Unknown";
    }
}
ASFUNCTIONS_EXPORT QPixmap characterPixmap(Character type, Mode mode);

constexpr const char* colorToString(Color c) noexcept
{
    switch (c)
    {
    case PINK  : return "Pink"  ;
    case BLUE  : return "Blue"  ;
    case GREEN : return "Green" ;
    case YELLOW: return "Yellow";
    case RED   : return "Red"   ;
    case WHITE : return "White" ;
    default: return "Unknown";
    }
}
ASFUNCTIONS_EXPORT Color stringToColor(const char *str) noexcept;

constexpr const char* powerupToString(Powerup p) noexcept
{
    switch(p)
    {
    case as::STORM     : return "Storm"     ;
    case as::PAINT     : return "Paint"     ;
    case as::MULTIPLIER: return "Multiplier";
    case as::SORT      : return "Sort"      ;
    default            : return "Unknown"   ;
    }
}

constexpr ASAfterrideBonus bonusesPreset(Character c) noexcept
{
    ASAfterrideBonus p(as::CLEAN_FINISH, as::MATCH_21);
    switch (c)
    {
    case as::ERASER:
    case as::POINTMAN:
    case as::VEGAS:
        p.setBonus(SEEING_RED  );
        p.setBonus(BUTTER_NINJA);
        break;
    case as::MONO:
        p.setBonus(STEALTH);
        break;
    default: break;
    }

    return p;
}
}

#endif // ASFUNCTIONS_H
