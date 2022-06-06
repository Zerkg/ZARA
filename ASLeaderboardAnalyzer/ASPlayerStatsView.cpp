#include "ASPlayerStatsView.h"
#include "ui_ASPlayerStatsView.h"

#include "ASCharacter.h"
#include "ASAfterrideBonus.h"
#include "ASFunctions.h"
#include "ASPlayerStatsModel.h"

//#include "TableModel.h"

#include <QStringListModel>
//#include <QDebug>

ASPlayerStatsView::ASPlayerStatsView(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ASPlayerStatsView)
{
    ui->setupUi(this);
    init();
}

ASPlayerStatsView::~ASPlayerStatsView()
{
    delete ui;
}

ASAbstractPlayerStatsModel *ASPlayerStatsView::getModel()
{
    return m_PlayerStatsModel;
}

void ASPlayerStatsView::setModel(ASAbstractPlayerStatsModel *model)
{
    m_PlayerStatsModel = model;

    ui->table_BlocksStats  ->setModel(m_PlayerStatsModel->blocksStatsModel());
    ui->treeView_ScoreStats->setModel(m_PlayerStatsModel->scoreStatsModel ());

    ui->treeView_ScoreStats->setColumnWidth(0, 161);
    ui->treeView_ScoreStats->setColumnWidth(1, 73);

    QObject::connect(m_PlayerStatsModel, &ASPlayerStatsModel::dataChanged, this, &ASPlayerStatsView::updateView);
}

void ASPlayerStatsView::setPlayersList(QStringListModel *model)
{
    ui->cmbBox_PlayersList->setModel(model);
}

void ASPlayerStatsView::updateView()
{
    if (m_PlayerStatsModel == nullptr) return;

    auto [traffic, blocks, duration, character](m_PlayerStatsModel->additionalData());

    if (character != nullptr)
    {
        ui->lbl_Character->setPixmap    (as::characterPixmap(character->getType(), character->getMode()));
        ui->lbl_Character->setFrameStyle(QFrame::Shape::NoFrame);

        if (character->getType() == as::MONO)
        {
            ui->spBox_BlocksCount ->clear();
            ui->spBox_TrackTraffic->clear();

            ui->treeView_ScoreStats->setRowHidden(ASPlayerStatsModel::SHTORMS_USED - 1, QModelIndex(), true);
        }
        else
        {
            ui->spBox_BlocksCount ->setValue(blocks );
            ui->spBox_TrackTraffic->setValue(traffic);

            ui->treeView_ScoreStats->setRowHidden(ASPlayerStatsModel::SHTORMS_USED - 1, QModelIndex(), false);
        }
    }

    if (duration.isValid())
        ui->tmEdit_TrackDuration->setTime (duration);

    QAbstractItemModel *stats(ui->treeView_ScoreStats->model());

    ui->treeView_ScoreStats->setRowHidden(ASPlayerStatsModel::BONUS_IRONMODE  , QModelIndex(), !stats->data(stats->index(ASPlayerStatsModel::BONUS_IRONMODE  , 1), Qt::EditRole).toBool());
    ui->treeView_ScoreStats->setRowHidden(ASPlayerStatsModel::BONUS_CF        , QModelIndex(), !stats->data(stats->index(ASPlayerStatsModel::BONUS_CF        , 1), Qt::EditRole).toBool());
    ui->treeView_ScoreStats->setRowHidden(ASPlayerStatsModel::BONUS_MATCH     , QModelIndex(), !stats->data(stats->index(ASPlayerStatsModel::BONUS_MATCH     , 1), Qt::EditRole).toBool());
    ui->treeView_ScoreStats->setRowHidden(ASPlayerStatsModel::BONUS_STEALTH   , QModelIndex(), !stats->data(stats->index(ASPlayerStatsModel::BONUS_STEALTH   , 1), Qt::EditRole).toBool());
    ui->treeView_ScoreStats->setRowHidden(ASPlayerStatsModel::BONUS_BN        , QModelIndex(), !stats->data(stats->index(ASPlayerStatsModel::BONUS_BN        , 1), Qt::EditRole).toBool());
    ui->treeView_ScoreStats->setRowHidden(ASPlayerStatsModel::BONUS_SR        , QModelIndex(), !stats->data(stats->index(ASPlayerStatsModel::BONUS_SR        , 1), Qt::EditRole).toBool());

    ui->treeView_ScoreStats->setRowHidden(ASPlayerStatsModel::SHTORMS_USED    , QModelIndex(), !stats->data(stats->index(ASPlayerStatsModel::SHTORMS_USED    , 1), Qt::DisplayRole).toBool());
    ui->treeView_ScoreStats->setRowHidden(ASPlayerStatsModel::PAINTS_USED     , QModelIndex(), !stats->data(stats->index(ASPlayerStatsModel::PAINTS_USED     , 1), Qt::DisplayRole).toBool());
    ui->treeView_ScoreStats->setRowHidden(ASPlayerStatsModel::MULTIPLIERS_USED, QModelIndex(), !stats->data(stats->index(ASPlayerStatsModel::MULTIPLIERS_USED, 1), Qt::DisplayRole).toBool());
    ui->treeView_ScoreStats->setRowHidden(ASPlayerStatsModel::SORTS_USED      , QModelIndex(), !stats->data(stats->index(ASPlayerStatsModel::SORTS_USED      , 1), Qt::DisplayRole).toBool());

    ui->table_BlocksStats  ->viewport()->update();
    ui->treeView_ScoreStats->viewport()->update();
}


void ASPlayerStatsView::init()
{
    m_PlayerStatsModel = nullptr;

    ui->table_BlocksStats->verticalHeader  ()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->table_BlocksStats->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

    QObject::connect(ui->cmbBox_PlayersList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ASPlayerStatsView::currentPlayerChanged);
}
