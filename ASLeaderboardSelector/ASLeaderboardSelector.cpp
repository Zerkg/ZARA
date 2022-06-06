#include "ASLeaderboardSelector.h"
#include "ui_ASLeaderboardSelector.h"


#include "ASFunctions.h"
#include "ASLeaderboard.h"
#include "ASPhpQuery.h"


ASLeaderboardSelector::ASLeaderboardSelector(QWidget *parent) noexcept
    : QWidget(parent)
    , ui(new Ui::ASLeaderboardSelector)
    , m_LeaderboardQuery(new ASPhpLeaderboardQuery(-1, this))
{
    ui->setupUi(this);

    QObject::connect(ui->comboBox_Mode , &QComboBox::currentTextChanged, this, &ASLeaderboardSelector::displayModeData );
    QObject::connect(m_LeaderboardQuery, &ASPhpQuery::dataProcessed    , this, &ASLeaderboardSelector::processQueryData);
}

ASLeaderboardSelector::~ASLeaderboardSelector() noexcept
{
    delete ui;
}

const ASLeaderboard *ASLeaderboardSelector::leaderboard() const noexcept
{
    return m_CurrentLeaderboard;
}
const ASLeaderboard *ASLeaderboardSelector::leaderboard(as::Mode mode) const noexcept
{
    return m_LeaderboardQuery->getLeaderboard(mode);
}

as::Mode ASLeaderboardSelector::currentMode() const noexcept
{
    return *m_CurrentLeaderboard;
}

void ASLeaderboardSelector::loadLeaderboard(int id) noexcept
{
    if (id < 0) return;

    m_LeaderboardQuery->setLeaderboardID(id);
    m_LeaderboardQuery->execute();
}

void ASLeaderboardSelector::reset() noexcept
{
    ui->comboBox_Mode->setEnabled(false);
    ui->comboBox_Mode->clear();
    ui->label_Mode->setEnabled(false);
    ui->label_PlayersCount->setText("Leaderboard is empty");
}
void ASLeaderboardSelector::displayModeData(const QString &mode) noexcept
{
    if (mode.isEmpty()) return;

    as::Mode m(as::stringToMode(mode));

    m_CurrentLeaderboard = m_LeaderboardQuery->getLeaderboard(m);
    ui->label_PlayersCount->setText("Players: " + QString::number(m_CurrentLeaderboard->size()));

    emit modeChanged(m);
}

void ASLeaderboardSelector::processQueryData() noexcept
{
    reset();

    for (auto mode : as::modes_array)
        if (m_LeaderboardQuery->getLeaderboard(mode)->size() > 0)
            ui->comboBox_Mode->addItem(as::modeToString(mode));

    if (ui->comboBox_Mode->count())
    {
        ui->comboBox_Mode->setEnabled(true);
        ui->label_Mode   ->setEnabled(true);
    }
    emit leaderboardChanged();
}

