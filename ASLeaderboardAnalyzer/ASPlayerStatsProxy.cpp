#include "ASPlayerStatsProxy.h"
#include "ASPlayerStatsModel.h"

#include "ASTypes.h"
#include "TableModel.h"
#include "ASCharacter.h"

#include <QBrush>
//#include <QDebug>

using Index = ASAbstractPlayerStatsModel::StatsDataIndex;

FocusCompareProxy::FocusCompareProxy(QObject *parent)
    : QIdentityProxyModel(parent)
    , m_FocusModel(nullptr){}

QVariant FocusCompareProxy::data(const QModelIndex &index, int role) const
{
    int sourceValue(sourceModel()->data(sourceModel()->index(index.row(), index.column()), Qt::EditRole).toInt());
    int focusValue (m_FocusModel ->data(m_FocusModel ->index(index.row(), index.column()), Qt::EditRole).toInt());

    if      (role == Qt::DisplayRole && sourceValue > 0)
        return (QString::number(sourceValue) + diffToString(sourceValue - focusValue));
    else if (role == Qt::ForegroundRole)
        return compareValues(sourceValue, focusValue);
    else
        return QIdentityProxyModel::data(index, role);
}

void FocusCompareProxy::setFocusModel(QAbstractItemModel *model)
{
    m_FocusModel = model;
}
QString FocusCompareProxy::diffToString(int value) const noexcept
{
    if (value != 0)
        return QString::asprintf("       (%+d)", value);
    else return QString();
}
template<typename T>
QVariant FocusCompareProxy::compareValues(T first, T second) const noexcept
{
    if      (first > second) return QBrush(Qt::darkGreen);
    else if (first < second) return QBrush(Qt::darkRed  );

    else return QVariant();
}



FocusStatsCompareProxy::FocusStatsCompareProxy(QObject *parent) noexcept
    : FocusCompareProxy(parent) {}

QVariant FocusStatsCompareProxy::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::ForegroundRole:
    {
        int row(index.row()), dataRole(row < 9 ? Qt::EditRole : Qt::DisplayRole);

        double source(sourceModel()->data(sourceModel()->index(index.row(), 1), dataRole).toDouble());
        double focus (m_FocusModel ->data(m_FocusModel ->index(index.row(), 1), dataRole).toDouble());

        return foregroundHandler(source, focus, row);
    }
    default: return QIdentityProxyModel::data(index, role); // clazy:exclude=skipped-base-method
    }
}
QVariant FocusStatsCompareProxy::foregroundHandler(double source, double focus, int row) const
{
    if (source == focus) return QVariant();

    switch (row)
    {
    case Index::DROPPED_CHAIN  :
    case Index::MAX_CONS_DODGED:
    case Index::OVERFILLS      :
        return compareValues(focus, source);

    case Index::CHARACTER_DATA_1:
    case Index::CHARACTER_DATA_2:
    {
        int sourceType(sourceModel()->data(sourceModel()->index(Index::CHARACTER_TYPE, 1), Qt::EditRole).toInt());
        int focusType (m_FocusModel ->data(m_FocusModel ->index(Index::CHARACTER_TYPE, 1), Qt::EditRole).toInt());

        if (sourceType != focusType) return QVariant();

        [[fallthrough]];
    }
    case Index::BONUS_IRONMODE  :
    case Index::MULTIPLIERS_USED:
    case Index::PAINTS_USED     :
    case Index::SHTORMS_USED    :
    case Index::SORTS_USED      :
        return (focus != source ? QVariant(QBrush(Qt::darkYellow)) : QVariant());

    default: return compareValues(source, focus);
    }
}







ASPlayerStatsCompareModel::ASPlayerStatsCompareModel(QObject *parent) noexcept
    : ASAbstractPlayerStatsModel(parent)
    , m_SourceModel(nullptr){}

ASAbstractPlayerStatsModel::AdditionalData ASPlayerStatsCompareModel::additionalData() const
{
    return m_SourceModel->additionalData();
}
ASAbstractPlayerStatsModel *ASPlayerStatsCompareModel::sourceModel() noexcept
{
    return m_SourceModel;
}
void ASPlayerStatsCompareModel::setModelSource(const PlayerStats &pStats)
{
    m_SourceModel->setModelSource(pStats);
}


ASFocusCompareModel::ASFocusCompareModel(QObject *parent) noexcept
    : ASPlayerStatsCompareModel(parent)
{
    m_BlocksStats = new FocusCompareProxy     (this);
    m_ScoreStats  = new FocusStatsCompareProxy(this);
}

void ASFocusCompareModel::setSourceModel(ASAbstractPlayerStatsModel *source) noexcept
{
    if (m_SourceModel != nullptr)
        QObject::disconnect(m_SourceModel, &ASAbstractPlayerStatsModel::dataChanged, this, &ASFocusCompareModel::dataChanged);

    m_SourceModel = source;
    QObject::connect(m_SourceModel, &ASAbstractPlayerStatsModel::dataChanged, this, &ASFocusCompareModel::dataChanged);

    qobject_cast<QIdentityProxyModel*>(m_BlocksStats)->setSourceModel(source->blocksStatsModel());
    qobject_cast<QIdentityProxyModel*>(m_ScoreStats )->setSourceModel(source->scoreStatsModel ());
}
void ASFocusCompareModel::setFocusModel(ASAbstractPlayerStatsModel *focusModel) noexcept
{
    if (focusModel == nullptr) return;

    m_FocusModel = focusModel;

    dynamic_cast<FocusCompareProxy*     >(m_BlocksStats)->setFocusModel(focusModel->blocksStatsModel());
    dynamic_cast<FocusStatsCompareProxy*>(m_ScoreStats )->setFocusModel(focusModel->scoreStatsModel ());
}

















//MaxMinCompareProxy::MaxMinCompareProxy(QObject *parent)
//    : QIdentityProxyModel(parent) {}

//QVariant MaxMinCompareProxy::data(const QModelIndex &index, int role) const
//{
//    switch (role)
//    {
//    case Qt::FontRole:
//    case Qt::ForegroundRole:
//    {
//        double source(sourceModel()->data(sourceModel()->index(index.row(), index.column()), Qt::EditRole).toDouble());
//        double max(source);
//        double min(source);

//        for (int i{}; i < m_Models.size(); ++i)
//        {
//            double focusValue (m_Models[i]  ->data(m_Models[i]  ->index(index.row(), index.column()), Qt::EditRole).toDouble());

//            if      (focusValue > source) max = focusValue;
//            else if (focusValue < source) min = focusValue;

//            if (source > min && source < max) return QIdentityProxyModel::data(index, role);
//        }
//        if (!max && !min) return QIdentityProxyModel::data(index, role);

//        if      (role == Qt::ForegroundRole) return foregroundHandler(max, min, source       );
//        else if (role == Qt::FontRole      ) return fontHandler      (max, min, source, index);

//        else break;
//    }
//    default: break;
//    }
//    return QIdentityProxyModel::data(index, role);
//}

//QVariant MaxMinCompareProxy::fontHandler(double max, double min, double source, const QModelIndex &index) const
//{
//    QFont font(sourceModel()->data(sourceModel()->index(index.row(), index.column()), Qt::FontRole).value<QFont>());
//    font.setBold(true);

//    if ((min == source) || (max == source)) return font;
//    else                                    return QVariant();
//}
//QVariant MaxMinCompareProxy::foregroundHandler(double max, double min, double source) const
//{
//    if      (min == source) return (QBrush(qRgb(182, 0, 255)));
//    else if (max == source) return (QBrush(qRgb(255, 0, 0  )));

//    return QVariant();
//}

//void MaxMinCompareProxy::setModelsList(const QList<const QAbstractItemModel *> &models)
//{
//    m_Models = models;
//    while (m_Models.contains(sourceModel()))
//        m_Models.removeAt(m_Models.indexOf(sourceModel()));
//}
//void MaxMinCompareProxy::setSourceModel(QAbstractItemModel *model)
//{
//    removeModel(model);
//    QIdentityProxyModel::setSourceModel(model);
//}

//void MaxMinCompareProxy::addModel(const QAbstractItemModel *model) noexcept
//{
//    //m_Models.push_back(qobject_cast<TableModel*>(model));
//}
//void MaxMinCompareProxy::removeModel(const QAbstractItemModel *model) noexcept
//{
//    for (int i{}; i < m_Models.size(); ++i)
//        if (m_Models[i] == model)
//            m_Models.removeAt(i);
//}



//MaxMinStatsCompareProxy::MaxMinStatsCompareProxy(QObject *parent) noexcept
//    : MaxMinCompareProxy(parent){}

//QVariant MaxMinStatsCompareProxy::data(const QModelIndex &index, int role) const
//{
//    switch (role)
//    {
//    case Qt::FontRole:
//    case Qt::ForegroundRole:
//    {
//        int dataRole(index.row() < 9 ? Qt::EditRole : Qt::DisplayRole);
//        return MaxMinCompareProxy::data(sourceModel()->index(index.row(), 1), dataRole);
//    }
//    default: break;
//    }
//    return MaxMinCompareProxy::data(index, role);
//}











//ASMinMaxCompareModel::ASMinMaxCompareModel(QObject *parent) noexcept
//    : ASPlayerStatsCompareModel(parent)
//{
//    m_BlocksStats = new MaxMinCompareProxy     (this);
//    m_ScoreStats  = new MaxMinStatsCompareProxy(this);

//    m_ModelIndex = m_IdCounter++;
//    m_Models.append(nullptr);
//}

//void ASMinMaxCompareModel::setSourceModel(ASAbstractPlayerStatsModel *source) noexcept
//{
//    if (m_Models[m_ModelIndex] != nullptr);

//    m_Models[m_ModelIndex] = source;

//    dynamic_cast<MaxMinCompareProxy     *>(m_BlocksStats)->addModel(m_Models[m_ModelIndex]->blocksStatsModel());
//    dynamic_cast<MaxMinStatsCompareProxy*>(m_ScoreStats )->addModel(m_Models[m_ModelIndex]->scoreStatsModel ());
//}

//void ASMinMaxCompareModel::setModelsList(const QList<ASAbstractPlayerStatsModel *> &models) noexcept
//{
//    m_Models = models;

//    if (m_Models.contains(m_SourceModel))
//        removeModel(m_SourceModel);

//    for (int i{}; i < m_Models.size(); ++i)
//    {
//        dynamic_cast<MaxMinCompareProxy     *>(m_BlocksStats)->addModel(m_Models[i]->blocksStatsModel());
//        dynamic_cast<MaxMinStatsCompareProxy*>(m_ScoreStats )->addModel(m_Models[i]->scoreStatsModel() );
//    }
//}

//void ASMinMaxCompareModel::addModel(ASAbstractPlayerStatsModel *model) noexcept
//{
//    m_Models.push_back(model);
//}

//void ASMinMaxCompareModel::removeModel(ASAbstractPlayerStatsModel *model) noexcept
//{
//    for (int i{}; i < m_Models.size(); ++i)
//        if (m_Models[i] == model)
//        {
//            dynamic_cast<MaxMinCompareProxy     *>(m_BlocksStats)->removeModel(m_Models[i]->blocksStatsModel());
//            dynamic_cast<MaxMinStatsCompareProxy*>(m_ScoreStats )->removeModel(m_Models[i]->scoreStatsModel() );

//            m_Models.removeAt(i);
//            --i;
//        }
//}


//int ASMinMaxCompareModel::m_IdCounter(0);
