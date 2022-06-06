#include "ASPlayerStatsController.h"
#include "ui_GuiController.h"

#include <QStringListModel>

#include "ASPlayerStatsView.h"

#include "ASPhpQuery.h"

#include "CompareModeSettings.h"
#include "ASLeaderboardSelector.h"
#include "ASPlayerStatsProxy.h"

//#include <QDebug>

GuiController::GuiController(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GuiController)
{
    ui->setupUi(this);

    m_Views    .reserve(10);
}

void GuiController::setDataController(DataController *controller)
{
    if (controller != nullptr)
    {
        m_DataController = controller;

        QObject::connect(controller, &DataController::modelUpdated , this      , &GuiController ::updateView );
        QObject::connect(controller, &DataController::modelChanged , this      , [=](int i, ASAbstractPlayerStatsModel *m){ setModel(i, m); } );
        QObject::connect(this      , &GuiController ::playerChanged, controller, &DataController::loadStats  );
        QObject::connect(this      , &GuiController ::viewCreated  , controller, &DataController::createModel);
        QObject::connect(this      , &GuiController ::viewDestroyed, controller, &DataController::deleteModel);

        setViewsCount(1);
        ui->layout_Column1->addWidget(m_Views.last());
    }
}

void GuiController::updateView(int index)
{
    m_Views[index]->updateView();
}

void GuiController::setModel(int index, ASAbstractPlayerStatsModel *model)
{
    m_Views[index]->setModel(model);
    updateView(index);
}


void GuiController::setViewsCount(int count)
{
    int oldSize(m_Views.size());

    if (count > oldSize)
        while (m_Views.size() != count)
            m_Views.append(initView());
    else
        while (m_Views.size() != count)
        {
            delete m_Views.takeLast();
            emit viewDestroyed(m_Views.size());
        }

    QVBoxLayout *columns[] { ui->layout_Column1, ui->layout_Column2 };

    for (int i{ oldSize % 2 ? oldSize - 1 : oldSize }; i < count - 1; ++i)
    {
        columns[i % 2]->addWidget(m_Views[i]);
        m_Views[i]->updateView();
    }

    if (count > 1)
    {
        if (count % 2 == 1) ui->layout_Adjusting->insertWidget(1, m_Views.last());
        else                ui->layout_Column2  ->addWidget   (   m_Views.last());
    }

    emit viewsCountChanged(count);
}



ASPlayerStatsView *GuiController::initView()
{
    ASPlayerStatsView *view(new ASPlayerStatsView(this));

    QObject::connect(view, &ASPlayerStatsView::currentPlayerChanged, this, [=](int i){ emit playerChanged(view, i); });

    emit viewCreated(view);
    return view;
}



DataController::DataController(QObject *parent)
    : QObject(parent)
    , m_CompareModelType(0)
    , m_FocusModel(0)
    , m_PlayersList(new QStringListModel(this)) {}


void DataController::setLeaderboardSelector(const ASLeaderboardSelector *selector)
{
    m_Selector = selector;

    QObject::connect(m_Selector, &ASLeaderboardSelector::modeChanged, this, [=](){ m_PlayersList->setStringList(m_Selector->leaderboard()->players()); });
}

void DataController::createModel(ASPlayerStatsView *dest)
{
    if (dest == nullptr) return;

    auto *model(dest->getModel());
    if (model != nullptr) delete model;

    model = new ASPlayerStatsModel(this);

    m_Models.push_back(model);

    if (m_CompareModelType != CompareModeSettings::NONE)
    {
        int index(m_Models.size() - 1);

        initComparer(index);
        updateModel (index);
    }
    dest->setModel(m_Models.last());
    dest->setPlayersList(m_PlayersList  );
}

void DataController::loadStats(ASPlayerStatsView *dest, int index)
{
    auto *model(dest->getModel());
    while (!m_Selector->leaderboard()->isStatsValid(index))
        QApplication::processEvents();

    model->setModelSource(m_Selector->leaderboard()->statsAt(index));

    if (m_Models.indexOf(dest->getModel()) == m_FocusModel)
        for (int i{}; i < m_Models.size(); ++i)
        {
            if (i == m_FocusModel) continue;

            emit modelUpdated(i);
        }
}

void DataController::setPlayersList(const QStringList &players)
{
    m_PlayersList->setStringList(players);
}

void DataController::deleteModel(int index)
{
    delete m_Models.takeAt(index);
}

void DataController::setCompareModel(int type )
{
    if (m_CompareModelType == type) return;
    if (m_CompareModelType != CompareModeSettings::NONE)
        for (int i{}; i < m_Models.size(); ++i)
            removeComparer(i);

    m_CompareModelType = type;
    for (int i{}; i < m_Models.size(); ++i)
    {
        if (i == m_FocusModel) continue;

        initComparer(i);
        updateModel (i);

        emit modelChanged(i, m_Models[i]);
    }
}
void DataController::setFocusModel  (int index)
{
    if (m_FocusModel == index) return;

    std::swap(index, m_FocusModel);

    removeComparer(m_FocusModel);
    emit modelChanged(m_FocusModel, m_Models[m_FocusModel]);

    if (index < m_Models.size())
    {
        initComparer  (index );
        emit modelChanged(index, m_Models[index]);
    }

    for (int i{}; i < m_Models.size(); ++i)
    {
        if (i == m_FocusModel) continue;

        updateModel(i);
        emit modelUpdated(i);
    }
}

void DataController::updateModel   (int index)
{
    switch (m_CompareModelType)
    {
    case CompareModeSettings::FOCUS: dynamic_cast<ASFocusCompareModel*>(m_Models[index])->setFocusModel(m_Models[m_FocusModel]); break;
    default: break;
    }
}
void DataController::initComparer  (int index)
{
    switch (m_CompareModelType)
    {
    case CompareModeSettings::FOCUS:
    {
        ASFocusCompareModel *proxy(new ASFocusCompareModel(this));

        proxy->setSourceModel(m_Models[index]);
        m_Models[index] = proxy;

        break;
    }

    default: return;
    }
}
void DataController::removeComparer(int index)
{
    ASPlayerStatsCompareModel* proxy(dynamic_cast<ASPlayerStatsCompareModel*>(m_Models[index]));
    if (proxy != nullptr)
    {
        m_Models[index] = proxy->sourceModel();
        delete proxy;
    }
}









