#include "StatsChart.h"
#include "ui_StatsChart.h"

#include "qcustomplot.h"
#include "ASPhpQuery.h"
#include "ASFunctions.h"
#include "PlayerStats.h"

#include "TracerToolTip.h"


#include <QDebug>



StatsChart::StatsChart(QWidget *parent) : QWidget(parent){ init(); }

void StatsChart::setSource(const ASPhpLeaderboardQuery *source)
{
    m_Source = source;
    connect(source, &ASPhpQuery::dataProcessed, this, &StatsChart::updateChart);
}

void StatsChart::recalculateRange()
{
    QCPRange range;
    range.upper = std::numeric_limits<double>::min();
    range.lower = std::numeric_limits<double>::max();

    for (auto mode : as::modes_array)
    {
        if (!ui->widget_StatsChart->graph(mode)->visible())
            continue;

        const auto graphData(ui->widget_StatsChart->graph(mode)->data());

        range.upper = std::max(range.upper, graphData ->constBegin()   ->mainValue());
        range.lower = std::min(range.lower, (graphData->constEnd() - 1)->mainValue());
    }

    auto median((range.upper + range.lower) / 2);

    range.upper += median * 0.05;
    range.lower -= median * 0.05;

    ui->widget_StatsChart->yAxis->setRange(range);
}

void StatsChart::init()
{
    ui = new Ui::StatsChart;
    ui->setupUi(this);

    m_ToolTip = new TracerToolTip(this);
    m_ToolTip->hide();
    m_ToolTip->installEventFilter(this);

    ui->widget_StatsChart->xAxis->setLabel("Player postion");
    ui->widget_StatsChart->xAxis->setRange(0.8, 10.2);
    ui->widget_StatsChart->xAxis->ticker()->setTickCount(10);
    ui->widget_StatsChart->xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);

    ui->widget_StatsChart->yAxis->setLabel("Score");
    ui->widget_StatsChart->yAxis->setNumberFormat("f");
    ui->widget_StatsChart->yAxis->setNumberPrecision(0);

    ui->widget_StatsChart->installEventFilter(this);
    ui->widget_StatsChart->replot();

    m_Tracers.reserve(30);

    QColor colors[]{ QColor(0x92, 0xdc, 0x54), QColor(0xff, 0x85, 0x57), QColor(0x5d, 0xc2, 0xee) };
    for (auto mode : as::modes_array)
    {
        QPen p(colors[mode]);
        p.setWidth(2);

        auto graph(ui->widget_StatsChart->addGraph());
        graph->setName(as::modeToString(mode));
        graph->setPen(p);
        graph->setProperty("mode", mode);

        for (size_t i{}; i < 10; ++i)
        {
            QCPItemTracer *phaseTracer(new QCPItemTracer(ui->widget_StatsChart));

            phaseTracer->setGraph   (graph);
            phaseTracer->setGraphKey(i + 1);
            phaseTracer->setStyle   (QCPItemTracer::tsCircle  );
            phaseTracer->setPen     (p);
            phaseTracer->setBrush   (colors[mode].lighter(150));
            phaseTracer->setSize    (6);

            m_Tracers.push_back(phaseTracer);
        }

    }

    auto onCheckStateChanged([=](bool state)
    {
        const auto s(sender());
        setChartVisible(s == ui->checkBox_Casual ? as::CASUAL : s == ui->checkBox_Pro ? as::PRO : as::ELITE, state);

        recalculateRange();
        ui->widget_StatsChart->replot();
    });

    connect(ui->checkBox_Casual, &QCheckBox::toggled, this, onCheckStateChanged);
    connect(ui->checkBox_Pro   , &QCheckBox::toggled, this, onCheckStateChanged);
    connect(ui->checkBox_Elite , &QCheckBox::toggled, this, onCheckStateChanged);
    connect(static_cast<QGuiApplication*>(QGuiApplication::instance()), &QGuiApplication::applicationStateChanged, this, [this](auto state){ if (state != Qt::ApplicationActive) m_ToolTip->hide(); });
}

bool StatsChart::eventFilter(QObject* obj, QEvent *event)
{
    if (obj == m_ToolTip && event->type() == QEvent::Enter)
    {
        m_ToolTip->hide();
        return false;
    }

    if (event->type() != QEvent::MouseMove || static_cast<QGuiApplication*>(QGuiApplication::instance())->applicationState() != Qt::ApplicationActive)
        return false;

    auto *mouseEvent(static_cast<QMouseEvent*>(event));
    auto *item(static_cast<QCPItemTracer*>(ui->widget_StatsChart->itemAt(mouseEvent->pos())));

    if (item && item->visible())
    {
        auto toolTipPos(mouseEvent->globalPos());

        toolTipPos.rx() += 16;
        toolTipPos.ry() -= m_ToolTip->height() / 2;

        m_ToolTip->move(toolTipPos);

        if (!m_ToolTip->isVisible())
        {
            as::Mode mode(static_cast<as::Mode>(item->graph()->property("mode").toInt()));

            PlayerStats stats(m_Source->leaderboard(mode)->at(item->graphKey() - 1));
            m_ToolTip->loadSource(stats);
            m_ToolTip->show();
        }
    }
    else
        m_ToolTip->hide();

    return true;
}

void StatsChart::updateChart()
{
    for (const auto trace : qAsConst(m_Tracers))
        trace->setVisible(false);

    for (auto mode : as::modes_array)
    {
        const auto leaderboard(m_Source->leaderboard(mode));
        bool hasStats(leaderboard != nullptr);

        auto checkBox(findChildren<QCheckBox*>().at(mode));
        checkBox->setVisible(hasStats);
        setChartVisible(mode, hasStats && checkBox->isChecked());

        if (!hasStats)
            continue;

        QVector<double> playerPos(leaderboard->size()), playerScore(leaderboard->size());

        std::transform(leaderboard->cbegin(), leaderboard->cend(), playerScore.begin(), [](const auto &stats){ return stats.score; });
        std::iota     (playerPos    .begin(), playerPos    .end(), 1.0);

        for (int i{}; i < leaderboard->size(); ++i)
            m_Tracers[i + (mode * 10)]->setVisible(true);

        ui->widget_StatsChart->graph(mode)->setData(playerPos, playerScore, true);
    }

    recalculateRange();
    ui->widget_StatsChart->replot();
}

void StatsChart::setChartVisible(as::Mode mode, bool visible)
{
    auto chart(ui->widget_StatsChart->graph(mode));

    if (chart->visible() == visible)
        return;

    chart->setVisible(visible);

    for (int i{mode * 10}; i < 10 + mode * 10; ++i)
        m_Tracers[i]->setVisible(visible);
}






















