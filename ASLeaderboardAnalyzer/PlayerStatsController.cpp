#include "PlayerStatsController.h"
#include "ui_PlayerStatsController.h"

#include "PlayerStatsWidget.h"
#include "StatsConverter.h"

#include <QDebug>

PlayerStatsController::PlayerStatsController(QWidget *parent) : QWidget(parent) { init(); }
PlayerStatsController::PlayerStatsController(uchar viewsCount, QWidget *parent)
    : QWidget(parent),
      m_StatsData(viewsCount)
{
    init();
    setViewsCount(viewsCount);
}

void PlayerStatsController::setViewsCount(uchar count)
{
    if (count == 0)
        return;

    QVBoxLayout *columns[] { ui->layout_Column1, ui->layout_Column2 };

    while (m_StatsData.size() != count)
    {
        if (m_StatsData.size() > count)
            deleteView();
        else
            createView();
    }

    for (int i{}; i < count - 1; ++i)
        columns[i % 2]->addWidget(m_StatsData[i].first);

    if (count % 2 == 1)
        ui->layout_Adjusting->insertWidget(1, m_StatsData.back().first);
    else
        ui->layout_Column2  ->addWidget(m_StatsData.back().first);
}

void PlayerStatsController::displayStats(const PlayerStats &stats)
{
    for (size_t i{}; i < m_StatsData.size(); ++i)
        if (m_StatsData[i].second.rID.isEmpty())
        {            
            m_StatsData[i].second = stats;
            m_StatsData[i].first->displayStats(stats);

            if (i == m_FocusWidget && m_DisplayMode == sc::DIFF)
                for (size_t i{}; i < m_StatsData.size(); ++i)
                {
                    if (i == m_FocusWidget)
                        continue;

                    auto converter(static_cast<StatsDiffConverter*>(m_StatsData[i].first->statsConverter()));
                    converter->setFocusStats(m_StatsData[m_FocusWidget].second);

                    if (!m_StatsData[i].second.rID.isEmpty())
                        m_StatsData[i].first->displayStats(m_StatsData[i].second);
                }

            return;
        }
    emit outOfFreeViews();
}

void PlayerStatsController::resetStats(const QString &rideId)
{
    const auto viewsCount(m_StatsData.size());
    auto statsPos(viewsCount);

    for (size_t i{}; i < viewsCount; ++i)
        if (m_StatsData[i].second.rID == rideId)
        {
            statsPos = i;
            break;
        }

    if (statsPos == viewsCount)
        return;

    m_StatsData[statsPos].second.rID.clear();
    m_StatsData[statsPos].first->reset();

    if (m_DisplayMode == sc::DIFF && statsPos == m_FocusWidget)
        updateFocusWidget();
}
void PlayerStatsController::resetStats(const PlayerStats &stats)
{
    resetStats(stats.rID);
}

void PlayerStatsController::setDisplayMode(int mode)
{
    if (m_DisplayMode == mode)
        return;

    m_DisplayMode = mode;

    if (m_StatsData.empty())
        return;

    std::function<AbstractStatsConverter*()> initConverter;

    switch (m_DisplayMode)
    {
    case sc::DEFAULT:
    {
        initConverter = [    ]
        { return new StatsDefaultConverter; };
        break;
    }
    case sc::DIFF:
    {
        initConverter = [this]
        { return new StatsDiffConverter(m_StatsData[m_FocusWidget].second); };
        break;
    }
    default: return;
    }

    for (auto &data : m_StatsData)
    {
        if (data.first == m_StatsData[m_FocusWidget].first)
            continue;

        data.first->setStatsConverter(initConverter());

        if (!data.second.rID.isEmpty())
            data.first->displayStats(data.second);
    }
}

void PlayerStatsController::reset()
{
    for (auto &data : m_StatsData)
    {
        data.first->reset();
        data.second.rID.clear();
    }
}

void PlayerStatsController::init()
{
    ui = new Ui::PlayerStatsController;
    ui->setupUi(this);

    m_DisplayMode = sc::DEFAULT;
    m_FocusWidget = 0;
}

void PlayerStatsController::createView()
{
    PlayerStatsWidget *view(new PlayerStatsWidget(this));

    switch (m_DisplayMode)
    {
    case sc::DIFF:
    {
        auto converter(new StatsDiffConverter);

        if (!m_StatsData.empty() && m_FocusWidget)
            converter->setFocusStats(m_StatsData[m_FocusWidget].second);

        view->setStatsConverter(converter);
        break;
    }
    default: break;
    }

    view->reset();

    m_StatsData.push_back({ view, {} });
}
void PlayerStatsController::deleteView()
{
    m_StatsData.back().first->deleteLater();
    m_StatsData.pop_back();

    if (m_FocusWidget == m_StatsData.size())
        updateFocusWidget();
}

bool PlayerStatsController::updateFocusWidget()
{
    if (m_DisplayMode == sc::DIFF)
        m_StatsData[m_FocusWidget].first->setStatsConverter(new StatsDiffConverter());

    m_FocusWidget = 0;

    for (size_t i{}; i < m_StatsData.size(); ++i)
        if (!m_StatsData[i].second.rID.isEmpty())
        {
            m_FocusWidget = i;
            break;
        }

    const auto &focusData(m_StatsData[m_FocusWidget]);
    focusData.first->setStatsConverter(new StatsDefaultConverter);

    if (!focusData.second.rID.isEmpty())
    {
        focusData.first->displayStats(focusData.second);

        for (auto &data : m_StatsData)
        {
            if (data.first == focusData.first)
                continue;

            auto converter(static_cast<StatsDiffConverter*>(data.first->statsConverter()));
            converter->setFocusStats(focusData.second);

            if (!data.second.rID.isEmpty())
                data.first->displayStats(data.second);
        }
    }

    return false;
}





