#ifndef ASPHPQUERY_H
#define ASPHPQUERY_H

#include "ASFunctions_global.h"
#include "ASTypes.h"

#include <QObject>

#include <deque>
#include <set>

struct PlayerStats;

class QNetworkAccessManager;

struct ASFUNCTIONS_EXPORT SongTitle
{
    QString artistName;
    QString songName;
};
struct ASFUNCTIONS_EXPORT SearchData
{
    int songID;
    QString name;
};


class ASFUNCTIONS_EXPORT ASPhpQuery : public QObject
{
Q_OBJECT
public:

    ASPhpQuery(const QString &query = QString(), QObject *parent = nullptr) noexcept;

    void execute() noexcept;
    void waitForFinished() const noexcept;
    void setQueryString(const QString &query) noexcept;
    bool isSuccessfull() const noexcept;

protected:

    virtual void parse() noexcept = 0;
    virtual bool errorCheck() noexcept;

    QNetworkAccessManager *m_ASWebsiteConnection;

    QByteArray m_RawReplyData;
    QString m_QueryString;
    QStringList m_PreparedReply;
    QString m_URLTemplate;

    bool m_IsFinished, m_IsSuccessfull;

private slots:

    void updateData() noexcept;

signals:

    void dataProcessed();

};


class ASFUNCTIONS_EXPORT ASPhpStatsQuery : public ASPhpQuery
{
public:

    ASPhpStatsQuery(PlayerStats *output, QObject *parent = nullptr) noexcept;

    void setRideID(const QString& rid) noexcept;
    void setOutput(PlayerStats *output) noexcept;

protected:

    QString m_RideId;
    PlayerStats *m_Output;

};

class ASFUNCTIONS_EXPORT ASPhpBaseStatsQuery : public ASPhpStatsQuery
{
public:

    ASPhpBaseStatsQuery(PlayerStats *output, const QString& rid = QString(), QObject *parent = nullptr) noexcept;

protected:

    virtual void parse() noexcept override;
};
class ASFUNCTIONS_EXPORT ASPhpExtStatsQuery : public ASPhpStatsQuery
{
public:

    ASPhpExtStatsQuery(PlayerStats *output, const QString& rid = QString(), QObject *parent = nullptr) noexcept;

protected:

    virtual void parse() noexcept override;
};


class ASFUNCTIONS_EXPORT ASPhpLeaderboardQuery : public ASPhpQuery
{
public:

    using LeaderboardData = std::deque<PlayerStats>;

    ASPhpLeaderboardQuery(int leaderboardID = -1, QObject *parent = nullptr) noexcept;

    void reset() noexcept;
    void setLeaderboardID(int id) noexcept;

    const LeaderboardData* leaderboard(as::Mode mode) const;
    std::set<as::Mode> availableLeaderboards() const;

    SongTitle getSongTitle() const noexcept;

protected:

    virtual void parse() noexcept override;

private:

    SongTitle m_SongTitle;
    std::map<as::Mode, LeaderboardData> m_Leaderboards;

};


class ASFUNCTIONS_EXPORT ASPhpGetSongsQuery : public ASPhpQuery
{
public:

    using Data = std::vector<SearchData>;

    ASPhpGetSongsQuery(QObject *parent = nullptr) noexcept;

    const Data* searchResults() const noexcept;

    void setSearchingTarget(const QString& name) noexcept;

    SearchData searchResult(size_t index           ) const noexcept;
    SearchData searchResult(const QString& itemName) const noexcept;

    virtual SongTitle songTitle(int index) = 0;

protected:

    virtual bool errorCheck() noexcept override;

protected:

    std::vector<SearchData> m_SearchResults;

};

class ASFUNCTIONS_EXPORT ASPhpSongsQuery : public ASPhpGetSongsQuery
{
public:

    ASPhpSongsQuery(const QString &artistName = QString(), QObject *parent = nullptr) noexcept;

    virtual SongTitle songTitle(int index) override;

protected:

    virtual void parse() noexcept override;

};
class ASFUNCTIONS_EXPORT ASPhpArtistsQuery : public ASPhpGetSongsQuery
{
public:

    ASPhpArtistsQuery(const QString &songName = QString(), QObject *parent = nullptr) noexcept;

    virtual SongTitle songTitle(int index) override;

protected:

    virtual void parse() noexcept override;

};

#endif // ASPHPQUERY_H
