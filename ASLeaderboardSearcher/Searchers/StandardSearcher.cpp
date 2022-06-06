#include "StandardSearcher.h"
#include "ui_StandardSearcher.h"

#include "PlayerStats.h"
#include "ASFunctions.h"
#include "ASPhpQuery.h"

#include "DataTypes.h"

#include <QCompleter>


StandardSearcher::StandardSearcher(QWidget *parent) : QWidget(parent) { init(); }
StandardSearcher::~StandardSearcher() { delete ui; }

void StandardSearcher::setCompleter(QCompleter* c)
{
    c->setCaseSensitivity(Qt::CaseInsensitive);

    ui->lnEdit_Input->setCompleter(c);
}

void StandardSearcher::search()
{    
    QString target(ui->lnEdit_Input->text());

    if (target.isEmpty())
        return;

    if (int urlID(parseUrl(target)); urlID != -1)
    {
        ui->searchResultsBox->hide();
        dispatchUrlId(urlID);
    }
    else
    {
        m_SongsSearcher  ->setSearchingTarget(target);
        m_ArtistsSearcher->setSearchingTarget(target);

        m_SongsSearcher  ->execute();
        m_ArtistsSearcher->execute();

        ui->searchResultsBox->show();
    }
}

void StandardSearcher::init()
{
    ui = new Ui::StandardSearcher;
    ui->setupUi(this);

    m_SongsSearcher   = new ASPhpSongsQuery  (QString(), this);
    m_ArtistsSearcher = new ASPhpArtistsQuery(QString(), this);

    ui->searchResultsBox->hide();
    ui->searchResultsBox->setSource(m_ArtistsSearcher, m_SongsSearcher);

    connect(ui->lnEdit_Input      , &QLineEdit::returnPressed        , this, &StandardSearcher::search);
    connect(ui->pshBut_SearchURL  , &QPushButton::clicked            , this, &StandardSearcher::search);

    connect(ui->searchResultsBox  , &SearchResultsBox::resultSelected, this, &StandardSearcher::dispatchResultId);
}

int StandardSearcher::parseUrl(const QString &url) const
{
    if (!url.contains("song.php"))
        return -1;

    bool ok;
    int urlId(url.rightRef(url.size() - url.lastIndexOf('#') - 1).toInt(&ok));

    if (ok)
        return urlId;
    else
        return -1;
}
void StandardSearcher::dispatchUrlId(int sId)
{
    ASPhpLeaderboardQuery query(sId, this);

    query.execute();
    query.waitForFinished();

    auto title(query.getSongTitle());

    SearchRecord record;

    record.searchDateTime = QDateTime::currentDateTime();
    record.songID         = sId;
    record.artist         = title.artistName;
    record.songName       = title.songName;

    emit resultSelected(record);
}
void StandardSearcher::dispatchResultId(const SearchData &data, int type)
{
    SongTitle title{ data.name, ui->lnEdit_Input->text() };

    if (type == SearchResultsBox::SONG)
        std::swap(title.artistName, title.songName);

    SearchRecord record;

    record.searchDateTime = QDateTime::currentDateTime();
    record.songID         = data .songID;
    record.artist         = title.artistName;
    record.songName       = title.songName;

    emit resultSelected(record);
}











