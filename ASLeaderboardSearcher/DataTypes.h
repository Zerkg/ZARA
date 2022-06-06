#ifndef DATATYPES_H
#define DATATYPES_H

#include <QDateTime>

struct SearchRecord
{
    quint32 songID;

    QString artist;
    QString songName;

    QDateTime searchDateTime;
    quint32 searches{1};
};

#endif // DATATYPES_H
