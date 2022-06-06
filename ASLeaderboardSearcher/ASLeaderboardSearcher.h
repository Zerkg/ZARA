#ifndef ASLEADERBOARDSEARCHER_H
#define ASLEADERBOARDSEARCHER_H

#include "ASLeaderboardSearcher_global.h"

#include <QWidget>

class SearchHistoryWidget;

namespace Ui {
class ASLeaderboardSearcher;
}

class ASLEADERBOARDSEARCHER_EXPORT ASLeaderboardSearcher : public QWidget
{
    Q_OBJECT

public:

    explicit ASLeaderboardSearcher(QWidget *parent = nullptr);
    ~ASLeaderboardSearcher();

private:

    Ui::ASLeaderboardSearcher *ui;
    QVector<int> m_SongsIDs;
    SearchHistoryWidget *m_History;
    int m_LeaderboardId;

    void init();

private slots:

    void search();
    void displaySearchResults();
    void saveInHistory(int leaderboardID);

signals:

    void songSelected(int);

};

#endif // ASLEADERBOARDSEARCHER_H
