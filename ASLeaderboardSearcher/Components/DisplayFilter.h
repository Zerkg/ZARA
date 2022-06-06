#ifndef DISPLAYFILTER_H
#define DISPLAYFILTER_H

#include <QString>

class DisplayFilter
{
public:

    enum Type : signed char
    {
        STRING,
        REG_EX
    };
    enum Mode : signed char
    {
        EXCLUDE,
        INCLUDE
    };

    DisplayFilter() noexcept;
    DisplayFilter(const QString &filter, Type type, Mode mode = INCLUDE) noexcept;

    void setExpression(const QString &expression) noexcept;
    QString expression() const noexcept;

    void setType(Type type) noexcept;
    Type type() const noexcept;

    void setMode(Mode mode) noexcept;
    Mode mode() const noexcept;

    bool isValid(const QString& result) const;

    static QString modeToString(Mode mode);
    static QString typeToString(Type type);

private:

    QString m_Expression;

    Type m_Type;
    Mode m_Mode;

};

#endif // DISPLAYFILTER_H
