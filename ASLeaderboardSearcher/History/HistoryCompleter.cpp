#include "HistoryCompleter.h"

#include "DataTypes.h"
#include "HistoryModel.h"

#include <QCompleter>
#include <QStringListModel>

CompleterManager::CompleterManager(QObject *parent)
    : QObject(parent)
{
    m_Completer = new QCompleter(this);
    m_Completer->setModel(new QStringListModel(this));
}

void CompleterManager::setHistoryModel(const HistoryModel *model)
{
    for (int i{}, rows(model->rowCount()); i < rows; ++i)
    {
        auto parentItem(model->item(i));
        for (int j{}, childCount(parentItem->rowCount()); j < childCount; ++j)
        {
            QString artist(parentItem->child(j, HistoryModel::C_SONG_ARTIST)->data(Qt::DisplayRole).toString());
            QString song  (parentItem->child(j, HistoryModel::C_SONG_NAME  )->data(Qt::DisplayRole).toString());

            tryAdd({ {}, artist, song, {} });
        }
    }
    connect(model, &HistoryModel::recordAdded, this, &CompleterManager::tryAdd);
}

QCompleter *CompleterManager::completer() const
{
    return m_Completer;
}

void CompleterManager::tryAdd(const SearchRecord &record)
{    
    auto model(static_cast<QStringListModel*>(m_Completer->model()));
    for (const auto str : { &record.artist, &record.songName })
    {
        int insertPos{};
        for (int i{}, rows(model->rowCount()); i < rows; ++i)
        {
            auto modelStr(model->data(model->index(i, 0)).toString());
            if (int c(str->compare(modelStr, Qt::CaseInsensitive)); c == 0)
            {
                insertPos = -1;
                break;
            }
            else if (c > 0)
                insertPos = i + 1;
        }
        if (insertPos != -1)
        {
            model->insertRow(insertPos);
            model->setData(model->index(insertPos), *str);
        }
    }
}
