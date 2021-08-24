#include "SearchHistoryWidget.h"
#include "ui_SearchHistoryWidget.h"

#include <QMessageBox>
#include <QKeyEvent>
//#include <QDebug>

using namespace std::chrono_literals;

QDataStream& operator >>(QDataStream &out, HistoryRecord &hr)
{
    out >> hr.dateTime >> hr.artist >> hr.songName >> hr.songID >> hr.searches;
    return out;
}
QDataStream& operator <<(QDataStream &in, const HistoryRecord &hr)
{
    in << hr.dateTime << hr.artist << hr.songName << hr.songID << hr.searches;
    return in;
}



HistoryModel::HistoryModel(const QString &filePath, QObject *parent)
    : QStandardItemModel(0, Index::TOTAL_COUNT, parent)
    , m_SourceFile(filePath, this)
{
    setHorizontalHeaderItem(SONG_ID    , new QStandardItem("Song ID" ));
    setHorizontalHeaderItem(DATE_TIME  , new QStandardItem("Date"    ));
    setHorizontalHeaderItem(SONG_ARTIST, new QStandardItem("Artist"  ));
    setHorizontalHeaderItem(SONG_NAME  , new QStandardItem("Name"    ));
    setHorizontalHeaderItem(SEARCHES   , new QStandardItem("Searches"));

    if (m_SourceFile.exists())
        readData();
    else
    {
        m_SourceFile.open(QFile::WriteOnly);
        m_SourceFile.close();
    }
}

HistoryModel::~HistoryModel()
{
    writeData();
}

void HistoryModel::addRecord(HistoryRecord &hr)
{
    for (int i{}; i < rowCount(); ++i)
    {
        QStandardItem *topLevelParent(item(i));
        for (int j{}; j < topLevelParent->rowCount(); ++j)
        {
            if (topLevelParent->child(j, SONG_ID)->data(Qt::EditRole).toInt() == hr.songID)
            {
                hr.searches += topLevelParent->child(j, SEARCHES)->data(Qt::DisplayRole).toInt();
                topLevelParent->removeRow(j);
            }
        }
        if (!topLevelParent->hasChildren())
            removeRow(i);
    }

    QStandardItem *topRecord(item(DATE_TIME));

    if (topRecord == nullptr || topRecord->data(Qt::DisplayRole).toDate() != hr.dateTime.date())
        topRecord = createTopLevelRow(hr.dateTime.date());

    createRecordRow(topRecord, hr);
}

void HistoryModel::deleteRecord(const QModelIndex &index)
{
    if (index.parent().isValid())
    {
        int parentRow(index.parent().row());

        if (item(parentRow)->rowCount() == 1)
            removeRow(parentRow);
        else
            item(parentRow)->removeRow(index.row());
    }
}

void HistoryModel::writeData()
{
    if (!m_SourceFile.open(QFile::WriteOnly)) return;

    m_SourceFile.resize(0);
    QDataStream out (&m_SourceFile);

    for (int i(rowCount() - 1); i > -1; --i)
    {
        QStandardItem *parent(item(i));

        for (int j{}, childCount(parent->rowCount()); j < childCount; ++j)
        {
            HistoryRecord hr;

            hr.dateTime.setDate(item         (i, DATE_TIME)->data(Qt::DisplayRole).toDate());
            hr.dateTime.setTime(parent->child(j, DATE_TIME)->data(Qt::DisplayRole).toTime());

            hr.songID   = parent->child(j, SONG_ID    )->data(Qt::EditRole   ).toInt();
            hr.artist   = parent->child(j, SONG_ARTIST)->data(Qt::DisplayRole).toString();
            hr.songName = parent->child(j, SONG_NAME  )->data(Qt::DisplayRole).toString();
            hr.searches = parent->child(j, SEARCHES   )->data(Qt::DisplayRole).toInt();

            out << hr;
        }
    }
    m_SourceFile.close();
}

void HistoryModel::clearContent()
{
    setRowCount(0);
}

void HistoryModel::readData()
{
    if (m_SourceFile.open(QFile::ReadOnly))
    {
        resetInternalData();

        QDataStream in(&m_SourceFile);
        while (!in.atEnd())
        {
            HistoryRecord hr;
            in >> hr;

            addRecord(hr);
        }
        m_SourceFile.close();
    }
    else
    {
        QMessageBox::critical(nullptr, "Error", "Can't load history data: " + m_SourceFile.errorString());
        return;
    }
}

QStandardItem *HistoryModel::createTopLevelRow(const QDate &date)
{
    QList<QStandardItem *> row;
    for (int i{}; i < columnCount(); ++i)
    {
        QStandardItem *item(new QStandardItem);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsEditable);

        row.append(item);
    }

    row[DATE_TIME]->setData(date.toString(Qt::DateFormat::ISODate), Qt::DisplayRole);

    insertRow(0, row);
    return row[0];
}
void HistoryModel::createRecordRow(QStandardItem *parent, const HistoryRecord &hr)
{
    QList<QStandardItem *> row;
    for (int i{}, size(columnCount()); i < size; ++i)
    {
        QStandardItem *item(new QStandardItem);

        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        row.append(item);
    }

    row[SONG_ID    ]->setData(hr.songID         , Qt::EditRole   );
    row[DATE_TIME  ]->setData(hr.dateTime.time(), Qt::DisplayRole);
    row[SONG_ARTIST]->setData(hr.artist         , Qt::DisplayRole);
    row[SONG_NAME  ]->setData(hr.songName       , Qt::DisplayRole);
    row[SEARCHES   ]->setData(hr.searches       , Qt::DisplayRole);

    parent->insertRow(0, row);
}



SearchHistoryWidget::SearchHistoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchHistoryWidget)
{
    ui->setupUi(this);

    init();
}

SearchHistoryWidget::~SearchHistoryWidget()
{
    delete ui;
}

void SearchHistoryWidget::init()
{
    static HistoryModel model("Data/hr.dat", nullptr);

    m_Model = &model;

    ui->treeView_History->setModel(m_Model);

    ui->treeView_History->setColumnHidden(HistoryModel::SONG_ID    , true);
    ui->treeView_History->setColumnWidth (HistoryModel::DATE_TIME  , 90  );
    ui->treeView_History->setColumnWidth (HistoryModel::SONG_ARTIST, 240 );
    ui->treeView_History->setColumnWidth (HistoryModel::SONG_NAME  , 240 );
    ui->treeView_History->setColumnWidth (HistoryModel::SEARCHES   , 40  );

    auto sendUrlId([=](const QModelIndex& index)
    {
        if (!index.parent().isValid()) return;
        close();

        emit leaderboardSelected(m_Model->data(m_Model->index(index.row(), HistoryModel::SONG_ID, index.parent()), Qt::EditRole).toInt());
    });

    QObject::connect(ui->pushButton_DeleteSelected , &QPushButton::clicked    , this, &SearchHistoryWidget::deleteRecords);
    QObject::connect(ui->pushButton_ClearHistory   , &QPushButton::clicked    , this, &SearchHistoryWidget::clearHistory );
    QObject::connect(ui->treeView_History          , &QTreeView::doubleClicked, this, sendUrlId                          );
}

void SearchHistoryWidget::timerEvent(QTimerEvent *event)
{
    killTimer(event->timerId());
    m_Model->writeData();
}
void SearchHistoryWidget::deleteRecords()
{
    QList<QModelIndex> selectedRows(ui->treeView_History->selectionModel()->selectedRows());
    int size(selectedRows.size());

    if (size != 0 && QMessageBox::question(nullptr, "Question", "Do you really want to delete " + QString::number(size) + " record" + (size > 1 ? "s" : "") + " from history?") == QMessageBox::Yes)
    {
        foreach(const auto &index, selectedRows)
            m_Model->deleteRecord(index);
    }
}
void SearchHistoryWidget::addToHistory(HistoryRecord &hr)
{
    m_Model->addRecord(hr);
    startTimer(5min);
}
void SearchHistoryWidget::clearHistory()
{
    if (QMessageBox::question(nullptr, "Question", "Do you really want to clear the history?") == QMessageBox::Yes)
        m_Model->clearContent();
}
void SearchHistoryWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape: close        (); break;
    case Qt::Key_Delete: deleteRecords(); break;
    default: break;
    }
}
