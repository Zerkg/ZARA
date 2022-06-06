#include "ASScoreSelector.h"
#include "ui_ASScoreSelector.h"

#include "ASFunctions.h"
#include "PlayerStats.h"
#include "ASPhpQuery.h"

#include <QMenu>
#include <QContextMenuEvent>
#include <QDesktopServices>

ASScoreSelector::ASScoreSelector(QWidget *parent) noexcept
    : QWidget(parent)
    , ui(new Ui::ASScoreSelector)
    , m_SelectedItemsCount(0)
    , m_MaxSelectedItemsCount(30)
    , m_LeaderboardQuery(new ASPhpLeaderboardQuery(-1, this))
    , m_Leaderboards(3)
{
    ui->setupUi(this);

    m_Leaderboards[as::CASUAL] = ui->treeWidget_Casual;
    m_Leaderboards[as::PRO   ] = ui->treeWidget_Pro   ;
    m_Leaderboards[as::ELITE ] = ui->treeWidget_Elite ;

    for (auto mode : as::modes_array)
    {
        QString colorHex(QColor::fromRgb(as::modes_colors_array[mode]).name(QColor::HexRgb));

        m_Leaderboards[mode]->setColumnWidth(0, 21);
        m_Leaderboards[mode]->setStyleSheet("* {background: qlineargradient( x1:0 y1:0, x2:0 y2:1, stop:0 white, stop:1 " + colorHex + ");}");
        m_Leaderboards[mode]->installEventFilter(this);

        connect(m_Leaderboards[mode], &QTreeWidget::itemClicked, this, &ASScoreSelector::onItemClick);
    }

    connect(m_LeaderboardQuery, &ASPhpQuery::dataProcessed, this, &ASScoreSelector::processQueryData);
}

ASScoreSelector::~ASScoreSelector() noexcept
{
    delete ui;
}

void ASScoreSelector::setSelectionLimit(quint8 limit) noexcept
{
    m_MaxSelectedItemsCount = limit;
}

const ASPhpLeaderboardQuery *ASScoreSelector::source() const noexcept
{
    return m_LeaderboardQuery;
}

void ASScoreSelector::loadLeaderboard(int id) noexcept
{
    if (id < 0)
        return;

    reset();

    m_LeaderboardId = id;

    m_LeaderboardQuery->setLeaderboardID(id);
    m_LeaderboardQuery->execute();
}

void ASScoreSelector::reset() noexcept
{    
    for (auto mode : as::modes_array)
    {
        auto selectedItems(m_Leaderboards[mode]->selectedItems());

        for (auto item : selectedItems)
            emit scoreDeselected(m_LeaderboardQuery->leaderboard(mode)->at(m_Leaderboards[mode]->indexOfTopLevelItem(item)));

        m_Leaderboards[mode]->clear();
    }

    m_SelectedItemsCount = 0;
}

bool ASScoreSelector::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::ContextMenu)
    {
        auto contextMenuEvent(static_cast<QContextMenuEvent*>(event));
        QMenu menu(this);

        QList<QAction*> deselectActions;

        for (const auto mode : as::modes_array)
            if (!m_Leaderboards[mode]->selectedItems().isEmpty())
            {
                deselectActions.append(new QAction(as::modeToString(mode), &menu));
                connect(deselectActions.last(), &QAction::triggered, this, [mode, this]
                {
                    foreach (auto item, m_Leaderboards[mode]->selectedItems())
                    {
                        item->setSelected(false);
                        --m_SelectedItemsCount;

                        emit scoreDeselected(m_LeaderboardQuery->leaderboard(mode)->at(m_Leaderboards[mode]->indexOfTopLevelItem(item)));
                    }
                });
            }

        if (deselectActions.size())
        {
            auto deselectMenu(new QMenu("Deselect", &menu));
            deselectMenu->addActions(deselectActions);
            if (deselectActions.size() > 1)
            {
                deselectMenu->addSeparator();
                connect(deselectMenu->addAction("All"), &QAction::triggered, this, [&deselectActions]
                {
                    for (auto action : qAsConst(deselectActions))
                        action->trigger();
                });
            }
            menu.addMenu(deselectMenu);
        }

        if (auto index(m_Leaderboards.indexOf(static_cast<QTreeWidget*>(obj))); index != -1)
        {
            menu.addSeparator();

            auto item(m_Leaderboards[index]->itemAt(contextMenuEvent->pos()));
            if (item != nullptr)
                connect(menu.addAction("Open Audiosurf profile"), &QAction::triggered, this, [item]
                {
                    QUrl profileURL("http://www.audio-surf.com/mypage.php?u=" + item->text(1));
                    QDesktopServices::openUrl(profileURL);
                });
        }

        if (!menu.actions().isEmpty())
            menu.exec(contextMenuEvent->globalPos());

        return true;
    }
    return false;
}
void ASScoreSelector::onItemClick(QTreeWidgetItem *item)
{
    const bool isSelected(item->isSelected());

    if (!isSelected + m_SelectedItemsCount > m_MaxSelectedItemsCount)
        return;

    PlayerStats stats;
    for (auto mode : as::modes_array)
    {
        if (item->treeWidget() == m_Leaderboards[mode])
        {
            int itemIndex(m_Leaderboards[mode]->indexOfTopLevelItem(item));
            stats = m_LeaderboardQuery->leaderboard(mode)->at(itemIndex);

            break;
        }
    }

    if (isSelected)
    {
        --m_SelectedItemsCount;
        emit scoreDeselected(stats);
    }
    else
    {
        ++m_SelectedItemsCount;
        emit scoreSelected(stats);
    }

    item->setSelected(!isSelected);
}

void ASScoreSelector::processQueryData() noexcept
{
    for (auto mode : m_LeaderboardQuery->availableLeaderboards())
    {
        auto leaderboard(m_LeaderboardQuery->leaderboard(mode));
        for (const auto &stats : *leaderboard)
        {
            QTreeWidgetItem *pScore(new QTreeWidgetItem);

            pScore->setText(0, QString::number(m_Leaderboards[mode]->topLevelItemCount() + 1) + ".");
            pScore->setData(0, Qt::TextAlignmentRole, static_cast<int>(Qt::AlignRight | Qt::AlignVCenter));
            pScore->setIcon(1, as::characterPixmap(stats.characterData.type, stats.characterData.mode));
            pScore->setText(1, stats.nickname);
            pScore->setData(2, Qt::TextAlignmentRole, static_cast<int>(Qt::AlignRight | Qt::AlignVCenter));
            pScore->setText(2, as::commaPrint(stats.score));

            m_Leaderboards[mode]->addTopLevelItem(pScore);
        }

        for (size_t i{}, columnsCount(m_Leaderboards[mode]->columnCount()); i < columnsCount; ++i)
            m_Leaderboards[mode]->resizeColumnToContents(i);
    }
    emit leaderboardLoaded(m_LeaderboardId);
}
