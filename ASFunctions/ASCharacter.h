#ifndef ASCHARACTER_H
#define ASCHARACTER_H

#include "ASFunctions_global.h"
#include "ASTypes.h"

#include <QMetaType>

class ASFUNCTIONS_EXPORT ASCharacter
{
public:

    ASCharacter(as::Mode mode, as::Character character) noexcept;
    virtual ~ASCharacter() noexcept = default;

    virtual QPair<QString, QString> getAbilityStatsName() const noexcept = 0;

    void setAbilityStat (int index, int value ) noexcept;
    void setAbilityStats(int first, int second) noexcept;

    int    getAbilityStat  (int index) const noexcept;
    std::array<int, 2> getAbilityStats () const noexcept;

    as::Character getType() const noexcept;
    as::Mode      getMode() const noexcept;

    ASCharacter *clone() const noexcept;

protected:

    as::Mode m_Mode;
    as::Character m_Type;
    std::array<int, 2> m_AbilityStats;

};
Q_DECLARE_METATYPE(ASCharacter*)



class ASFUNCTIONS_EXPORT Mono         : public ASCharacter
{
public:

    Mono(as::Mode mode = as::CASUAL) noexcept;
    virtual QPair<QString, QString> getAbilityStatsName() const noexcept override;

};
class ASFUNCTIONS_EXPORT Pointman     : public ASCharacter
{
public:

    Pointman(as::Mode mode = as::CASUAL) noexcept;
    virtual QPair<QString, QString> getAbilityStatsName() const noexcept override;

};
class ASFUNCTIONS_EXPORT DoubleVision : public ASCharacter
{
public:

    DoubleVision(as::Mode mode = as::CASUAL) noexcept;
    virtual QPair<QString, QString> getAbilityStatsName() const noexcept override;

};
class ASFUNCTIONS_EXPORT Eraser       : public ASCharacter
{
public:

    Eraser(as::Mode mode = as::PRO) noexcept;
    virtual QPair<QString, QString> getAbilityStatsName() const noexcept override;

};
class ASFUNCTIONS_EXPORT Pusher       : public ASCharacter
{
public:

    Pusher(as::Mode mode = as::PRO) noexcept;
    virtual QPair<QString, QString> getAbilityStatsName() const noexcept override;

};
class ASFUNCTIONS_EXPORT Vegas        : public ASCharacter
{
public:

    Vegas() noexcept;
    virtual QPair<QString, QString> getAbilityStatsName() const noexcept override;

};

#endif // ASCHARACTER_H
