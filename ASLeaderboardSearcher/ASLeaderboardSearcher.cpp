#include "ASLeaderboardSearcher.h"
#include "ui_ASLeaderboardSearcher.h"

#include <QUrl>
#include <QMessageBox>

#include "ASFunctions.h"
#include "ASPhpQuery.h"
#include "SearchHistoryWidget.h"


ASLeaderboardSearcher::ASLeaderboardSearcher(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ASLeaderboardSearcher)
{
    ui->setupUi(this);
    init();
}

ASLeaderboardSearcher::~ASLeaderboardSearcher()
{
    delete ui;
    delete m_History;
}

void ASLeaderboardSearcher::init()
{
    auto SelectLeaderboardID([=](int id)
    {
        if (id >= 0)
        {
            m_LeaderboardId = m_SongsIDs[id];
            emit songSelected(m_LeaderboardId);
        }
    });

    auto dispatchID([=](int id)
    {
        if (m_LeaderboardId != id)
        {
            ui->widget_SearchResults->hide();
            m_LeaderboardId = id;
            emit songSelected(id);
        }
    });

    m_History = new SearchHistoryWidget(this);
    m_History->setWindowFlag(Qt::Window);
    m_History->hide();

    ui->widget_SearchResults->hide();

    m_LeaderboardId = 0;

    QObject::connect(ui->radioButton_Song     , &QRadioButton::toggled                   , this     , [=](){ui->lnEdit_Input->setPlaceholderText("Input a song name here..."   );});
    QObject::connect(ui->radioButton_Artist   , &QRadioButton::toggled                   , this     , [=](){ui->lnEdit_Input->setPlaceholderText("Input an artist name here...");});
    QObject::connect(ui->radioButton_URL      , &QRadioButton::toggled                   , this     , [=](){ui->lnEdit_Input->setPlaceholderText("Input a song URL here..."    );});

    QObject::connect(ui->pshBut_ShowHistory   , &QPushButton::clicked                    , m_History, &SearchHistoryWidget::show                                                  );
    QObject::connect(m_History                , &SearchHistoryWidget::leaderboardSelected, this     , dispatchID                                                                  );

    QObject::connect(ui->pshBut_SearchURL     , &QPushButton::clicked                    , this     , &ASLeaderboardSearcher::search                                              );
    QObject::connect(ui->lnEdit_Input         , &QLineEdit::returnPressed                , this     , [=](){ ui->comboBox_FoundedNames->clear(); search();                       });
    QObject::connect(ui->lnEdit_Input         , &QLineEdit::textChanged                  , this     , [=](const QString &t){ ui->pshBut_SearchURL->setDisabled(t.isEmpty());      });

    QObject::connect(ui->comboBox_FoundedNames, QOverload<int>::of(&QComboBox::activated), this     , SelectLeaderboardID                                                         );
    QObject::connect(this                     , &ASLeaderboardSearcher::songSelected     , this     , &ASLeaderboardSearcher::saveInHistory                                       );
}

void ASLeaderboardSearcher::search()
{
    ui->widget_SearchResults->hide();

    QString input(ui->lnEdit_Input->text());
    if (input.isEmpty()) return;

    if (ui->radioButton_URL->isChecked())
    {
        if (input.contains("song.php"))
            emit songSelected(as::intFromString(input));
        else
        {
            QMessageBox::warning(nullptr, "Warning", "Specified URL is not an Audiosurf leaderboard URL. Search aborted.");
            return;
        }
    }
    else
    {
        ASPhpGetSongsQuery *query;

        if (ui->radioButton_Artist->isChecked()) query = new ASPhpSongsQuery  ;
        else                                     query = new ASPhpArtistsQuery;

        query->setSearchingTarget(QUrl::toPercentEncoding(input));

        QObject::connect(query, &ASPhpQuery::dataProcessed, this, &ASLeaderboardSearcher::displaySearchResults);
        query->execute();
    }
}

void ASLeaderboardSearcher::displaySearchResults()
{
    ASPhpGetSongsQuery *reply(dynamic_cast<ASPhpGetSongsQuery*>(sender()));

    QString dest;
    if (ui->radioButton_Artist->isChecked()) dest.append("song"  );
    else                                     dest.append("artist");

    if (!reply->isSuccessfull())
    {
        ui->label_Title->setText("No " + dest + "s found");
        return;
    }

    m_SongsIDs.clear();
    ui->comboBox_FoundedNames->clear();

    foreach(const auto& artistData, reply->getSearchResult())
    {
        m_SongsIDs.push_back(artistData.songID);
        ui->comboBox_FoundedNames->addItem(artistData.name);
    }

    ui->label_Title     ->setText("Select " + dest);
    ui->label_ItemsFound->setText("Founded " + QString::number(m_SongsIDs.size()) + ' ' + dest + "s:");

    ui->widget_SearchResults->show();
}

void ASLeaderboardSearcher::saveInHistory(int leaderboardID)
{
    ASPhpLeaderboardQuery lQuery(leaderboardID);

    lQuery.execute();
    lQuery.waitForFinished();

    HistoryRecord hr;

    hr.dateTime = QDateTime::currentDateTime();
    auto header(lQuery.getHeader());
    hr.artist   = header.artistName;
    hr.songName = header.songName  ;
    hr.songID   = leaderboardID;

    m_History->addToHistory(hr);

    ui->label_Title->setText(hr.artist + " - " + hr.songName);
}
