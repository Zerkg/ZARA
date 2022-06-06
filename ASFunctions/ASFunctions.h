#ifndef ASFUNCTIONS_H
#define ASFUNCTIONS_H

#include "ASFunctions_global.h"
#include "ASTypes.h"

namespace std {
template <typename T>
class shared_ptr;
}
class ASCharacter;
class QStringList;
class QPixmap;

namespace as
{
ASFUNCTIONS_EXPORT QString convertNumber   (double number, char sep = ',');
ASFUNCTIONS_EXPORT QString fixName         (const QString &name          );
ASFUNCTIONS_EXPORT int     intFromString   (const QString &input         );
ASFUNCTIONS_EXPORT double  doubleFromString(const QString &input         );


ASFUNCTIONS_EXPORT QString modeToString(Mode mode         ) noexcept;
ASFUNCTIONS_EXPORT Mode    stringToMode(const QString& str) noexcept;


ASFUNCTIONS_EXPORT QString      characterToString(Character type, Mode mode            ) noexcept;
ASFUNCTIONS_EXPORT QString      characterToString(std::shared_ptr<ASCharacter> charater) noexcept;
ASFUNCTIONS_EXPORT ASCharacter* createCharacter  (Character type, Mode mode            ) noexcept;
ASFUNCTIONS_EXPORT QPixmap      characterPixmap  (Character type, Mode mode            ) noexcept;
ASFUNCTIONS_EXPORT QPixmap      characterPixmap  (std::shared_ptr<ASCharacter> charater) noexcept;
ASFUNCTIONS_EXPORT QStringList  charactersList   (                                     ) noexcept;

ASFUNCTIONS_EXPORT QString     colorToString(Color c           ) noexcept;
ASFUNCTIONS_EXPORT Color       stringToColor(const QString& str) noexcept;
ASFUNCTIONS_EXPORT QStringList colorsList   (                  ) noexcept;
}

#endif // ASFUNCTIONS_H
