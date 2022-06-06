#include "DisplayFilter.h"

#include <QRegExp>

DisplayFilter::DisplayFilter() noexcept : m_Type(STRING), m_Mode(INCLUDE){}
DisplayFilter::DisplayFilter(const QString &filter, Type type, Mode mode) noexcept
{
    setExpression(filter);
    setType(type);
    setMode(mode);
}

void DisplayFilter::setExpression(const QString &filter) noexcept
{
    m_Expression = filter;
}
QString DisplayFilter::expression() const noexcept
{
    return m_Expression;
}

void DisplayFilter::setType(Type type) noexcept
{
    m_Type = type;
}
DisplayFilter::Type DisplayFilter::type() const noexcept
{
    return m_Type;
}

void DisplayFilter::setMode(Mode mode) noexcept
{
    m_Mode = mode;
}
DisplayFilter::Mode DisplayFilter::mode() const noexcept
{
    return m_Mode;
}

bool DisplayFilter::isValid(const QString &result) const
{
    bool validation;

    switch (m_Type)
    {
    case STRING: validation = result.contains(m_Expression, Qt::CaseInsensitive); break;
    case REG_EX: validation = result.contains(QRegExp(m_Expression)            ); break;

    default: return false;
    }

    return (validation == static_cast<bool>(m_Mode));
}

QString DisplayFilter::modeToString(Mode mode)
{
    switch (mode)
    {
    case EXCLUDE : return "Exclude";
    case INCLUDE : return "Include";

    default: return QString();
    }
}

QString DisplayFilter::typeToString(Type type)
{
    switch (type)
    {
    case REG_EX : return "RegEx" ;
    case STRING : return "String";

    default: return QString();
    }
}
