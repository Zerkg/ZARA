#include "ASPhpQuery.h"

#include <QCoreApplication>
#include <QNetworkReply>

#include "ASFunctions.h"
#include "ASCharacter.h"
#include "ASAfterrideBonus.h"

using namespace as;


//========================================================:: ASPhpQuery ::==========================================================

ASPhpQuery::ASPhpQuery(const QString &query, QObject *parent) noexcept
    : QObject(parent)
    , m_ASWebsiteConnection(new QNetworkAccessManager(this))
    , m_QueryString(query)
    , m_IsFinished(false) {}


void ASPhpQuery::execute() noexcept
{
    m_IsFinished = false;

    QNetworkRequest request(m_QueryString);
    QObject::connect(m_ASWebsiteConnection->get(request), &QNetworkReply::finished, this, &ASPhpQuery::updateData);
}

void ASPhpQuery::waitForFinished() const noexcept
{
    while (!m_IsFinished) QCoreApplication::processEvents();
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
        {
            parse();
            m_IsSuccessfull = true;
        }
        else m_IsSuccessfull = false;

        m_IsFinished = true;
        emit dataProcessed();
    }
    else m_IsSuccessfull = false;

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
    m_Output->bonuses.reset();

    QString duration(QString::number(as::intFromString(m_PreparedReply[5])));
    duration.insert(duration.size() - 2, ':');

    m_Output->trackDuration = QTime::fromString(duration, duration.size() == 5 ? "mm:ss" : "m:ss");

    for (int i{8}; m_PreparedReply[i].size() != 21; ++i)
    {
        switch(m_PreparedReply[i][m_PreparedReply[i].size() - 9].toLatin1())
        {
        case 'd': m_Output->bonuses.setBonus(ASAfterrideBonus::IRONMODE    ); continue;

        case 's': m_Output->bonuses.setBonus(ASAfterrideBonus::CLEAN_FINISH); continue;

        case 'h': m_Output->bonuses.setBonus(ASAfterrideBonus::MATCH_7     ); continue;
        case '1': m_Output->bonuses.setBonus(ASAfterrideBonus::MATCH_11    ); continue;
        case '2': m_Output->bonuses.setBonus(ASAfterrideBonus::MATCH_21    ); continue;

        case 't': m_Output->bonuses.setBonus(ASAfterrideBonus::STEALTH     ); continue;

        case 'e': m_Output->bonuses.setBonus(ASAfterrideBonus::SEEING_RED  ); continue;
        case 'j': m_Output->bonuses.setBonus(ASAfterrideBonus::BUTTER_NINJA); continue;

        default: continue;
        }
    }
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
    m_Output->skillRaiting = as::intFromString(m_PreparedReply[13]);
    if (m_Output->skillRaiting.value() == 0)                      //Not "too old" score check (too old scores have no extended stats)
    {
        m_Output->skillRaiting = std::nullopt;
        return;
    }
    m_Output->character->setAbilityStats(as::intFromString(m_PreparedReply[3]), as::intFromString(m_PreparedReply[4]));

    m_Output->overfillsCount                = as::intFromString   (m_PreparedReply[5 ]);
    m_Output->longestChainTime              = as::intFromString   (m_PreparedReply[6 ]);
    m_Output->droppedChains                 = as::intFromString   (m_PreparedReply[7 ]);
    m_Output->bestCluster                   = as::intFromString   (m_PreparedReply[8 ]);
    m_Output->avgClusterSize                = as::doubleFromString(m_PreparedReply[9 ]);
    m_Output->avgColorsCount                = as::doubleFromString(m_PreparedReply[10]);
    m_Output->maxConsecutiveBlocksDodged    = as::intFromString   (m_PreparedReply[11]);
    m_Output->maxConsecutiveBlocksCollected = as::intFromString   (m_PreparedReply[12]);

    if (m_PreparedReply.size() > 50)
    {
        int tableData[]{31, 40, 49};         //Table data are indexes of each block stat (collected n, derived % and collected %)

        for (int row{}; row < 3; ++row)
            for (int column{}; column < 6; ++column)
                m_Output->blocksStats[row][column] = as::intFromString(m_PreparedReply[tableData[row] + column]);

        m_Output->paintsUsed      = as::intFromString(m_PreparedReply[60]);
        m_Output->shtormsUsed     = as::intFromString(m_PreparedReply[61]);
        m_Output->multipliersUsed = as::intFromString(m_PreparedReply[62]);
        m_Output->sortsUsed       = as::intFromString(m_PreparedReply[63]);
    }
}



//===================================================:: ASPhpLeaderboardQuery ::====================================================

ASPhpLeaderboardQuery::ASPhpLeaderboardQuery(int leaderboardID, QObject *parent) noexcept
    : ASPhpQuery(QString(), parent)
    , m_Leaderboards({ CASUAL, PRO, ELITE })
{
    m_URLTemplate = "http://www.audio-surf.com/ext/song_getSong.php?sid=%1";
    if (leaderboardID > 0) m_QueryString = m_URLTemplate.arg(leaderboardID);
}


void ASPhpLeaderboardQuery::reset() noexcept
{
    for (int i{}; i < 3; ++i)
        m_Leaderboards[i].reset();
}


void ASPhpLeaderboardQuery::setLeaderboardID(int id) noexcept
{
    m_QueryString = m_URLTemplate.arg(id);
}

const ASLeaderboard *ASPhpLeaderboardQuery::getLeaderboard(as::Mode mode) const noexcept
{
    return &m_Leaderboards[mode];
}

ASPhpLeaderboardQuery::Header ASPhpLeaderboardQuery::getHeader() const noexcept
{
    return m_Header;
}

void ASPhpLeaderboardQuery::parse() noexcept
{
    for (int leaderboardID{}, line{12}, pos{}; leaderboardID < 3; line += (12 + leaderboardID * 2), ++leaderboardID, pos = 0)
    {
        m_Leaderboards[leaderboardID].reset();

        while (m_PreparedReply[++line].contains("id="))
        {
            PlayerStats pStats;

            QStringRef characterName(&m_PreparedReply[line], m_PreparedReply[line].indexOf("t/") + 2, 2);

            if      (characterName == "mo" || characterName == "ni") pStats.character.reset(new Mono        (static_cast<as::Mode>(leaderboardID)));
            else if (characterName == "po"                         ) pStats.character.reset(new Pointman    (static_cast<as::Mode>(leaderboardID)));
            else if (characterName == "dv"                         ) pStats.character.reset(new DoubleVision(static_cast<as::Mode>(leaderboardID)));
            else if (characterName == "er"                         ) pStats.character.reset(new Eraser      (static_cast<as::Mode>(leaderboardID)));
            else if (characterName == "pu"                         ) pStats.character.reset(new Pusher      (static_cast<as::Mode>(leaderboardID)));
            else if (characterName == "ve"                         ) pStats.character.reset(new Vegas       (                                    ));

            pStats.rID       = m_PreparedReply[line++].section('"', 3, 3);
            pStats.nickname  = m_PreparedReply[line++].section('"', 3, 3);
            pStats.score     = as::intFromString(m_PreparedReply[line++]);

            m_Leaderboards[leaderboardID].emplaceStat(std::move(pStats));

            ASPhpBaseStatsQuery *base(new ASPhpBaseStatsQuery(&m_Leaderboards[leaderboardID][pos], pStats.rID, this));
            ASPhpExtStatsQuery  *ext (new ASPhpExtStatsQuery (&m_Leaderboards[leaderboardID][pos], pStats.rID, this));

            QObject::connect(base, &ASPhpQuery::dataProcessed, this, [=](){ m_Leaderboards[leaderboardID].setValid(pos, ASLeaderboard::BASE_STATS); base->deleteLater(); });
            QObject::connect(ext , &ASPhpQuery::dataProcessed, this, [=](){ m_Leaderboards[leaderboardID].setValid(pos, ASLeaderboard::EXT_STATS ); ext ->deleteLater(); });

            base->execute();
            ext ->execute();

            ++pos;
        }
    }

    int start (1 + m_PreparedReply[1].indexOf(QRegExp(">[^<]"   )));
    int end   (    m_PreparedReply[1].indexOf(QRegExp("[^>\\s]<")));

    m_Header.songName   = as::fixName(QStringRef(&m_PreparedReply[1], start, end - start + 1).toString());
    m_Header.artistName = as::fixName(m_PreparedReply[1].section('\'', 1, 1));
}



//=====================================================:: ASPhpGetSongsQuery ::======================================================

ASPhpGetSongsQuery::ASPhpGetSongsQuery(QObject *parent) noexcept
    : ASPhpQuery(QString(), parent){}

std::vector<SearchData> ASPhpGetSongsQuery::getSearchResult() noexcept
{
    return m_Songs;
}

void ASPhpGetSongsQuery::setSearchingTarget(const QString &name) noexcept
{
    QUrl sT(m_URLTemplate.arg(name));
    m_QueryString = sT.toEncoded();
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
    if (!artistName.isEmpty()) m_QueryString = m_URLTemplate.arg(artistName);
}
void ASPhpSongsQuery::parse() noexcept
{
    m_Songs.clear();

    for (int i{2}; i < m_PreparedReply.size(); i += 3)
    {
        SearchData sData;

        sData.songID = m_PreparedReply[i].section(QRegExp("\"."), 1, 1).toInt();
        int start (2 + m_PreparedReply[i].indexOf("b>"));
        int end   (    m_PreparedReply[i].indexOf("</"));

        if (start == end) sData.name = "#" + QString::number(sData.songID) + "_UNNAMED!";
        else              sData.name = as::fixName(QStringRef(&m_PreparedReply[i], start, end - start).toString());

        m_Songs.emplace_back(std::move(sData));
    }
}



//=====================================================:: ASPhpArtistsQuery ::==============================-=======================

ASPhpArtistsQuery::ASPhpArtistsQuery(const QString &songName, QObject *parent) noexcept
    : ASPhpGetSongsQuery(parent)
{
    m_URLTemplate = "http://www.audio-surf.com/ext/song_search.php?q=%1";
    if (!songName.isEmpty()) m_QueryString = m_URLTemplate.arg(songName);
}
void ASPhpArtistsQuery::parse() noexcept
{
    m_Songs.clear();

    int line(2);
    while (!m_PreparedReply[line].contains('#'))
        if (++line == m_PreparedReply.size()) return;

    for (; line < m_PreparedReply.size(); line += 3)
    {
        int   songID(m_PreparedReply[line].section(QRegExp("\"."), 1, 1).toInt());
        QString name(m_PreparedReply[line].section('\''          , 1, 1)        );


        m_Songs.emplace_back(SearchData({songID, as::fixName(name)}));
    }
}
