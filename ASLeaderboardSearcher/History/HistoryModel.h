#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QStandardItemModel>
#include <QDateTime>

struct SearchRecord;

class QStringListModel;
class QFile;

class HistoryModel : public QStandardItemModel
{
    Q_OBJECT

public:

    enum Column
    {
        C_DATE_TIME,
        C_SONG_ARTIST,
        C_SONG_NAME,
        C_SONG_ID,
        C_SEARCHES,

        C_TOTAL_COUNT
    };

    explicit HistoryModel(const QString &filePath, QObject *parent = nullptr);

    SearchRecord record(const QModelIndex& index);
    int recordsCount() const noexcept;

public slots:

    void addRecord   (const SearchRecord &sr);
    void deleteRecord(const QModelIndex &index);
    void deleteRecord(int songID);

    void saveHistory();
    void loadHistory();

    void clearContent();

private:

    QModelIndex findDuplicate(int songId) const;
    QList<QStandardItem*> updateRecord(const QModelIndex &index);
    QList<QStandardItem*> initRow();

    QFile* m_HistoryFile;
    quint16 m_RecordsCount{};

signals:

    void recordsCountChanged(int);
    void recordAdded(const SearchRecord&);

};

#endif // HISTORYMODEL_H
