#include "ASPhpQuery.h"

#include <QCoreApplication>
#include <QNetworkReply>

#include "ASFunctions.h"
#include "ASAfterrideBonus.h"

#include "PlayerStats.h"

//========================================================:: ASPhpQuery ::==========================================================

ASPhpQuery::ASPhpQuery(const QString &query, QObject *parent) noexcept
    : QObject(parent)
    , m_ASWebsiteConnection(new QNetworkAccessManager(this))
    , m_QueryString(query){}


void ASPhpQuery::execute() noexcept
{
    m_IsSuccessfull = false;
    m_IsFinished    = false;

    QNetworkRequest request(m_QueryString);
    QObject::connect(m_ASWebsiteConnection->get(request), &QNetworkReply::finished, this, &ASPhpQuery::updateData);
}

void ASPhpQuery::waitForFinished() const noexcept
{
    while (!m_IsFinished)
        QCoreApplication::processEvents();
}

bool ASPhpQuery::isSuccessfull() const noexcept
{
    return m_IsSuccessfull;
}

bool ASPhpQuery::errorCheck() noexcept
{
    return !m_PreparedReply.isEmpty();
}

void ASPhpQuery::updateData() noexcept
{
    QNetworkReply *reply(qobject_cast<QNetworkReply*>(sender()));

    if (reply->error() == QNetworkReply::NoError)
    {        
        m_RawReplyData = reply->readAll();
        m_PreparedReply = QString(m_RawReplyData).split('\n');

        if (errorCheck())
            parse();

        m_IsFinished = true;
        emit dataProcessed();
    }

    reply->deleteLater();
}



//======================================================:: ASPhpStatsQuery ::=======================================================

ASPhpStatsQuery::ASPhpStatsQuery(PlayerStats *output, QObject *parent) noexcept
    : ASPhpQuery(QString(), parent)
    , m_Output(output)
{}
void ASPhpStatsQuery::setRideID(const QString &rid) noexcept
{
    m_RideId = rid;
    m_QueryString = m_URLTemplate.arg(rid);
}
void ASPhpStatsQuery::setOutput(PlayerStats *output) noexcept
{
    m_Output = output;
}



//====================================================:: ASPhpBaseStatsQuery ::=====================================================

ASPhpBaseStatsQuery::ASPhpBaseStatsQuery(PlayerStats *output, const QString &rid, QObject *parent) noexcept
    : ASPhpStatsQuery(output, parent)
{
    m_URLTemplate = "http://www.audio-surf.com/ext/song_getScoreDetail.php?rid=%1";

    if (!rid.isEmpty())
        setRideID(rid);
}
void ASPhpBaseStatsQuery::parse() noexcept
{      
    m_Output->validationFlags[PlayerStats::BASE] = PlayerStats::PROCESSING;

    m_Output->bonuses.reset();

    QString duration(QString::number(as::parseInt(m_PreparedReply[5], ":")));
    duration.insert(duration.size() - 2, ':');

    m_Output->trackDuration = QTime::fromString(duration, duration.size() == 5 ? "mm:ss" : "m:ss");

    for (int i{8}; m_PreparedReply[i].size() != 21; ++i)
    {
        switch(m_PreparedReply[i][m_PreparedReply[i].size() - 9].toLatin1())
        {
        case 'd': m_Output->bonuses.setBonus(as::IRONMODE    ); continue;

        case 's': m_Output->bonuses.setBonus(as::CLEAN_FINISH); continue;

        case 'h': m_Output->bonuses.setBonus(as::MATCH_7     ); continue;
        case '1': m_Output->bonuses.setBonus(as::MATCH_11    ); continue;
        case '2': m_Output->bonuses.setBonus(as::MATCH_21    ); continue;

        case 't': m_Output->bonuses.setBonus(as::STEALTH     ); continue;

        case 'e': m_Output->bonuses.setBonus(as::SEEING_RED  ); continue;
        case 'j': m_Output->bonuses.setBonus(as::BUTTER_NINJA); continue;

        default: continue;
        }
    }

    m_Output->validationFlags[PlayerStats::BASE] = PlayerStats::VALID;
    m_IsSuccessfull = true;
}




//====================================================:: ASPhpExtStatsQuery ::======================================================

ASPhpExtStatsQuery::ASPhpExtStatsQuery(PlayerStats *output, const QString &rid, QObject *parent) noexcept
    : ASPhpStatsQuery(output, parent)
{
    m_URLTemplate = "http://www.audio-surf.com/ext/song_getExtendedScoreDetail.php?rid=%1";

    if (!rid.isEmpty())
        setRideID(rid);
}
void ASPhpExtStatsQuery::parse() noexcept
{    
    // Too old scores check; Too old scores have no extended stats (https://www.audio-surf.com/ext/song_getExtendedScoreDetail.php?rid=23721.930.2837793)
    if (!m_PreparedReply[13].contains(QRegExp("[0-9]")))
    {
        m_Output->validationFlags[PlayerStats::EXT] = PlayerStats::INVALID;
        return;
    }

    m_Output->validationFlags[PlayerStats::EXT] = PlayerStats::PROCESSING;

    m_Output->skillRating = as::parseInt(m_PreparedReply[13]);

    m_Output->characterData.stats[0] = as::parseInt   (m_PreparedReply[3 ]);
    m_Output->characterData.stats[1] = as::parseInt   (m_PreparedReply[4 ]);
    m_Output->overfillsCount         = as::parseInt   (m_PreparedReply[5 ]);
    m_Output->longestChainTime       = as::parseInt   (m_PreparedReply[6 ]);
    m_Output->droppedChains          = as::parseInt   (m_PreparedReply[7 ]);
    m_Output->bestCluster            = as::parseInt   (m_PreparedReply[8 ]);
    m_Output->avgClusterSize         = as::parseDouble(m_PreparedReply[9 ]);
    m_Output->avgColorsCount         = as::parseDouble(m_PreparedReply[10]);
    m_Output->maxConsBlocksDodged    = as::parseInt   (m_PreparedReply[11]);
    m_Output->maxConsBlocksCollected = as::parseInt   (m_PreparedReply[12]);

    if (m_PreparedReply.size() > 50)
    {
        int tableData[]{31, 40, 49};         //Table data are indexes of each block stat (collected n, derived % and collected %)

        for (auto color : as::colors_array)
            for (int stat{}; stat < 3; ++stat)
                m_Output->blocksStats[color][stat] = as::parseInt(m_PreparedReply[tableData[stat] + color]);

        m_Output->powerups[as::PAINT     ] = as::parseInt(m_PreparedReply[60]);
        m_Output->powerups[as::STORM     ] = as::parseInt(m_PreparedReply[61]);
        m_Output->powerups[as::MULTIPLIER] = as::parseInt(m_PreparedReply[62]);
        m_Output->powerups[as::SORT      ] = as::parseInt(m_PreparedReply[63]);
    }

    m_Output->validationFlags[PlayerStats::EXT] = PlayerStats::VALID;
    m_IsSuccessfull = true;
}



//===================================================:: ASPhpLeaderboardQuery ::====================================================

ASPhpLeaderboardQuery::ASPhpLeaderboardQuery(int leaderboardID, QObject *parent) noexcept
    : ASPhpQuery(QString(), parent)
{
    m_URLTemplate = "http://www.audio-surf.com/ext/song_getSong.php?sid=%1";

    if (leaderboardID > 0)
        m_QueryString = m_URLTemplate.arg(leaderboardID);
}


void ASPhpLeaderboardQuery::reset() noexcept
{
    m_Leaderboards.clear();
}

SongTitle ASPhpLeaderboardQuery::getSongTitle() const noexcept
{
    return m_SongTitle;
}

void ASPhpLeaderboardQuery::setLeaderboardID(int id) noexcept
{
    m_QueryString = m_URLTemplate.arg(id);
}

const ASPhpLeaderboardQuery::LeaderboardData* ASPhpLeaderboardQuery::leaderboard(as::Mode mode) const
{
    auto iter(m_Leaderboards.find(mode));

    if (iter != m_Leaderboards.cend())
        return &iter->second;
    else
        return nullptr;
}

std::set<as::Mode> ASPhpLeaderboardQuery::availableLeaderboards() const
{
    std::set<as::Mode> modes;
    for (const auto &pair : m_Leaderboards)
        modes.insert(modes.cend(), pair.first);

    return modes;
}

void ASPhpLeaderboardQuery::parse() noexcept
{
    reset();

    int line(12);
    for (auto mode : as::modes_array)
    {
        while (m_PreparedReply[++line].contains("id="))
        {
            PlayerStats pStats;

            pStats.characterData.mode = mode;

            QStringRef character(&m_PreparedReply[line], m_PreparedReply[line].indexOf("t/") + 2, 2);

            switch (character[0].unicode())
            {
            case 'm':
            case 'n': pStats.characterData.type = as::MONO  ; break;
            case 'd': pStats.characterData.type = as::DV    ; break;
            case 'e': pStats.characterData.type = as::ERASER; break;
            case 'v': pStats.characterData.type = as::VEGAS ; break;
            case 'p':
                if (character[1] == 'o')
                    pStats.characterData.type = as::POINTMAN;
                else
                    pStats.characterData.type = as::PUSHER;
                break;
            }

            pStats.rID       = m_PreparedReply[line++].section('"', 3, 3);
            pStats.nickname  = m_PreparedReply[line++].section('"', 3, 3);
            pStats.score     = as::parseInt(m_PreparedReply[line++], ",");

            m_Leaderboards[mode].emplace_back(std::move(pStats));

            ASPhpQuery *base(new ASPhpBaseStatsQuery(&m_Leaderboards[mode].back(), m_Leaderboards[mode].back().rID, this));
            ASPhpQuery *ext (new ASPhpExtStatsQuery (&m_Leaderboards[mode].back(), m_Leaderboards[mode].back().rID, this));

            for (auto query : { base, ext })
            {
                QObject::connect(query, &ASPhpQuery::dataProcessed, query, &QObject::deleteLater);
                query->execute();
            }
        }

        line += (12 + mode * 2);
    }

    int start(1 + m_PreparedReply[1].indexOf(QRegExp(">[^<]"   )));
    int end  (    m_PreparedReply[1].indexOf(QRegExp("[^>\\s]<")));

    m_SongTitle.songName   = as::fixName(QStringRef(&m_PreparedReply[1], start, end - start + 1).toString());
    m_SongTitle.artistName = as::fixName(m_PreparedReply[1].section('\'', 1, 1));

    m_IsSuccessfull = true;
}



//=====================================================:: ASPhpGetSongsQuery ::======================================================

ASPhpGetSongsQuery::ASPhpGetSongsQuery(QObject *parent) noexcept : ASPhpQuery(QString(), parent) {}

const ASPhpGetSongsQuery::Data* ASPhpGetSongsQuery::searchResults() const noexcept
{
    return &m_SearchResults;
}

void ASPhpGetSongsQuery::setSearchingTarget(const QString &name) noexcept
{
    QUrl sT(m_URLTemplate.arg(name));
    m_QueryString = sT.toEncoded();
}

SearchData ASPhpGetSongsQuery::searchResult(size_t index) const noexcept
{
    return m_SearchResults[index];
}
SearchData ASPhpGetSongsQuery::searchResult(const QString &itemName) const noexcept
{
    for (const auto &result : m_SearchResults)
        if (result.name == itemName)
            return result;

    return {};
}

bool ASPhpGetSongsQuery::errorCheck() noexcept
{
    if (m_PreparedReply.size())
    {
        if (!m_PreparedReply[0].size() || m_PreparedReply[0].contains("No songs "))
            return false;
        else
            return true;
    }
    else
        return false;
}



//======================================================:: ASPhpSongsQuery ::===============================-=======================

ASPhpSongsQuery::ASPhpSongsQuery(const QString &artistName, QObject *parent) noexcept
    : ASPhpGetSongsQuery(parent)
{
    m_URLTemplate = "http://www.audio-surf.com/ext/song_search.php?a=%1";
    if (!artistName.isEmpty())
        setSearchingTarget(artistName);
}

SongTitle ASPhpSongsQuery::songTitle(int index)
{
    SongTitle title;

    title.songName   = m_SearchResults[index].name;
    QUrl artist(m_QueryString.rightRef(m_QueryString.size() - m_QueryString.indexOf('=') - 1).toString());

    title.artistName = as::fixName(artist.toString());

    return title;
}
void ASPhpSongsQuery::parse() noexcept
{
    m_SearchResults.clear();

    for (int i{2}; i < m_PreparedReply.size(); i += 3)
    {
        SearchData sData;

        sData.songID = m_PreparedReply[i].section(QRegExp("\"."), 1, 1).toInt();
        int start (2 + m_PreparedReply[i].indexOf("b>"));
        int end   (    m_PreparedReply[i].indexOf("</"));

        if (start == end) sData.name = "{ #" + QString::number(sData.songID) + "_UNNAMED! }";
        else              sData.name = as::fixName(QStringRef(&m_PreparedReply[i], start, end - start).toString());

        m_SearchResults.emplace_back(std::move(sData));
    }
    m_IsSuccessfull = true;
}



//=====================================================:: ASPhpArtistsQuery ::==============================-=======================

ASPhpArtistsQuery::ASPhpArtistsQuery(const QString &songName, QObject *parent) noexcept
    : ASPhpGetSongsQuery(parent)
{
    m_URLTemplate = "http://www.audio-surf.com/ext/song_search.php?q=%1";
    if (!songName.isEmpty())
        setSearchingTarget(songName);
}

SongTitle ASPhpArtistsQuery::songTitle(int index)
{
    SongTitle title;

    title.artistName = m_SearchResults[index].name;
    QUrl song(m_QueryString.rightRef(m_QueryString.size() - m_QueryString.indexOf('=') - 1).toString());
    title.songName   = as::fixName(song.toString());

    return title;
}
void ASPhpArtistsQuery::parse() noexcept
{
    m_SearchResults.clear();

    int line(2);
    while (!m_PreparedReply[line].contains('#'))
        if (++line == m_PreparedReply.size()) return;

    for (; line < m_PreparedReply.size(); line += 3)
    {
        int   songID(m_PreparedReply[line].section(QRegExp("\"."), 1, 1).toInt());
        QString name(m_PreparedReply[line].section('\''          , 1, 1)        );

        if (name.isEmpty())
            name = "{ #" + QString::number(songID) + "_UNNAMED! }";

        m_SearchResults.push_back({songID, as::fixName(name)});
    }
    m_IsSuccessfull = true;
}

















