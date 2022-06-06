#ifndef ASLEADERBOARDSEARCHER_H
#define ASLEADERBOARDSEARCHER_H

#include "ASLeaderboardSearcher_global.h"

#include <QWidget>

struct SearchRecord;
class HistoryModel;

namespace Ui {
class ASLeaderboardSearcher;
}

class ASLEADERBOARDSEARCHER_EXPORT ASLeaderboardSearcher : public QWidget
{
    Q_OBJECT

public:

    explicit ASLeaderboardSearcher(QWidget *parent = nullptr);
    ~ASLeaderboardSearcher();

protected:

    virtual void timerEvent(QTimerEvent *event) override;

private:

    void init();

    Ui::ASLeaderboardSearcher *ui;
    HistoryModel *m_HistoryModel;

private slots:

    void saveSearchResult(const SearchRecord &hr);

signals:

    void songSelected(int);

};

#endif // ASLEADERBOARDSEARCHER_H
