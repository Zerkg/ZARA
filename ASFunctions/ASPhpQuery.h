#ifndef ASPHPQUERY_H
#define ASPHPQUERY_H

#include "ASFunctions_global.h"
#include "ASAfterrideBonus.h"

#include "ASTypes.h"
#include "ASLeaderboard.h"

#include <QObject>
#include <QTime>

#include <memory>

class QNetworkAccessManager;
class ASLeaderboard;

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

    virtual void parse() noexcept {}
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

    struct Header
    {
        QString artistName;
        QString songName;
    };

    ASPhpLeaderboardQuery(int leaderboardID = -1, QObject *parent = nullptr) noexcept;

    void reset() noexcept;
    void setLeaderboardID(int id) noexcept;

    const ASLeaderboard* getLeaderboard(as::Mode mode) const noexcept;

    Header getHeader() const noexcept;

protected:

    virtual void parse() noexcept override;

private:

    Header m_Header;
    std::array<ASLeaderboard, 3> m_Leaderboards;

};

struct ASFUNCTIONS_EXPORT SearchData
{
    int songID;
    QString name;
};

class ASFUNCTIONS_EXPORT ASPhpGetSongsQuery : public ASPhpQuery
{
public:

    ASPhpGetSongsQuery(QObject *parent = nullptr) noexcept;

    std::vector<SearchData> getSearchResult() noexcept;
    void setSearchingTarget(const QString& name) noexcept;

protected:

    virtual bool errorCheck() noexcept override;

protected:

    std::vector<SearchData> m_Songs;

};

class ASFUNCTIONS_EXPORT ASPhpSongsQuery : public ASPhpGetSongsQuery
{
public:

    ASPhpSongsQuery(const QString &artistName = QString(), QObject *parent = nullptr) noexcept;

protected:

    virtual void parse() noexcept override;

};
class ASFUNCTIONS_EXPORT ASPhpArtistsQuery : public ASPhpGetSongsQuery
{
public:

    ASPhpArtistsQuery(const QString &songName = QString(), QObject *parent = nullptr) noexcept;

protected:

    virtual void parse() noexcept override;

};

#endif // ASPHPQUERY_H
