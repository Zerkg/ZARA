#include "HistoryModel.h"
#include "DataTypes.h"

#include <QFile>
#include <QStringListModel>

QDataStream& operator >>(QDataStream &out, SearchRecord &hr)
{
    out >> hr.searchDateTime >> hr.artist >> hr.songName >> hr.songID >> hr.searches;
    return out;
}
QDataStream& operator <<(QDataStream &in, const SearchRecord &hr)
{
    in << hr.searchDateTime << hr.artist << hr.songName << hr.songID << hr.searches;
    return in;
}



HistoryModel::HistoryModel(const QString &filePath, QObject *parent)
    : QStandardItemModel(0, C_TOTAL_COUNT, parent)
{
    m_HistoryFile      = new QFile           (filePath, this);

    setHorizontalHeaderItem(C_SONG_ID    , new QStandardItem("Song ID" ));
    setHorizontalHeaderItem(C_DATE_TIME  , new QStandardItem("Date"    ));
    setHorizontalHeaderItem(C_SONG_ARTIST, new QStandardItem("Artist"  ));
    setHorizontalHeaderItem(C_SONG_NAME  , new QStandardItem("Song"    ));
    setHorizontalHeaderItem(C_SEARCHES   , new QStandardItem("Searches"));

    if (m_HistoryFile->exists())
        loadHistory();
}

void HistoryModel::addRecord(const SearchRecord &sr)
{
    QList<QStandardItem*> record;
    QDate rd;

    auto index(findDuplicate(sr.songID));
    if (index.isValid())
    {
        record = updateRecord(index);
        rd = QDate::currentDate();
    }
    else
    {
        record = initRow();

        record[C_DATE_TIME  ]->setData(sr.searchDateTime.time(), Qt::DisplayRole);
        record[C_SEARCHES   ]->setData(sr.searches             , Qt::DisplayRole);
        record[C_SONG_ARTIST]->setData(sr.artist               , Qt::DisplayRole);
        record[C_SONG_ID    ]->setData(sr.songID               , Qt::DisplayRole);
        record[C_SONG_NAME  ]->setData(sr.songName             , Qt::DisplayRole);

        rd = sr.searchDateTime.date();
    }

    QStandardItem *group(item(0, C_DATE_TIME));
    if (group == nullptr || group->data(Qt::DisplayRole).toDate() != rd)
    {
        auto row(initRow());

        group = row[C_DATE_TIME];
        group->setData(rd, Qt::DisplayRole);

        insertRow(0, row);
    }
    group->insertRow(0, record);

    if (!index.isValid())
    {
        emit recordAdded(sr);
        emit recordsCountChanged(++m_RecordsCount);
    }
}

SearchRecord HistoryModel::record(const QModelIndex &index)
{    
    QStandardItem *group(itemFromIndex(index.parent()));

    if (group == nullptr)
        return SearchRecord();

    int recordRow(index.row());

    SearchRecord sr;

    sr.searchDateTime.setDate(group                               ->data(Qt::DisplayRole).toDate());
    sr.searchDateTime.setTime(group->child(recordRow, C_DATE_TIME)->data(Qt::DisplayRole).toTime());

    sr.songID   = group->child(recordRow, C_SONG_ID    )->data(Qt::DisplayRole).toInt   ();
    sr.artist   = group->child(recordRow, C_SONG_ARTIST)->data(Qt::DisplayRole).toString();
    sr.songName = group->child(recordRow, C_SONG_NAME  )->data(Qt::DisplayRole).toString();
    sr.searches = group->child(recordRow, C_SEARCHES   )->data(Qt::DisplayRole).toInt   ();

    return sr;
}

int HistoryModel::recordsCount() const noexcept
{
    return m_RecordsCount;
}

void HistoryModel::deleteRecord(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    if (!index.parent().isValid())
    {
        m_RecordsCount -= itemFromIndex(index)->rowCount();
        removeRow(index.row());
    }
    else
    {
        auto parent(itemFromIndex(index.parent()));

        if (parent->rowCount() < 2)
            removeRow(parent->row());
        else
            parent->removeRow(index.row());

        --m_RecordsCount;
    }

    emit recordsCountChanged(m_RecordsCount);
}
void HistoryModel::deleteRecord(int songID)
{
    for (int i{}; i < rowCount(); ++i)
    {
        QStandardItem *parent(item(i, C_DATE_TIME));

        for (int j{}, childCount(parent->rowCount()); j < childCount; ++j)
            if (parent->child(j, C_SONG_ID)->data(Qt::DisplayRole).toInt() == songID)
            {
                deleteRecord(parent->child(j, 0)->index());
                break;
            }
    }
}

void HistoryModel::saveHistory()
{
    if (!m_HistoryFile->open(QFile::WriteOnly))
        return;

    m_HistoryFile->resize(0);
    QDataStream out (m_HistoryFile);

    for (int i(rowCount() - 1); i > -1; --i)
    {
        QStandardItem *parent(item(i, C_DATE_TIME));

        for (int j{}, childCount(parent->rowCount()); j < childCount; ++j)
        {
            SearchRecord sr;

            sr.searchDateTime.setDate(parent                       ->data(Qt::DisplayRole).toDate());
            sr.searchDateTime.setTime(parent->child(j, C_DATE_TIME)->data(Qt::DisplayRole).toTime());

            sr.songID   = parent->child(j, C_SONG_ID    )->data(Qt::DisplayRole).toInt   ();
            sr.artist   = parent->child(j, C_SONG_ARTIST)->data(Qt::DisplayRole).toString();
            sr.songName = parent->child(j, C_SONG_NAME  )->data(Qt::DisplayRole).toString();
            sr.searches = parent->child(j, C_SEARCHES   )->data(Qt::DisplayRole).toInt   ();

            out << sr;
        }
    }
    m_HistoryFile->close();
}
void HistoryModel::loadHistory()
{
    if (m_HistoryFile->open(QFile::ReadOnly))
    {
        resetInternalData();

        QDataStream in(m_HistoryFile);
        while (!in.atEnd())
        {
            SearchRecord sr;
            in >> sr;

            addRecord(sr);
        }
        m_HistoryFile->close();
    }
}
void HistoryModel::clearContent()
{
    setRowCount(0);
    m_HistoryFile->resize(0);
    m_RecordsCount = 0;

    emit recordsCountChanged(0);
}

QModelIndex HistoryModel::findDuplicate(int songId) const
{
    for (int i{}; i < rowCount(); ++i)
    {
        QStandardItem *topLevelItem(item(i));
        for (int j{}; j < topLevelItem->rowCount(); ++j)
        {
            int sID(topLevelItem->child(j, C_SONG_ID)->data(Qt::DisplayRole).toInt());

            if (sID == songId)
                return topLevelItem->child(j, C_DATE_TIME)->index();
        }
    }
    return QModelIndex();
}

QList<QStandardItem*> HistoryModel::updateRecord(const QModelIndex &index)
{
    auto parentRow(itemFromIndex(index.parent()));
    auto row(parentRow->takeRow(index.row()));

    row[C_SEARCHES ]->setData(row[C_SEARCHES]->data(Qt::DisplayRole).toInt() + 1, Qt::DisplayRole);
    row[C_DATE_TIME]->setData(QTime::currentTime(), Qt::DisplayRole);

    if (!parentRow->rowCount() && parentRow->data(Qt::DisplayRole).toDate() != QDate::currentDate())
        removeRow(index.parent().row());

    return row;
}
QList<QStandardItem*> HistoryModel::initRow()
{
    QList<QStandardItem*> row;

    for (int i{}; i < C_TOTAL_COUNT; ++i) row.append(new QStandardItem);
    std::for_each(row.begin(), row.end(), [](auto *item){ item->setFlags(item->flags() & ~Qt::ItemIsEditable); });

    return row;
}







