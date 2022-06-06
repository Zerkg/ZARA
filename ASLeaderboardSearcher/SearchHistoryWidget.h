#ifndef SEARCHHISTORYWIDGET_H
#define SEARCHHISTORYWIDGET_H

#include <QWidget>
#include <QDateTime>
#include <QStandardItemModel>
#include <QFile>

class QTreeWidgetItem;

struct HistoryRecord
{
    QDateTime dateTime;

    QString artist;
    QString songName;
    int songID;
    int searches = 1;
};

class HistoryModel : public QStandardItemModel
{
    Q_OBJECT

public:

    enum Index
    {
        DATE_TIME,
        SONG_ARTIST,
        SONG_NAME,
        SONG_ID,
        SEARCHES,

        TOTAL_COUNT
    };

    HistoryModel(const QString &filePath, QObject *parent = nullptr);
    ~HistoryModel();

    void addRecord(HistoryRecord &hr);
    void deleteRecord(const QModelIndex &index);
    void writeData();

public slots:

    void clearContent();

private:

    void readData();

    QStandardItem* createTopLevelRow(const QDate &date);
    void createRecordRow(QStandardItem* parent, const HistoryRecord &hr);

    QFile m_SourceFile;

};

namespace Ui {
class SearchHistoryWidget;
}

class SearchHistoryWidget : public QWidget
{
    Q_OBJECT

public:

    explicit SearchHistoryWidget(QWidget *parent = nullptr);
    ~SearchHistoryWidget();

public slots:

    void addToHistory(HistoryRecord& hr);
    void clearHistory();

protected:

    virtual void timerEvent(QTimerEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

private:

    void init();
    QTreeWidgetItem* findDuplicate(int id) const;

    Ui::SearchHistoryWidget *ui;

    HistoryModel* m_Model;

private slots:

    void deleteRecords();

signals:

    void leaderboardSelected(int);

};

#endif // SEARCHHISTORYWIDGET_H
