#include "TableModel.h"

TableModel::TableModel(int nColumns, int nRows, QObject *parent)
    : QAbstractTableModel(parent)
    , m_Columns(nColumns)
    , m_Rows(nRows)
{
    init();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (m_HeaderData[orientation].size() > section)
        return m_HeaderData[orientation][section].value(role);
    else return QVariant();
}

bool TableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (m_HeaderData[orientation].size() <= section) m_HeaderData[orientation].resize(section + 5);

    m_HeaderData[orientation][section][role] = value;
    emit headerDataChanged(orientation, section, section);

    return true;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    return m_Data[index].value(role);
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) return false;

    m_Data[index][role] = value;
    emit dataChanged(index, index, QVector({ role }));

    return true;
}

bool TableModel::appendRow(int count)
{
    if (count <= 0) return false;

    beginInsertRows(QModelIndex(), m_Rows, count - 1);

    for (int oldSize(m_Rows); m_Rows < oldSize + count; ++m_Rows)
    {
        if (m_Columns)
            for (int column{}; column < m_Columns; ++column) m_Data[createIndex(m_Rows, column)];
        else m_Data[createIndex(m_Rows, 0)];
    }

    endInsertRows();
    return true;
}

bool TableModel::appendColumn(int count)
{
    if (count <= 0) return false;

    beginInsertColumns(QModelIndex(), m_Columns, count - 1);

    for (int oldSize(m_Columns); m_Columns < m_Columns + oldSize; ++m_Columns)
    {
        if (m_Rows)
            for (int row{}; row < m_Rows; ++row) m_Data[createIndex(row, m_Columns)];
        else m_Data[createIndex(0, m_Columns)];
    }


    endInsertColumns();
    return true;
}

bool TableModel::reset()
{
    beginResetModel();

    m_Data.clear();
    m_HeaderData.clear();

    m_Rows = 0;
    m_Columns = 0;

    endResetModel();

    return true;
}


void TableModel::init()
{
    if (m_Columns) m_HeaderData[Qt::Horizontal].resize(m_Columns);
    if (m_Rows   ) m_HeaderData[Qt::Vertical  ].resize(m_Rows   );

    for (int row{}; row < m_Rows; ++row)
        for (int column{}; column < m_Columns; ++column)
            m_Data[createIndex(row, column)];
}
