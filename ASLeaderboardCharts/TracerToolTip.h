#ifndef TRACERTOOLTIP_H
#define TRACERTOOLTIP_H

#include <QWidget>

struct PlayerStats;

namespace Ui {
class TracerToolTip;
}

class TracerToolTip : public QWidget
{
    Q_OBJECT

public:

    explicit TracerToolTip(QWidget *parent = nullptr);
    ~TracerToolTip();

public slots:

    void loadSource(const PlayerStats &source) noexcept;

protected:

    virtual void paintEvent(QPaintEvent *event) override;

private:

    Ui::TracerToolTip *ui;
    QColor m_BgColor;

};

#endif // TRACERTOOLTIP_H
