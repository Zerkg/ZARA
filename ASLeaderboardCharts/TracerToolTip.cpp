#include "TracerToolTip.h"
#include "ui_TracerToolTip.h"

#include "ASAfterrideBonus.h"
#include "PlayerStats.h"
#include "ASFunctions.h"

#include <QPainter>

TracerToolTip::TracerToolTip(QWidget *parent) :
    QWidget(parent)
{
    ui = new Ui::TracerToolTip;
    ui->setupUi(this);

    setAttribute  (Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
}

TracerToolTip::~TracerToolTip()
{
    delete ui;
}

void TracerToolTip::loadSource(const PlayerStats &source) noexcept
{   
    ui->label_Character->setPixmap(as::characterPixmap(source.characterData.type, source.characterData.mode));
    ui->label_Player   ->setText  (source.nickname);
    ui->label_Score    ->setText  ("<b>Score:</b> " + as::commaPrint(source.score));

    QStringList bonuses;
    for (auto bonus : source.bonuses.vectorize())
        bonuses.append(ASAfterrideBonus::bonusInfo(bonus).name);

    ui->label_Bonuses  ->setText  (bonuses.join('\n'));

    m_BgColor = QColor::fromRgb(as::modes_colors_array[source.characterData.mode]);

    adjustSize();
}

void TracerToolTip::paintEvent(QPaintEvent *event)
{
     QPainter painter(this);
     QPen pen(Qt::transparent);

     painter.setRenderHint (QPainter::Antialiasing);
     painter.setBrush      (QBrush(m_BgColor));
     painter.setPen    (pen );
     painter.setOpacity(0.65);

     painter.drawRoundedRect(this->rect(), 10, 10);

     QWidget::paintEvent(event);
}
