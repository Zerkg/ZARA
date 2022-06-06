#ifndef STATSCHART_H
#define STATSCHART_H

#include "ASLeaderboardCharts_global.h"

#include <QWidget>

class ASPhpLeaderboardQuery;
class QCPItemTracer;
class TracerToolTip;

namespace Ui { class StatsChart; }
namespace as { enum Mode : signed char; }


class ASLEADERBOARDCHARTS_EXPORT StatsChart : public QWidget
{
    Q_OBJECT

public:

    explicit StatsChart(QWidget *parent = nullptr);

    void setSource(const ASPhpLeaderboardQuery* source);
    void recalculateRange();

private:

    void init();

    Ui::StatsChart *ui;
    const ASPhpLeaderboardQuery* m_Source;
    std::vector<QCPItemTracer*> m_Tracers;
    TracerToolTip *m_ToolTip;

protected:

    virtual bool eventFilter(QObject*, QEvent *event) override;

private slots:

    void updateChart();
    void setChartVisible(as::Mode mode, bool visible);

};

#endif // STATSCHART_H
