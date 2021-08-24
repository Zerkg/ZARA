#include "ASFunctions.h"

#include <memory>
#include <QPixmap>

#include "ASCharacter.h"

namespace as
{
    QString convertNumber   (double number, char sep)
    {
        QString value(QString::number(number, 'f', 0));

        for (int i{ value.size() - 1 }, j{1}; i > -1; --i, ++j)
            if (j % 3 == 0 && j!= 0 && i!= 0) value.insert(i, sep);
        return value;
    }

    QString fixName         (const QString &name )
    {
        if (name.isEmpty()) return QString();

        QString fixedName(name[0].toUpper());
        for(int i{1}, size(name.size()); i < size; ++i)
        {
            if      (name[i - 1] == ' ') fixedName += name[i].toUpper();
            else if (name[i - 1] == '(') fixedName += name[i].toUpper();

            else fixedName += name[i];
        }
        return fixedName;
    }
    int     intFromString   (const QString &input)
    {
        int output(0), sign(1);

        for(int i{}, size(input.size()); i < size; ++i)
        {
            if (int temp(input[i].digitValue()); temp != -1)
            {
                output *= 10;
                output += temp;
            }
            else if (input[i] == '-' && input[i + 1].isDigit()) sign *= -1;
        }
        return output * sign;
    }
    double  doubleFromString(const QString &input)
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


    Mode    stringToMode(const QString &str) noexcept
    {
        for (size_t i{}; i < modes_array.size(); ++i)
        {
            if (modeToString(modes_array[i]) == str)
                return modes_array[i];
        }
        return Mode(-1);
    }
    QString modeToString(Mode mode         ) noexcept
    {
        switch (mode)
        {
        case CASUAL: return "Casual";
        case PRO   : return "Pro";
        case ELITE : return "Elite";

        default: return "Unknown";
        }
    }

    ASCharacter *createCharacter  (Character type, Mode mode            ) noexcept
    {
        switch (type)
        {
        case DV      : return new DoubleVision(mode);
        case ERASER  : return new Eraser      (mode);
        case POINTMAN: return new Pointman    (mode);
        case PUSHER  : return new Pusher      (mode);
        case VEGAS   : return new Vegas       (    );
        case MONO    : return new Mono        (mode);

        default: return nullptr;
        }
    }
    QString      characterToString(Character type, Mode mode            ) noexcept
    {
        switch (type)
        {
        case VEGAS: return "Vegas";
        case ERASER:
        {
            if (mode == ELITE) return "Eraser " + modeToString(mode);
            else return "Eraser";
        }
        case PUSHER:
        {
            if (mode == ELITE) return "Pusher " + modeToString(mode);
            else return "Pusher";
        }
        case DV    :
        {
            if      (mode == ELITE) return "DVE";
            else if (mode == PRO  ) return "DVP";
            else return "DV";
        }
        case MONO:
        {
            if      (mode == ELITE) return "Mono Ninja";
            else if (mode == PRO  ) return "Mono " + modeToString(mode);
            else return "Mono";
        }
        case POINTMAN:
        {
            if (mode != CASUAL) return "Pointman " + modeToString(mode);
            else return "Pointman";
        }
        default: return "Unknown";
        }
    }
    QString      characterToString(std::shared_ptr<ASCharacter> charater) noexcept
    {
        return characterToString(charater->getType(), charater->getMode());
    }
    QPixmap      characterPixmap  (Character type, Mode mode            ) noexcept
    {
        return QPixmap(":/Characters/Images/Characters/" + characterToString(type, mode) + ".png");
    }
    QPixmap      characterPixmap  (std::shared_ptr<ASCharacter> charater) noexcept
    {
        return characterPixmap(charater->getType(), charater->getMode());
    }
    QStringList  charactersList   (                                     ) noexcept
    {
        QStringList charactersList;

        for(size_t i{}; i < characters_array.size(); ++i)
            charactersList << characterToString(characters_array[i], CASUAL);

        return charactersList;
    }

    QString colorToString(Color c           ) noexcept
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
    Color   stringToColor(const QString &str) noexcept
    {
        for (size_t i{}; i < colors_array.size(); ++i)
        {
            if (colorToString(colors_array[i]) == str)
                return colors_array[i];
        }
        return Color(-1);
    }

    QStringList colorsList() noexcept
    {
    QStringList colors;

    for (size_t i{}; i < colors_array.size(); ++i)
        colors << colorToString(colors_array[i]);

    return colors;
    }
}











