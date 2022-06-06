#include "ASLeaderboardSearcher.h"
#include "ui_ASLeaderboardSearcher.h"

#include "DataTypes.h"

#include "History/HistoryModel.h"
#include "History/HistoryView.h"
#include "History/HistoryCompleter.h"

ASLeaderboardSearcher::ASLeaderboardSearcher(QWidget *parent) : QWidget(parent) { init(); }
ASLeaderboardSearcher::~ASLeaderboardSearcher()
{
    m_HistoryModel->saveHistory();
    delete ui;
}

void ASLeaderboardSearcher::init()
{
    ui = new Ui::ASLeaderboardSearcher;
    ui->setupUi(this);

    m_HistoryModel = new HistoryModel("Data/hr.dat", this);

    HistoryView* historyView(new HistoryView(this));

    historyView->setModel(m_HistoryModel);
    historyView->setWindowFlag    (Qt::Window     );
    historyView->setWindowModality(Qt::WindowModal);
    historyView->hide();

    CompleterManager *manager(new CompleterManager(this));
    manager->setHistoryModel(m_HistoryModel);

    ui->widget_Searcher->setCompleter(manager->completer());

    connect(ui->pshBut_ShowHistory, &QPushButton::clicked             , historyView, &HistoryView::show                      );
    connect(historyView           , &HistoryView::recordSelected      , this       , &ASLeaderboardSearcher::saveSearchResult);
    connect(ui->widget_Searcher   , &StandardSearcher::resultSelected , this       , &ASLeaderboardSearcher::saveSearchResult);

    startTimer(300000);
}

void ASLeaderboardSearcher::timerEvent(QTimerEvent *event)
{
    killTimer(event->timerId());
    m_HistoryModel->saveHistory();

    startTimer(300000);
}

void ASLeaderboardSearcher::saveSearchResult(const SearchRecord &sr)
{
    emit songSelected(sr.songID);

    m_HistoryModel->addRecord(sr);
    ui->label_Title->setText(sr.artist + " - " + sr.songName);
}
