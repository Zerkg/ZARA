#ifndef ASPLAYERSTATSPROXY_H
#define ASPLAYERSTATSPROXY_H

#include <QIdentityProxyModel>
#include "ASPlayerStatsModel.h"

class TableModel;

class FocusCompareProxy : public QIdentityProxyModel
{
public:

    FocusCompareProxy(FocusCompareProxy&) = delete;
    FocusCompareProxy(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;

    void setFocusModel(QAbstractItemModel *model);

    template<typename T>
    QVariant compareValues(T first, T second) const noexcept;
    QString diffToString(int value) const noexcept;

protected:

    QAbstractItemModel *m_FocusModel;

};
class FocusStatsCompareProxy : public FocusCompareProxy
{
public:

    FocusStatsCompareProxy(QObject *parent = nullptr) noexcept;
    QVariant data(const QModelIndex &index, int role) const override;

protected:

    QVariant foregroundHandler(double source, double focus, int row) const;

};



class ASPlayerStatsCompareModel : public ASAbstractPlayerStatsModel
{
public:

    explicit ASPlayerStatsCompareModel(QObject *parent = nullptr) noexcept;

    virtual AdditionalData additionalData() const override;

    virtual void setSourceModel(ASAbstractPlayerStatsModel *source) noexcept = 0;
    ASAbstractPlayerStatsModel *sourceModel() noexcept;

    virtual void setModelSource(const PlayerStats &pStats) override;

protected:

    ASAbstractPlayerStatsModel *m_SourceModel;

};
class ASFocusCompareModel : public ASPlayerStatsCompareModel
{
public:

    explicit ASFocusCompareModel(QObject *parent = nullptr) noexcept;

    virtual void setSourceModel(ASAbstractPlayerStatsModel *source) noexcept override;
    void setFocusModel(ASAbstractPlayerStatsModel *focusModel) noexcept;

private:

    ASAbstractPlayerStatsModel *m_FocusModel;

};


























//class MaxMinCompareProxy : public QIdentityProxyModel
//{
//public:

//    MaxMinCompareProxy(MaxMinCompareProxy&) = delete;
//    MaxMinCompareProxy(QObject *parent = nullptr);

//    QVariant data(const QModelIndex &index, int role) const override;
//    void setModelsList(const QList<const QAbstractItemModel*> &models);

//    virtual void setSourceModel(QAbstractItemModel *model) override;

//    void addModel   (const QAbstractItemModel *model) noexcept;
//    void removeModel(const QAbstractItemModel *model) noexcept;

//protected:

//    QList<const QAbstractItemModel*> m_Models;

//    QVariant fontHandler      (double max, double min, double source, const QModelIndex &index) const;
//    QVariant foregroundHandler(double max, double min, double source                          ) const;

//};
//class MaxMinStatsCompareProxy : public MaxMinCompareProxy
//{
//public:

//    MaxMinStatsCompareProxy(QObject *parent = nullptr) noexcept;
//    QVariant data(const QModelIndex &index, int role) const override;

//};


//class ASMinMaxCompareModel : public ASPlayerStatsCompareModel
//{
//public:

//    explicit ASMinMaxCompareModel(QObject *parent = nullptr) noexcept;

//    virtual void setSourceModel(ASAbstractPlayerStatsModel *source) noexcept override;
//    void setModelsList(const QList<ASAbstractPlayerStatsModel*> &focusModel) noexcept;

//    void addModel   (ASAbstractPlayerStatsModel *model) noexcept;
//    void removeModel(ASAbstractPlayerStatsModel *model) noexcept;

//private:

//    static QList<ASAbstractPlayerStatsModel*> m_Models;
//    static int m_IdCounter;
//    int m_ModelIndex;

//};

#endif // ASPLAYERSTATSPROXY_H
