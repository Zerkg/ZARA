#include "ASFunctions.h"
#include "ASAfterrideBonus.h"

#include <QPixmap>

namespace as
{
QString fixName    (const QString &name   )
{
    if (name.isEmpty())
        return QString();

    QString fixedName(name[0].toUpper());
    for(int i{1}, size(name.size()); i < size; ++i)
    {
        if      (name[i - 1] == ' ') fixedName += name[i].toUpper();
        else if (name[i - 1] == '(') fixedName += name[i].toUpper();

        else fixedName += name[i];
    }
    return fixedName;
}
int     parseInt   (const QString &input, const QString &ignoreList)
{
    if (input.isEmpty())
        return 0;

    const size_t size(input.size());
    size_t index{};

    for (; index < size; ++index)
        if (input[index].isDigit())
            break;

    if (index == size)
        return 0;

    signed char sign(index != 0 && input[index - 1] == '-' ? -1 : 1);
    int output{};

    while (index < size)
    {
        if (input[index].isDigit())
        {
            output *= 10;
            output += input[index].digitValue();
        }
        else if (!ignoreList.contains(input[index]))
            break;

        ++index;
    }
    return output * sign;
}
double  parseDouble(const QString &input  )
{
    double output(0);

    int i(-1), sign(1), base(10), size(input.size());
    while (input[++i] != '.')
    {
        if (int temp(input[i].digitValue()); temp != -1)
        {
            output *= base;
            output += temp;
        }
        else if (input[i] == '-' && input[i + 1].isDigit()) sign *= -1;
        else if (i == size - 1) return output * sign;
    }

    while (input[++i].isDigit())
    {
        output += static_cast<double>(input[i].digitValue()) / base;

        ++i;
    }
    return output * sign;
}
QString commaPrint (long value, char comma)
{
    QString formatted;
    unsigned char counter{};

    while (value)
    {
        formatted.prepend(static_cast<char>(value % 10 + '0'));

        if (counter == 2 && value > 9)
        {
            counter = 0;
            formatted.prepend(comma);
        }
        else
            ++counter;

        value /= 10;
    }
    return formatted;
}

Mode  stringToMode (const char *str) noexcept
{
    for (auto mode : modes_array)
    {
        if (modeToString(mode) == str)
            return mode;
    }
    return Mode(-1);
}
Color stringToColor(const char *str) noexcept
{
    for (auto color : colors_array)
    {
        if (colorToString(color) == str)
            return color;
    }
    return Color(-1);
}

QPixmap characterPixmap(Character type, Mode mode)
{
    return QPixmap(QString(":/Characters/") + characterToString(type, mode) + ".png");
}

}











