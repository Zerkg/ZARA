#include "PlayerStatsWidget.h"
#include "ui_PlayerStatsWidget.h"
#include "ui_PowerupsWidget.h"

#include "StatsConverter.h"
#include "ASFunctions.h"

#include <QDebug>

PlayerStatsWidget::PlayerStatsWidget(QWidget *parent) : QWidget(parent) { init(); }
PlayerStatsWidget::~PlayerStatsWidget() { delete m_StatsConverter; }

void PlayerStatsWidget::displayStats(const PlayerStats &stats)
{
    m_StatsConverter->convertToText(stats);

    ui->label_Player   ->setText(stats.nickname);
    ui->label_Character->setFrameShape(QFrame::NoFrame);
    ui->label_Character->setPixmap(as::characterPixmap(stats.characterData.type, stats.characterData.mode));

    for (const auto stat : sc::base_stats)
        m_StatsOutput.at(stat)->setValue(m_StatsConverter->stat(stat));

    ui->widget_BonusesStats->displayStats(m_StatsConverter->bonuses());

    bool isExtValid(stats.validationFlags[stats.EXT] == stats.VALID);
    ui->widget_ExtStats->setVisible(isExtValid);

    if (isExtValid)
    {
        auto [n1, n2](as::characterStatsNames(stats.characterData.type));

        m_StatsOutput.at(sc::CHARACTER_STAT_1)->setName(n1);
        m_StatsOutput.at(sc::CHARACTER_STAT_2)->setName(n2);

        for (auto stat : sc::ext_stats)
            m_StatsOutput.at(stat)->setValue(m_StatsConverter->stat(stat));

        if (stats.characterData.type != as::MONO)
        {
            auto powerups(m_StatsConverter->powerups());

            ui->widget_PowerupsStats  ->displayStats(powerups);
            ui->widget_PowerupsStats  ->setVisible(powerups.size());

            auto blocks(m_StatsConverter->blocks());
            for (const auto color : as::colors_array)
            {
                auto iter(blocks.find(color));

                if (iter == blocks.cend())
                {
                    ui->tableWidget_BlocksStats->setColumnHidden(color, true);
                    continue;
                }

                for (size_t i{}; i < 4; ++i)
                {
                    auto label(static_cast<QLabel*>(ui->tableWidget_BlocksStats->cellWidget(i, color)));

                    label->setText      (iter->second[i].text      );
                    label->setStyleSheet(iter->second[i].styleSheet);
                }
            }

            m_StatsOutput.at(sc::BLOCKS_COUNT)->setValue(m_StatsConverter->stat(sc::BLOCKS_COUNT));
            m_StatsOutput.at(sc::TRAFFIC     )->setValue(m_StatsConverter->stat(sc::TRAFFIC     ));
        }
    }
    else
    {
        for (const auto color : as::colors_array)
            for (int i{}, rows(ui->tableWidget_BlocksStats->rowCount()); i < rows; ++i)
                static_cast<QLabel*>(ui->tableWidget_BlocksStats->cellWidget(i, color))->clear();

        m_StatsOutput.at(sc::BLOCKS_COUNT)->setValue({ "N/A" });
        m_StatsOutput.at(sc::TRAFFIC     )->setValue({ "N/A" });
    }
}

void PlayerStatsWidget::setStatsConverter(AbstractStatsConverter *converter) noexcept
{
    delete m_StatsConverter;
    m_StatsConverter = converter;
}

AbstractStatsConverter *PlayerStatsWidget::statsConverter() const noexcept
{
    return m_StatsConverter;
}

void PlayerStatsWidget::reset()
{
    for (auto &statWidget : m_StatsOutput)
        statWidget.second->setDefaultValue();

    m_StatsOutput.at(sc::CHARACTER_STAT_1)->setName("Select stats");
    m_StatsOutput.at(sc::CHARACTER_STAT_2)->setName("Select stats");

    ui->label_Character->setFrameShape(QFrame::Box);
    ui->label_Character->setText("X"              );
    ui->label_Player   ->setText("Select stats"   );

    ui->widget_BonusesStats->reset();

    ui->widget_PowerupsStats->reset();
    ui->widget_PowerupsStats->hide ();

    for (const auto color : as::colors_array)
    {
        ui->tableWidget_BlocksStats->setColumnHidden(color, false);

        for (int i{}, rows(ui->tableWidget_BlocksStats->rowCount()); i < rows; ++i)
            static_cast<QLabel*>(ui->tableWidget_BlocksStats->cellWidget(i, color))->clear();
    }
}


void PlayerStatsWidget::init()
{
    ui = new Ui::PlayerStatsWidget;
    ui->setupUi(this);

    m_StatsConverter = new StatsDefaultConverter;

    for (const auto color : as::colors_array)
    {
        ui->tableWidget_BlocksStats->horizontalHeaderItem(color)->setText(as::colorToString(color));
        for (int i{}, rows(ui->tableWidget_BlocksStats->rowCount()); i < rows; ++i)
        {
            auto label(new QLabel);
            label->setWordWrap(true);
            label->setMargin(5);
            label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            label->setFont(QFont("Bahnschrift", 10));

            ui->tableWidget_BlocksStats->setCellWidget(i, color, label);
        }
    }

    for (int stat{}; stat <= sc::Stat::SKILL_RATING; ++stat)
        m_StatsOutput.insert({ static_cast<sc::Stat>(stat), new StatWidget(static_cast<sc::Stat>(stat)) });

    for (const auto stat : sc::base_stats)
        ui->widget_BaseStats->layout()->addWidget(m_StatsOutput.at(stat));
    for (const auto stat : sc::ext_stats )
        ui->widget_ExtStats ->layout()->addWidget(m_StatsOutput.at(stat));

    m_StatsOutput.insert(m_StatsOutput.cend(), { sc::TRAFFIC     , ui->widget_Traffic     });
    m_StatsOutput.insert(m_StatsOutput.cend(), { sc::BLOCKS_COUNT, ui->widget_BlocksCount });

    ui->widget_Traffic    ->setName(sc::TRAFFIC     );
    ui->widget_BlocksCount->setName(sc::BLOCKS_COUNT);

    ui->widget_Traffic    ->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    ui->widget_BlocksCount->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    ui->tableWidget_BlocksStats->verticalHeader  ()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->tableWidget_BlocksStats->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

    //connect(ui->pushButton, &QPushButton::clicked, this, [=]{ ui->label_Player->setNum(ui->tableWidget_BlocksStats->height()); });
}



PowerupsWidget::PowerupsWidget(QWidget *parent) : QWidget(parent) { init(); }

void PowerupsWidget::displayStats(const ConvertedPowerups &stats)
{
    std::for_each(m_Output.begin(), m_Output.end(), [](auto widget){ widget->hide(); });

    for (const auto&[powerup, data] : stats)
    {
        auto output(qobject_cast<QLabel*>(m_Output[powerup]->children().at(2)));

        output->setText("x" + data.text);
        output->setStyleSheet(data.styleSheet);

        m_Output[powerup]->show();
    }
}

void PowerupsWidget::reset()
{
    std::for_each(m_Output.begin(), m_Output.end(), [](auto widget){ widget->hide(); });
}

void PowerupsWidget::init()
{
    ui = new Ui::PowerupsWidget;
    ui->setupUi(this);

    m_Output[as::STORM     ] = ui->widget_StormsSection     ;
    m_Output[as::PAINT     ] = ui->widget_PaintsSection     ;
    m_Output[as::MULTIPLIER] = ui->widget_MultipliersSection;
    m_Output[as::SORT      ] = ui->widget_SortsSection      ;
}



StatWidget::StatWidget(sc::Stat stat, QWidget *parent) : QWidget(parent)
{
    init();
    setName(stat);
}
StatWidget::StatWidget(QWidget *parent) : QWidget(parent)
{
    init();
}

void StatWidget::setName(sc::Stat stat)
{
    switch (stat)
    {
    case sc::SCORE                : setName("Score"                  ); break;
    case sc::RAW_SCORE            : setName("Raw score"              ); break;
    case sc::DURATION             : setName("Duration"               ); break;
    case sc::OVERFILLS            : setName("Overfils"               ); break;
    case sc::LONGEST_CHAIN        : setName("Longest chain"          ); break;
    case sc::DROPPED_CHAIN        : setName("Dropped chain"          ); break;
    case sc::BEST_CLUSTER         : setName("Best cluster"           ); break;
    case sc::AVG_CLUSTER_SIZE     : setName("Avg. cluster size"      ); break;
    case sc::AVG_CLUSTER_COLORS   : setName("Avg. cluster colors"    ); break;
    case sc::CONS_BLOCKS_COLLECTED: setName("Max cons. blocks hit"   ); break;
    case sc::CONS_BLOCKS_DODGED   : setName("Max cons. blocks dodged"); break;
    case sc::SKILL_RATING         : setName("Skill rating"           ); break;
    case sc::BLOCKS_COUNT         : setName("Total blocks count:"    ); break;
    case sc::TRAFFIC              : setName("Traffic:"               ); break;
    default: reset(); break;
    }
}
void StatWidget::setName(const QString &name)
{
    m_Name->setText(name);
    setDefaultValue();
}
void StatWidget::setValue(const ConvertedStat &stat)
{
    m_Value->setText(stat.text);

    for (auto label : { m_Value, m_Name })
        label->setStyleSheet(stat.styleSheet);
}

void StatWidget::setDefaultValue()
{
    m_Value->setText("N/A");
    for (auto label : { m_Value, m_Name })
        label->setStyleSheet("");
}
void StatWidget::reset()
{
    resetName ();
    resetValue();
}
void StatWidget::resetName()
{
    m_Name->clear();
}
void StatWidget::resetValue()
{
    m_Value->clear();
}

void StatWidget::init()
{
    QFont f("Bahnschrift", 10);

    m_Name  = new QLabel;
    m_Value = new QLabel;

    m_Name ->setFont(f);
    m_Value->setFont(f);

    m_Value->setAlignment(Qt::AlignRight);

    auto horLayout(new QHBoxLayout);
    horLayout->setMargin(0);
    horLayout->setSpacing(5);

    horLayout->addWidget(m_Name );
    horLayout->addWidget(m_Value);

    setLayout(horLayout);
}



BonusesWidget::BonusesWidget(QWidget *parent) : QWidget(parent){ init(); }

void BonusesWidget::displayStats(const ConvertedBonuses &stats)
{
    reset();

    auto node(m_Output.cbegin());
    StatWidget *statWidget;

    for (const auto &[name, data] : stats)
    {
        if (node == m_Output.cend())
        {
            statWidget = new StatWidget;

            layout()->addWidget(statWidget);
            m_Output.push_back(statWidget);

            node = m_Output.cend();
            --node;
        }
        else
            statWidget = *node;

        statWidget->setName (name);
        statWidget->setValue(data);
        statWidget->show();

        ++node;
    }

    std::for_each(node, m_Output.cend(), [](auto statWidget)
    {
        statWidget->reset();
    });
}

void BonusesWidget::reset()
{
    for (auto statWidget : m_Output)
    {
        statWidget->reset();
        statWidget->hide();
    }

    m_Output.front()->setName("No afterride bonuses<br>has been achieved");
    m_Output.front()->resetValue();
    m_Output.front()->show();
}

void BonusesWidget::init()
{
    auto verLayout(new QVBoxLayout);
    verLayout->setMargin(0);
    verLayout->setSpacing(2);

    m_Output.push_back(new StatWidget);
    verLayout->addWidget(m_Output.front());

    setLayout(verLayout);

    reset();
}














