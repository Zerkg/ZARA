#ifndef ASPLAYERSTATSCONTROLLER_H
#define ASPLAYERSTATSCONTROLLER_H

#include "ASTypes.h"
#include <QWidget>

class ASLeaderboardSelector;

class ASPlayerStatsView;
class ASAbstractPlayerStatsModel;
class QStringListModel;


//=============================================:: GUI Controller ::=============================================

class DataController;

namespace Ui {
class GuiController;
}

class GuiController : public QWidget
{
    Q_OBJECT

public:

    GuiController(QWidget *parent = nullptr);

    void setDataController(DataController *controller);

public slots:

    void updateView(int index);
    void setModel  (int index, ASAbstractPlayerStatsModel *model);
    void setViewsCount (int count);

private:

    Ui::GuiController *ui;

    ASPlayerStatsView* initView();

    QVector<ASPlayerStatsView*> m_Views;

    DataController *m_DataController;

signals:

    void viewsCountChanged(int);
    void viewCreated  (ASPlayerStatsView*);
    void viewDestroyed(int);

    void playerChanged(ASPlayerStatsView*, int);

};



//=============================================:: Data Controller ::=============================================

class DataController : public QObject
{
    Q_OBJECT

public:

    DataController(QObject *parent = nullptr);

    void setLeaderboardSelector(const ASLeaderboardSelector *selector);
    void createModel(ASPlayerStatsView *dest);

public slots:

    void setCompareModel(int model);
    void setFocusModel  (int index);

    void loadStats(ASPlayerStatsView *dest, int index);
    void setPlayersList(const QStringList &players);
    void deleteModel(int index);

private:

    void updateModel   (int index);
    void initComparer  (int index);
    void removeComparer(int index);

    const ASLeaderboardSelector* m_Selector;

    QVector<ASAbstractPlayerStatsModel*> m_Models;
    QVector<ASAbstractPlayerStatsModel*> m_ModelsPool;

    int m_CompareModelType;
    int m_FocusModel;

    QStringListModel *m_PlayersList;

signals:

    void modelUpdated(int) const;
    void modelChanged(int, ASAbstractPlayerStatsModel*);

};

#endif // ASPLAYERSTATSCONTROLLER_H
