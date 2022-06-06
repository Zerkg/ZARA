#include "HistoryView.h"
#include "ui_HistoryView.h"

#include "HistoryModel.h"
#include "DataTypes.h"

#include <QMessageBox>
#include <QKeyEvent>

HistoryView::HistoryView(QWidget *parent) : QWidget(parent) { init(); }

HistoryModel *HistoryView::model()
{
    return m_Model;
}

void HistoryView::setModel(HistoryModel *model)
{
    m_Model = model;

    ui->treeView_History->setModel(m_Model);

    ui->treeView_History->setColumnHidden(HistoryModel::C_SONG_ID    , true);
    ui->treeView_History->setColumnWidth (HistoryModel::C_DATE_TIME  , 120 );
    ui->treeView_History->setColumnWidth (HistoryModel::C_SONG_ARTIST, 180 );
    ui->treeView_History->setColumnWidth (HistoryModel::C_SONG_NAME  , 270 );

    auto updateRecordsCount([=](int count)
    {
        QString text("Records count: " + QString::number(count));
        ui->label_RecordsCount->setText(count ? text : "History is empty.");
    });

    connect(m_Model, &HistoryModel::recordsCountChanged, this, updateRecordsCount);

    updateRecordsCount(m_Model->recordsCount());
}

void HistoryView::init()
{
    ui = new Ui::HistoryView;
    ui->setupUi(this);

    auto sendUrlId([=](const QModelIndex& index)
    {
        if (!index.parent().isValid())
            return;

        emit recordSelected(m_Model->record(index));
        close();
    });

    auto clearHistory([this]()
    {
        if (QMessageBox::question(this, "Question", "Do you really want to clear the history?") == QMessageBox::Yes)
        {
            m_Model->clearContent();
            m_Model->saveHistory ();
        }
    });

    connect(ui->pushButton_DeleteRecords, &QPushButton::clicked    , this, &HistoryView::deleteRecords);
    connect(ui->pushButton_ClearHistory , &QPushButton::clicked    , this, clearHistory               );
    connect(ui->treeView_History        , &QTreeView::doubleClicked, this, sendUrlId                  );
}

void HistoryView::deleteRecords()
{
    QList<QModelIndex> selectedRows(ui->treeView_History->selectionModel()->selectedRows()), rowsToDelete;

    int recordsCount{};
    foreach(auto &index, selectedRows)
    {
        if (index.parent().isValid())
        {
            if (!selectedRows.contains(index.parent()))
                ++recordsCount;
            else continue;
        }
        else
            recordsCount += m_Model->itemFromIndex(index)->rowCount();

        rowsToDelete.append(index);
    }

    if (recordsCount)
    {
        QString message("Do you really want to delete %1 record(s) from history?");

        if (QMessageBox::question(this, "Question", message.arg(recordsCount)) == QMessageBox::No)
            return;

        std::sort(rowsToDelete.begin(), rowsToDelete.end(), [](const auto &a, const auto &b){ return a.row() < b.row(); });

        auto iter(rowsToDelete.cend());
        while (iter != rowsToDelete.cbegin())
            m_Model->deleteRecord(*(--iter));

    }
}
void HistoryView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape: close        (); break;
    case Qt::Key_Delete: deleteRecords(); break;
    default: break;
    }

    if ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_E)
    {
        if (ui->treeView_History->isExpanded(m_Model->index(0, 0)))
            ui->treeView_History->collapseAll();
        else
            ui->treeView_History->expandAll();
    }
}





















