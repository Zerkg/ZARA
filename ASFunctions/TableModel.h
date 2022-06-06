#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include "ASFunctions_global.h"

#include <QAbstractItemModel>

struct PlayerStats;

class ASFUNCTIONS_EXPORT TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    explicit TableModel(int nColumns = 0, int nRows = 0, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool  setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
    int rowCount   (const QModelIndex & = QModelIndex()) const override { return m_Rows   ; }
    int columnCount(const QModelIndex & = QModelIndex()) const override { return m_Columns; }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData (const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    // Row & column control:
    bool appendRow(int count = 1);
    bool appendColumn(int count = 1);

    bool reset();

private:

    void init();

    QHash<QModelIndex    , QHash  <int, QVariant>>        m_Data      ;   //int here is actually Qt::ItemDataRole enumerator
    QHash<Qt::Orientation, QVector<QHash<int, QVariant>>> m_HeaderData;   //int here is actually Qt::ItemDataRole enumerator

    int m_Columns, m_Rows;

};

#endif // TABLEMODEL_H
